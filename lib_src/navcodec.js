import native from '../build/Release/navcodec.node';
import avconv from './avconv';
import fs from 'fs-promise';
import _ from 'underscore';
import printf from 'printf';

const THUMBNAIL_DEFAULTS = {
  pix_fmt:native.PixelFormat.PIX_FMT_YUVJ420P,
  width:64,
  height:64,
  bit_rate:1000
};

export default class NAVCodec
{
    static CodecId = native.CodecId;

    static async open(input, options = {})
    {
        // Take first frame if the input is a sequence
        const pInput = printf(input, 0);

        const stats = await fs.stat(pInput);
        const inputFormat = new native.NAVFormat(pInput);
        const media = new Media(input, inputFormat, options, stats.size);
        return media;
    }

    static async relocateMoov(input, output)
    {
        await new Promise((resolve, reject) => {
            native.relocateMoov(input, output, err => {
                if (err) {
                    reject(err);
                }

                resolve();
            });
        });
    }

    stringToCodec(str)
    {
        var codecId = native.CodecId[`CODEC_ID_${str.toUpperCase()}`];

        if (!codecId) {
            throw new Error("Invalid codec id");
        }

        return codecId;
    }

    gcd(x, y)
    {
  	     while (y !== 0) {
            var z = x % y;
            [x, y] = [y, z];
        }

        return x;
      }

    calculateVideoBitrate(options = {})
    {
        const width = options.width || 640;
        const height = options.height || width;
        const fps = options.fps || 30;

        return width * height * fps * 0.075;
    }

    fitKeepingRatio(aspectRatio, newSize)
    {
        const dstSize = {};

        if (aspectRatio > newSize.width /newSize.heigth) {
            dstSize.width = newSize.width;
            dstSize.height = Math.round(newSize.width * (1/aspectRatio));
        } else {
            dstSize.width = Math.round(newSize.height * aspectRatio);
            dstSize.height = newSize.height;
        }

        if (dstSize.width % 2 == 1) {
            dstSize.width -= 1;
        }

        if (dstSize.height % 2 == 1) {
            dstSize.height -= 1;
        }

        return dstSize;
    }
}


class Media
{
    constructor(inputFile, inputFormat, inputOptions, fileSize)
    {
        var streams = inputFormat.streams || [],
        videoStreams = [],
        audioStreams = [];

        // Find video and audio streams
        for (let stream of streams) {
            if (stream.codec.codec_type === 'Video') {
                videoStreams.push(stream);
            } else if (stream.codec.codec_type === 'Audio') {
                audioStreams.push(stream);
            }
        }

        this.inputOptions = Object.assign({
            y : true,
            stats : true,
        }, inputOptions);

        this.inputFile = inputFile;
        this.inputFormat = inputFormat;

        this.metadata = inputFormat.metadata;
        this.duration = inputFormat.duration;

        this.fileSize = fileSize;
        this.videoStreams = videoStreams;
        this.audioStreams = audioStreams;

        this.width = this.videoStreams[0] && this.videoStreams[0].codec.width;
        this.height = this.videoStreams[0] && this.videoStreams[0].codec.height;

        this.videoBitrate = this._bitrate(videoStreams);
        this.audioBitrate = this._bitrate(audioStreams);

        this.bitrate = this.audioBitrate + this.videoBitrate;
        this.samplerate = this.audioStreams[0] && this.audioStreams[0].codec.sample_rate;

        this._outputs = [];
        this._thumbnails = [];
    }

    info()
    {
      this.inputFormat.dump();
      return this;
    }

    addOutput(filename, options = {})
    {
      options = options || {};
      this._outputs.push({filename:filename, options:options});
      return this;
    }

    thumbnail(filenames, options = {})
    {
      this._thumbnails.push({filenames:filenames, options:options});
      return this;
    }

    addPyramid(filename, maxSize, minSize, options)
    {
        const inputVideo = this.videoStreams[0];
        const outputs = [];

        options = options || options;
        maxSize.width = maxSize.width || maxSize.height;
        minSize.width = minSize.width || minSize.height;

        if (inputVideo) {
            let aspectRatio = inputVideo.codec.width / inputVideo.codec.height;
            let size = maxSize;
            let bitrate;

            if (options.videoBitrate) {
                if (_.isString(options.videoBitrate)) {
                    var match = options.videoBitrate.match(/([0-9]+)k/);

                    if (match) {
                        bitrate = parseFloat(match[1]) * 1000;
                    } else {
                        bitrate = options.videoBitrate;
                    }
                }
            } else {
                bitrate = calculateVideoBitrate(options);
            }

            while (size.width >= minSize.width || size.height >= minSize.height) {
              let output = {options: _.clone(options)};

                if (options.keepAspectRatio)　{
                  let aspectRatioSize = fitKeepingRatio(aspectRatio, size);
                  _.extend(output.options, aspectRatioSize);
                } else {
                  _.extend(output.options, size);
                }

                output.options.videoBitrate = bitrate;
                output.filename = output.options.height + '-' + filename;

                bitrate = bitrate / 4;
                size.width = size.width / 2;
                size.height = size.height / 2;

                outputs.push(output);
            }
        }

        _.each(outputs, output => {
            this.addOutput(output.filename, output.options);
        });

        return outputs;
    }

    async transcode()
    {
        let inputVideo = null;
        let inputAudio = null;
        let outputs = this._outputs;
        let numOutputs = outputs.length;
        let thumbnails = this._thumbnails;
        let numThumbnails = thumbnails.length;
        let hasVideo = false;
        let hasAudio = false;

        const time = Date.now();

        inputVideo = this.videoStreams[0];
        inputAudio = this.audioStreams[0];

        let numFrames;
        if (inputVideo) {
            numFrames = inputVideo.numFrames;
            if (!numFrames && inputVideo.frameRate) {
                numFrames = (this.duration * inputVideo.frameRate.num) / inputVideo.frameRate.den;
            }
        }

        if (inputAudio) {
            numFrames = numFrames || inputAudio.numFrames;
        }

        numFrames = numFrames || 1000;

        const outputFiles = {};
        for (let output of outputs) {
            var opts = output.options;

            if (opts.keepAspectRatio)　{
                var dims = fitKeepingRatio(inputVideo.codec.width /　inputVideo.codec.height, opts);
                opts.width = dims.width;
                opts.height = dims.height;
            }

            // Force dimensions to multiple of 16 x multiple of 2
            opts.width -= opts.width % 16;
            opts.height-= opts.height % 2;

            outputFiles[output.filename] = this._convertOptions(opts, inputVideo);
        }

        if　(inputVideo)　{
            for　(let thumbnail of thumbnails){
                let options = _.defaults(thumbnail.options, THUMBNAIL_DEFAULTS);

                if (options.keepAspectRatio) {
                    var dims = fitKeepingRatio(inputVideo.codec.width/inputVideo.codec.height, options);
                    options.width = dims.width;
                    options.height = dims.height;
                }

                var filenames = thumbnail.filenames;
                for (let filename of filenames) {
                    var thumbOpts = _.clone(options);
                    thumbOpts.offset = filename;
                    outputFiles[filename] = this._convertOptions(thumbOpts, inputVideo)
                    outputFiles[filename].vframes = 1;
                }
            }
        }

        const inputFiles = {};
        inputFiles[this.inputFile] = this._convertOptions(this.inputOptions, inputVideo);

        await avconv(inputFiles, outputFiles, this._convertOptions({}, inputVideo));

        // function(err, finished, status){
        //   if(finished){
        //     cb && cb(err, 100, true, Date.now()-time);
        //   }else{
        //     cb && cb(err, ((status.frames / numFrames) * 100) % 101, false);
        //   }
        // });
    }

    _bitrate(streams)
    {
        let bitrate = 0;

        for　(let stream of streams)　{
            bitrate += stream.codec.bit_rate;
        }

        return bitrate;
    }

    _convertOptions(opts, inputVideo)
    {
        const result = {};

        if (opts.width && opts.height) {
            if (opts.rotate) {
                let tmp = opts.width;
                opts.width = opts.height;
                opts.height = tmp;
            }

            result.s = opts.width + 'x' + opts.height;
        }

        _.each(opts, (value, key) => {
            switch (key) {
                case 'y':
                    result['y'] = true;
                break;

                case 'stats':
                    result['stats'] = true;
                break;

                case 'sampleRate':
                    result['ar'] = value;
                break;

                case 'videoBSF':
                    result['bsf'] = value;
                break;

                case 'streamCopy':
                    result['codec'] = "copy";
                break;

                case 'loglevel':
                    if (['error', 'quiet', 'verbose', 'debug'].includes(value)) {
                        result['loglevel'] = value;
                    }
                break;

                case 'itsoffset':
                    result['itsoffset'] = value;
                break;

                case 'format':
                    result['f'] = value;
                break;

                case 'fps':
                    result.r = value;
                break;

                case 'minFps':
                    if (inputVideo.frameRate) {
                        if ((inputVideo.frameRate.num / inputVideo.frameRate.den) < value) {
                            result.r = value;
                        }
                    } else {
                        result.r = value;
                    }
                break;

                case 'videoBitrate':
                    if (opts.crf) {
                        result['maxrate'] = result['maxrate'] || value;
                        result['bufsize'] = result['bufsize'] || '1000k';
                    } else　{
                        result['b:v'] = value;
                    }
                break;

                case 'videoCodec':
                    result.vcodec = value;
                break;

                case 'keepAspectRatio':
                break;

                case 'offset':
                    result.ss = value;
                break;

                case 'skipVideo':
                    result.vn = true;
                break;

                case 'audioBitrate':
                    result['b:a'] = value;
                break;

                case 'audioCodec':
                    result.acodec = value;
                break;

                case 'channels':
                    result.ac = value;
                break;

                case 'skipAudio':
                    result.an = true;
                break;

                case 'rotate':
                    if(value === 'clock'){
                        result.vf = 'transpose=1';
                    }　else if (value === 'cclock') {
                        result.vf = 'transpose=2';
                    }
                break;

                case 'crf':
                case 'preset':
                    result[key] = value;
                break;

                case 'videoProfile':
                    result['profile:v'] = value;
                break;
            }
      });

      return result;
    }
}
