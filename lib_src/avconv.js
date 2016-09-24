"use strict";

import {child} from 'child_process';
import _ from 'underscore';

// infiles = {filename: {opts}}
// outfiles: {filename: {opts}}
export default function avconv(infiles, outfiles, options)
{
    const args = optionsToArgs(options);

    const defer = {};
    defer.promise = new Promise((resolve, reject) => Object.assign(defer, {resolve, reject}));

    _.each(infiles, (opts, filename) => {
        if (!_.isEmpty(opts)) {
            args.push.apply(args, optionsToArgs(opts));
        }
        args.push('-i', filename);
    });

    _.each(outfiles, (opts, filename) => {
        if (!_.isEmpty(opts)) {
            args.push.apply(args, optionsToArgs(opts));
        }
        args.push('-strict', 'experimental', filename);
    });

    // args.unshift('--adjustment=15', 'avconv') // '-n 15'
    //args.unshift('avconv')
    args.unshift('ffmpeg');
    const _avconv = child.spawn('nice', args);

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
            defer.reject(new Error(`Exited with error code:${code}`));
        } else {
            defer.resolve(null, true);
        }
    });

    return defer.promise;
}

function optionsToArgs(opts)
{
    var args = [];

    if (opts) {
        _.each(opts, (value, key) => {
            args.push('-'+key);
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

    if　(lastLine) {
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
