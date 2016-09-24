module.exports =
/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};

/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {

/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId])
/******/ 			return installedModules[moduleId].exports;

/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			exports: {},
/******/ 			id: moduleId,
/******/ 			loaded: false
/******/ 		};

/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);

/******/ 		// Flag the module as loaded
/******/ 		module.loaded = true;

/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}


/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;

/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;

/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";

/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(0);
/******/ })
/************************************************************************/
/******/ ([
/* 0 */
/***/ function(module, exports, __webpack_require__) {

	'use strict';

	Object.defineProperty(exports, "__esModule", {
	    value: true
	});

	var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

	var _navcodec = __webpack_require__(1);

	var _navcodec2 = _interopRequireDefault(_navcodec);

	var _avconv = __webpack_require__(2);

	var _avconv2 = _interopRequireDefault(_avconv);

	var _fsPromise = __webpack_require__(5);

	var _fsPromise2 = _interopRequireDefault(_fsPromise);

	var _underscore = __webpack_require__(4);

	var _underscore2 = _interopRequireDefault(_underscore);

	var _printf = __webpack_require__(6);

	var _printf2 = _interopRequireDefault(_printf);

	var _stream = __webpack_require__(7);

	var _stream2 = _interopRequireDefault(_stream);

	function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

	function _asyncToGenerator(fn) { return function () { var gen = fn.apply(this, arguments); return new Promise(function (resolve, reject) { function step(key, arg) { try { var info = gen[key](arg); var value = info.value; } catch (error) { reject(error); return; } if (info.done) { resolve(value); } else { return Promise.resolve(value).then(function (value) { return step("next", value); }, function (err) { return step("throw", err); }); } } return step("next"); }); }; }

	function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

	const THUMBNAIL_DEFAULTS = {
	    pix_fmt: _navcodec2.default.PixelFormat.PIX_FMT_YUVJ420P,
	    width: 64,
	    height: 64,
	    bit_rate: 1000
	};

	let NAVCodec = function () {
	    function NAVCodec() {
	        _classCallCheck(this, NAVCodec);
	    }

	    _createClass(NAVCodec, [{
	        key: 'stringToCodec',
	        value: function stringToCodec(str) {
	            var codecId = _navcodec2.default.CodecId[`CODEC_ID_${ str.toUpperCase() }`];

	            if (!codecId) {
	                throw new Error("Invalid codec id");
	            }

	            return codecId;
	        }
	    }, {
	        key: 'gcd',
	        value: function gcd(x, y) {
	            while (y !== 0) {
	                var z = x % y;
	                [x, y] = [y, z];
	            }

	            return x;
	        }
	    }, {
	        key: 'calculateVideoBitrate',
	        value: function calculateVideoBitrate(options = {}) {
	            const width = options.width || 640;
	            const height = options.height || width;
	            const fps = options.fps || 30;

	            return width * height * fps * 0.075;
	        }
	    }, {
	        key: 'fitKeepingRatio',
	        value: function fitKeepingRatio(aspectRatio, newSize) {
	            const dstSize = {};

	            if (aspectRatio > newSize.width / newSize.heigth) {
	                dstSize.width = newSize.width;
	                dstSize.height = Math.round(newSize.width * (1 / aspectRatio));
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
	    }], [{
	        key: 'open',
	        value: (() => {
	            var _ref = _asyncToGenerator(function* (input, options = {}) {
	                // Take first frame if the input is a sequence
	                const pInput = (0, _printf2.default)(input, 0);

	                const stats = yield _fsPromise2.default.stat(pInput);
	                const inputFormat = new _navcodec2.default.NAVFormat(pInput);
	                return new Media(input, inputFormat, options, stats.size);
	            });

	            function open(_x, _x2) {
	                return _ref.apply(this, arguments);
	            }

	            return open;
	        })()
	    }, {
	        key: 'openStream',
	        value: (() => {
	            var _ref2 = _asyncToGenerator(function* (input, options = {}) {
	                console.log('wait');
	                const media = yield NAVCodec.open(input, options);
	                return new _stream2.default(media);
	            });

	            function openStream(_x3, _x4) {
	                return _ref2.apply(this, arguments);
	            }

	            return openStream;
	        })()
	    }, {
	        key: 'relocateMoov',
	        value: (() => {
	            var _ref3 = _asyncToGenerator(function* (input, output) {
	                yield new Promise(function (resolve, reject) {
	                    _navcodec2.default.relocateMoov(input, output, function (err) {
	                        if (err) {
	                            reject(err);
	                        }

	                        resolve();
	                    });
	                });
	            });

	            function relocateMoov(_x5, _x6) {
	                return _ref3.apply(this, arguments);
	            }

	            return relocateMoov;
	        })()
	    }]);

	    return NAVCodec;
	}();

	NAVCodec.CodecId = _navcodec2.default.CodecId;
	exports.default = NAVCodec;

	let Media = function () {
	    function Media(inputFile, inputFormat, inputOptions, fileSize) {
	        _classCallCheck(this, Media);

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
	            y: true,
	            stats: true
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

	    _createClass(Media, [{
	        key: 'info',
	        value: function info() {
	            this.inputFormat.dump();
	            return this;
	        }
	    }, {
	        key: 'addOutput',
	        value: function addOutput(filename, options = {}) {
	            options = options || {};
	            this._outputs.push({ filename: filename, options: options });
	            return this;
	        }
	    }, {
	        key: 'thumbnail',
	        value: function thumbnail(filenames, options = {}) {
	            this._thumbnails.push({ filenames: filenames, options: options });
	            return this;
	        }
	    }, {
	        key: 'addPyramid',
	        value: function addPyramid(filename, maxSize, minSize, options) {
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
	                    if (_underscore2.default.isString(options.videoBitrate)) {
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
	                    let output = { options: _underscore2.default.clone(options) };

	                    if (options.keepAspectRatio) {
	                        let aspectRatioSize = fitKeepingRatio(aspectRatio, size);
	                        _underscore2.default.extend(output.options, aspectRatioSize);
	                    } else {
	                        _underscore2.default.extend(output.options, size);
	                    }

	                    output.options.videoBitrate = bitrate;
	                    output.filename = output.options.height + '-' + filename;

	                    bitrate = bitrate / 4;
	                    size.width = size.width / 2;
	                    size.height = size.height / 2;

	                    outputs.push(output);
	                }
	            }

	            _underscore2.default.each(outputs, output => {
	                this.addOutput(output.filename, output.options);
	            });

	            return outputs;
	        }
	    }, {
	        key: 'transcode',
	        value: (() => {
	            var _ref4 = _asyncToGenerator(function* () {
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
	                        numFrames = this.duration * inputVideo.frameRate.num / inputVideo.frameRate.den;
	                    }
	                }

	                if (inputAudio) {
	                    numFrames = numFrames || inputAudio.numFrames;
	                }

	                numFrames = numFrames || 1000;

	                const outputFiles = {};
	                for (let output of outputs) {
	                    var opts = output.options;

	                    if (opts.keepAspectRatio) {
	                        var dims = fitKeepingRatio(inputVideo.codec.width / inputVideo.codec.height, opts);
	                        opts.width = dims.width;
	                        opts.height = dims.height;
	                    }

	                    // Force dimensions to multiple of 16 x multiple of 2
	                    opts.width -= opts.width % 16;
	                    opts.height -= opts.height % 2;

	                    outputFiles[output.filename] = this._convertOptions(opts, inputVideo);
	                }

	                if (inputVideo) {
	                    for (let thumbnail of thumbnails) {
	                        let options = _underscore2.default.defaults(thumbnail.options, THUMBNAIL_DEFAULTS);

	                        if (options.keepAspectRatio) {
	                            var dims = fitKeepingRatio(inputVideo.codec.width / inputVideo.codec.height, options);
	                            options.width = dims.width;
	                            options.height = dims.height;
	                        }

	                        var filenames = thumbnail.filenames;
	                        for (let filename of filenames) {
	                            var thumbOpts = _underscore2.default.clone(options);
	                            thumbOpts.offset = filename;
	                            outputFiles[filename] = this._convertOptions(thumbOpts, inputVideo);
	                            outputFiles[filename].vframes = 1;
	                        }
	                    }
	                }

	                const inputFiles = {};
	                inputFiles[this.inputFile] = this._convertOptions(this.inputOptions, inputVideo);

	                yield (0, _avconv2.default)(inputFiles, outputFiles, this._convertOptions({}, inputVideo));

	                // function(err, finished, status){
	                //   if(finished){
	                //     cb && cb(err, 100, true, Date.now()-time);
	                //   }else{
	                //     cb && cb(err, ((status.frames / numFrames) * 100) % 101, false);
	                //   }
	                // });
	            });

	            function transcode() {
	                return _ref4.apply(this, arguments);
	            }

	            return transcode;
	        })()
	    }, {
	        key: '_bitrate',
	        value: function _bitrate(streams) {
	            let bitrate = 0;

	            for (let stream of streams) {
	                bitrate += stream.codec.bit_rate;
	            }

	            return bitrate;
	        }
	    }, {
	        key: '_convertOptions',
	        value: function _convertOptions(opts, inputVideo) {
	            const result = {};

	            if (opts.width && opts.height) {
	                if (opts.rotate) {
	                    let tmp = opts.width;
	                    opts.width = opts.height;
	                    opts.height = tmp;
	                }

	                result.s = opts.width + 'x' + opts.height;
	            }

	            _underscore2.default.each(opts, (value, key) => {
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
	                            if (inputVideo.frameRate.num / inputVideo.frameRate.den < value) {
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
	                        } else {
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
	                        if (value === 'clock') {
	                            result.vf = 'transpose=1';
	                        } else if (value === 'cclock') {
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
	    }]);

	    return Media;
	}();

/***/ },
/* 1 */
/***/ function(module, exports) {

	module.exports = require("../build/Release/navcodec.node");

/***/ },
/* 2 */
/***/ function(module, exports, __webpack_require__) {

	"use strict";

	Object.defineProperty(exports, "__esModule", {
	    value: true
	});
	exports.default = avconv;

	var _child_process = __webpack_require__(3);

	var _underscore = __webpack_require__(4);

	var _underscore2 = _interopRequireDefault(_underscore);

	function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

	// infiles = {filename: {opts}}
	// outfiles: {filename: {opts}}
	function avconv(infiles, outfiles, options) {
	    const args = optionsToArgs(options);

	    const defer = {};
	    defer.promise = new Promise((resolve, reject) => Object.assign(defer, { resolve, reject }));

	    _underscore2.default.each(infiles, (opts, filename) => {
	        if (!_underscore2.default.isEmpty(opts)) {
	            args.push.apply(args, optionsToArgs(opts));
	        }
	        args.push('-i', filename);
	    });

	    _underscore2.default.each(outfiles, (opts, filename) => {
	        if (!_underscore2.default.isEmpty(opts)) {
	            args.push.apply(args, optionsToArgs(opts));
	        }
	        args.push('-strict', 'experimental', filename);
	    });

	    // args.unshift('--adjustment=15', 'avconv') // '-n 15'
	    //args.unshift('avconv')
	    args.unshift('ffmpeg');
	    const _avconv = _child_process.child.spawn('nice', args);

	    _avconv.stdout.on('data', data => {
	        //console.log('stdout: ' + data);
	    });

	    _avconv.stderr.on('data', data => {
	        // var status = parseStatus('' + data);
	        //
	        // if (status) {
	        //     // cb(null, false, status);
	        // } else {
	        //     // console.log(""+data);
	        // }
	    });

	    _avconv.on('exit', code => {
	        if (code !== 0) {
	            defer.reject(new Error(`Exited with error code:${ code }`));
	        } else {
	            defer.resolve(null, true);
	        }
	    });

	    return defer.promise;
	}

	function optionsToArgs(opts) {
	    var args = [];

	    if (opts) {
	        _underscore2.default.each(opts, (value, key) => {
	            args.push('-' + key);
	            if (value !== true) {
	                args.push(value);
	            }
	        });
	    }

	    return args;
	}

	const statusRegExp = /frame=\s*([0-9]+)\s*fps=\s*([0-9]+)\s*/;

	function parseStatus(stderrString, totalDurationSec) {
	    // get last stderr line
	    var lines = stderrString.split(/\r\n|\r|\n/g);
	    var lastLine = lines[lines.length - 2];
	    var ret;

	    if (lastLine) {
	        var match = lastLine.match(statusRegExp);
	        if (match) {
	            var ret = {
	                frames: match[1],
	                fps: match[2]
	            };
	        }
	    }

	    return ret;

	    // if (progress && progress.length > 10) {
	    //     // build progress report object
	    //     var ret = {
	    //         frames: parseInt(progress[1], 10),
	    //         currentFps: parseInt(progress[2], 10),
	    //         currentKbps: parseFloat(progress[10]),
	    //         targetSize: parseInt(progress[5], 10),
	    //         timemark: progress[6]
	    //     };
	    //
	    //     // calculate percent progress using duration
	    //
	    //     if (totalDurationSec && totalDurationSec > 0) {
	    //         ret.percent = (this.ffmpegTimemarkToSeconds(ret.timemark) / totalDurationSec) * 100;
	    //     }
	    //
	    //     this.options.onProgress(ret);
	    // }

	    // return ret;
	}

/***/ },
/* 3 */
/***/ function(module, exports) {

	module.exports = require("child_process");

/***/ },
/* 4 */
/***/ function(module, exports) {

	module.exports = require("underscore");

/***/ },
/* 5 */
/***/ function(module, exports) {

	module.exports = require("fs-promise");

/***/ },
/* 6 */
/***/ function(module, exports) {

	module.exports = require("printf");

/***/ },
/* 7 */
/***/ function(module, exports, __webpack_require__) {

	'use strict';

	Object.defineProperty(exports, "__esModule", {
	    value: true
	});

	var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

	var _navcodec = __webpack_require__(1);

	var _navcodec2 = _interopRequireDefault(_navcodec);

	function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

	function _asyncToGenerator(fn) { return function () { var gen = fn.apply(this, arguments); return new Promise(function (resolve, reject) { function step(key, arg) { try { var info = gen[key](arg); var value = info.value; } catch (error) { reject(error); return; } if (info.done) { resolve(value); } else { return Promise.resolve(value).then(function (value) { return step("next", value); }, function (err) { return step("throw", err); }); } } return step("next"); }); }; }

	function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

	let Stream = function () {
	    function Stream(media) {
	        _classCallCheck(this, Stream);

	        this._media = media;

	        this._stream = new _navcodec2.default.NAVStreamReader(media.inputFile);
	        console.log(this._stream.setMetaData({
	            width: media.width,
	            height: media.height,
	            framerate: media.videoStreams[0] ? media.videoStreams[0].codec.framerate : 0
	        }));
	    }

	    _createClass(Stream, [{
	        key: 'next',
	        value: (() => {
	            var _ref = _asyncToGenerator(function* () {
	                return this._stream.next();
	            });

	            function next() {
	                return _ref.apply(this, arguments);
	            }

	            return next;
	        })()
	    }]);

	    return Stream;
	}();

	exports.default = Stream;

/***/ }
/******/ ]);