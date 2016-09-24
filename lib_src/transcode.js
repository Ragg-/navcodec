"use strict";

import navcodec from '../build/Release/navcodec.node';
import fs from 'fs-promise';

export default async function transcode(input, output, preset)
{
    const stats = await fs.stat(input);
    let inputVideo = null;
    let inputAudio = null;

    let outputVideo = null;
    let outputAudio = null;

    // Open Input Video.
    const inputFormat = new navcodec.AVFormat(input);
    // console.log(inputFormat);

    // inputFormat.dump();

    // Find first video and first audio streams
    for (let stream of inputFormat.streams) {
        if (inputVideo === null && stream.codec.codec_type === 'Video') {
            inputVideo = stream;
        }

        if (inputAudio === null && stream.codec.codec_type === 'Audio') {
            inputAudio = stream;
        }
    }

    // console.log(inputVideo);
    // console.log(inputAudio);

    // Create Output Video
    const outputFormat = new navcodec.NAVOutputFormat(output)

    if (inputVideo) {
        let time_base = inputVideo.codec.time_base;
        time_base.num *= inputVideo.codec.ticks_per_frame;

        var options = {
            time_base: time_base,
            ticks_per_frame: 1,
            pix_fmt: inputVideo.codec.pix_fmt,
            width: inputVideo.codec.width,
            height: inputVideo.codec.height
        };

        preset.width && (options.width = preset.width);
        preset.height && (options.height = preset.height);
        preset.bit_rate_video && (options.bit_rate = preset.bit_rate_video);
        preset.video_codec && (options.codec = preset.video_codec);

        outputVideo = outputFormat.addStream("Video", options);

        // Create a converter (Only usefull if changing dimensions
        // and/or pixel format. Otherwise it will act as a passthrough filter.
        converter = new navcodec.NAVSws(inputVideo, outputVideo);
    }

    if (inputAudio) {
        var options = {};

        preset.bit_rate_audio && (options.bit_rate = preset.bit_rate_audio);
        preset.audio_codec && (options.codec = preset.audio_codec);

        outputAudio = outputFormat.addStream("Audio", options);
    }

    //
    // Start Encoder
    //
    outputFormat.begin();

    let counter = 0;
    const time = Date.now();

    let streams = [];
    inputAudio && streams.push(inputAudio);
    inputVideo && streams.push(inputVideo);

    await new Promise((resolve, reject) => {
        inputFormat.decode(streams, (stream, frame, pos) => {
            counter++;

            if (frame) {
                // if (counter % 500 === 0) {
                //     cb && cb(null, pos / stats.size);
                // }
                if (outputVideo && stream.codec.codec_type === outputVideo.codec.codec_type) {
                    frame = converter.convert(frame);
                    outputFormat.encode(outputVideo, frame);
                }

                if (outputAudio && stream.codec.codec_type === outputAudio.codec.codec_type) {
                    outputFormat.encode(outputAudio, frame);
                }
            } else {
                // No more frames, end encoding.
                outputFormat.end();
                resolve();
            }
        });
    });

    // console.log("Duration: " + (Date.now() - time));
    // console.log(counter);
}
