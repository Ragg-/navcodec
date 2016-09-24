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

	var _printf = __webpack_require__(8);

	var _printf2 = _interopRequireDefault(_printf);

	function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

	function _asyncToGenerator(fn) { return function () { var gen = fn.apply(this, arguments); return new Promise(function (resolve, reject) { function step(key, arg) { try { var info = gen[key](arg); var value = info.value; } catch (error) { reject(error); return; } if (info.done) { resolve(value); } else { return Promise.resolve(value).then(function (value) { return step("next", value); }, function (err) { return step("throw", err); }); } } return step("next"); }); }; }

	function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

	var THUMBNAIL_DEFAULTS = {
	    pix_fmt: _navcodec2.default.PixelFormat.PIX_FMT_YUVJ420P,
	    width: 64,
	    height: 64,
	    bit_rate: 1000
	};

	var NAVCodec = function () {
	    function NAVCodec() {
	        _classCallCheck(this, NAVCodec);
	    }

	    _createClass(NAVCodec, [{
	        key: 'stringToCodec',
	        value: function stringToCodec(str) {
	            var codecId = _navcodec2.default.CodecId['CODEC_ID_' + str.toUpperCase()];

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
	                var _ref = [y, z];
	                x = _ref[0];
	                y = _ref[1];
	            }

	            return x;
	        }
	    }, {
	        key: 'calculateVideoBitrate',
	        value: function calculateVideoBitrate() {
	            var options = arguments.length <= 0 || arguments[0] === undefined ? {} : arguments[0];

	            var width = options.width || 640;
	            var height = options.height || width;
	            var fps = options.fps || 30;

	            return width * height * fps * 0.075;
	        }
	    }, {
	        key: 'fitKeepingRatio',
	        value: function fitKeepingRatio(aspectRatio, newSize) {
	            var dstSize = {};

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
	        value: function () {
	            var _ref2 = _asyncToGenerator(regeneratorRuntime.mark(function _callee(input) {
	                var options = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];
	                var pInput, stats, inputFormat, media;
	                return regeneratorRuntime.wrap(function _callee$(_context) {
	                    while (1) {
	                        switch (_context.prev = _context.next) {
	                            case 0:
	                                // Take first frame if the input is a sequence
	                                pInput = (0, _printf2.default)(input, 0);
	                                _context.next = 3;
	                                return _fsPromise2.default.stat(pInput);

	                            case 3:
	                                stats = _context.sent;
	                                inputFormat = new _navcodec2.default.NAVFormat(pInput);
	                                media = new Media(input, inputFormat, options, stats.size);
	                                return _context.abrupt('return', media);

	                            case 7:
	                            case 'end':
	                                return _context.stop();
	                        }
	                    }
	                }, _callee, this);
	            }));

	            function open(_x2, _x3) {
	                return _ref2.apply(this, arguments);
	            }

	            return open;
	        }()
	    }, {
	        key: 'relocateMoov',
	        value: function () {
	            var _ref3 = _asyncToGenerator(regeneratorRuntime.mark(function _callee2(input, output) {
	                return regeneratorRuntime.wrap(function _callee2$(_context2) {
	                    while (1) {
	                        switch (_context2.prev = _context2.next) {
	                            case 0:
	                                _context2.next = 2;
	                                return new Promise(function (resolve, reject) {
	                                    _navcodec2.default.relocateMoov(input, output, function (err) {
	                                        if (err) {
	                                            reject(err);
	                                        }

	                                        resolve();
	                                    });
	                                });

	                            case 2:
	                            case 'end':
	                                return _context2.stop();
	                        }
	                    }
	                }, _callee2, this);
	            }));

	            function relocateMoov(_x5, _x6) {
	                return _ref3.apply(this, arguments);
	            }

	            return relocateMoov;
	        }()
	    }]);

	    return NAVCodec;
	}();

	NAVCodec.CodecId = _navcodec2.default.CodecId;
	exports.default = NAVCodec;

	var Media = function () {
	    function Media(inputFile, inputFormat, inputOptions, fileSize) {
	        _classCallCheck(this, Media);

	        var streams = inputFormat.streams || [],
	            videoStreams = [],
	            audioStreams = [];

	        // Find video and audio streams
	        var _iteratorNormalCompletion = true;
	        var _didIteratorError = false;
	        var _iteratorError = undefined;

	        try {
	            for (var _iterator = streams[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
	                var stream = _step.value;

	                if (stream.codec.codec_type === 'Video') {
	                    videoStreams.push(stream);
	                } else if (stream.codec.codec_type === 'Audio') {
	                    audioStreams.push(stream);
	                }
	            }
	        } catch (err) {
	            _didIteratorError = true;
	            _iteratorError = err;
	        } finally {
	            try {
	                if (!_iteratorNormalCompletion && _iterator.return) {
	                    _iterator.return();
	                }
	            } finally {
	                if (_didIteratorError) {
	                    throw _iteratorError;
	                }
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
	        value: function addOutput(filename) {
	            var options = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];

	            options = options || {};
	            this._outputs.push({ filename: filename, options: options });
	            return this;
	        }
	    }, {
	        key: 'thumbnail',
	        value: function thumbnail(filenames) {
	            var options = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];

	            this._thumbnails.push({ filenames: filenames, options: options });
	            return this;
	        }
	    }, {
	        key: 'addPyramid',
	        value: function addPyramid(filename, maxSize, minSize, options) {
	            var _this = this;

	            var inputVideo = this.videoStreams[0];
	            var outputs = [];

	            options = options || options;
	            maxSize.width = maxSize.width || maxSize.height;
	            minSize.width = minSize.width || minSize.height;

	            if (inputVideo) {
	                var aspectRatio = inputVideo.codec.width / inputVideo.codec.height;
	                var size = maxSize;
	                var bitrate = void 0;

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
	                    var output = { options: _underscore2.default.clone(options) };

	                    if (options.keepAspectRatio) {
	                        var aspectRatioSize = fitKeepingRatio(aspectRatio, size);
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

	            _underscore2.default.each(outputs, function (output) {
	                _this.addOutput(output.filename, output.options);
	            });

	            return outputs;
	        }
	    }, {
	        key: 'transcode',
	        value: function () {
	            var _ref4 = _asyncToGenerator(regeneratorRuntime.mark(function _callee3() {
	                var inputVideo, inputAudio, outputs, numOutputs, thumbnails, numThumbnails, hasVideo, hasAudio, time, numFrames, outputFiles, _iteratorNormalCompletion2, _didIteratorError2, _iteratorError2, _iterator2, _step2, output, opts, dims, _iteratorNormalCompletion3, _didIteratorError3, _iteratorError3, _iterator3, _step3, thumbnail, options, filenames, _iteratorNormalCompletion4, _didIteratorError4, _iteratorError4, _iterator4, _step4, filename, thumbOpts, inputFiles;

	                return regeneratorRuntime.wrap(function _callee3$(_context3) {
	                    while (1) {
	                        switch (_context3.prev = _context3.next) {
	                            case 0:
	                                inputVideo = null;
	                                inputAudio = null;
	                                outputs = this._outputs;
	                                numOutputs = outputs.length;
	                                thumbnails = this._thumbnails;
	                                numThumbnails = thumbnails.length;
	                                hasVideo = false;
	                                hasAudio = false;
	                                time = Date.now();


	                                inputVideo = this.videoStreams[0];
	                                inputAudio = this.audioStreams[0];

	                                numFrames = void 0;

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

	                                outputFiles = {};
	                                _iteratorNormalCompletion2 = true;
	                                _didIteratorError2 = false;
	                                _iteratorError2 = undefined;
	                                _context3.prev = 19;

	                                for (_iterator2 = outputs[Symbol.iterator](); !(_iteratorNormalCompletion2 = (_step2 = _iterator2.next()).done); _iteratorNormalCompletion2 = true) {
	                                    output = _step2.value;
	                                    opts = output.options;


	                                    if (opts.keepAspectRatio) {
	                                        dims = fitKeepingRatio(inputVideo.codec.width / inputVideo.codec.height, opts);

	                                        opts.width = dims.width;
	                                        opts.height = dims.height;
	                                    }

	                                    // Force dimensions to multiple of 16 x multiple of 2
	                                    opts.width -= opts.width % 16;
	                                    opts.height -= opts.height % 2;

	                                    outputFiles[output.filename] = this._convertOptions(opts, inputVideo);
	                                }

	                                _context3.next = 27;
	                                break;

	                            case 23:
	                                _context3.prev = 23;
	                                _context3.t0 = _context3['catch'](19);
	                                _didIteratorError2 = true;
	                                _iteratorError2 = _context3.t0;

	                            case 27:
	                                _context3.prev = 27;
	                                _context3.prev = 28;

	                                if (!_iteratorNormalCompletion2 && _iterator2.return) {
	                                    _iterator2.return();
	                                }

	                            case 30:
	                                _context3.prev = 30;

	                                if (!_didIteratorError2) {
	                                    _context3.next = 33;
	                                    break;
	                                }

	                                throw _iteratorError2;

	                            case 33:
	                                return _context3.finish(30);

	                            case 34:
	                                return _context3.finish(27);

	                            case 35:
	                                if (!inputVideo) {
	                                    _context3.next = 82;
	                                    break;
	                                }

	                                _iteratorNormalCompletion3 = true;
	                                _didIteratorError3 = false;
	                                _iteratorError3 = undefined;
	                                _context3.prev = 39;
	                                _iterator3 = thumbnails[Symbol.iterator]();

	                            case 41:
	                                if (_iteratorNormalCompletion3 = (_step3 = _iterator3.next()).done) {
	                                    _context3.next = 68;
	                                    break;
	                                }

	                                thumbnail = _step3.value;
	                                options = _underscore2.default.defaults(thumbnail.options, THUMBNAIL_DEFAULTS);


	                                if (options.keepAspectRatio) {
	                                    dims = fitKeepingRatio(inputVideo.codec.width / inputVideo.codec.height, options);

	                                    options.width = dims.width;
	                                    options.height = dims.height;
	                                }

	                                filenames = thumbnail.filenames;
	                                _iteratorNormalCompletion4 = true;
	                                _didIteratorError4 = false;
	                                _iteratorError4 = undefined;
	                                _context3.prev = 49;

	                                for (_iterator4 = filenames[Symbol.iterator](); !(_iteratorNormalCompletion4 = (_step4 = _iterator4.next()).done); _iteratorNormalCompletion4 = true) {
	                                    filename = _step4.value;
	                                    thumbOpts = _underscore2.default.clone(options);

	                                    thumbOpts.offset = filename;
	                                    outputFiles[filename] = this._convertOptions(thumbOpts, inputVideo);
	                                    outputFiles[filename].vframes = 1;
	                                }
	                                _context3.next = 57;
	                                break;

	                            case 53:
	                                _context3.prev = 53;
	                                _context3.t1 = _context3['catch'](49);
	                                _didIteratorError4 = true;
	                                _iteratorError4 = _context3.t1;

	                            case 57:
	                                _context3.prev = 57;
	                                _context3.prev = 58;

	                                if (!_iteratorNormalCompletion4 && _iterator4.return) {
	                                    _iterator4.return();
	                                }

	                            case 60:
	                                _context3.prev = 60;

	                                if (!_didIteratorError4) {
	                                    _context3.next = 63;
	                                    break;
	                                }

	                                throw _iteratorError4;

	                            case 63:
	                                return _context3.finish(60);

	                            case 64:
	                                return _context3.finish(57);

	                            case 65:
	                                _iteratorNormalCompletion3 = true;
	                                _context3.next = 41;
	                                break;

	                            case 68:
	                                _context3.next = 74;
	                                break;

	                            case 70:
	                                _context3.prev = 70;
	                                _context3.t2 = _context3['catch'](39);
	                                _didIteratorError3 = true;
	                                _iteratorError3 = _context3.t2;

	                            case 74:
	                                _context3.prev = 74;
	                                _context3.prev = 75;

	                                if (!_iteratorNormalCompletion3 && _iterator3.return) {
	                                    _iterator3.return();
	                                }

	                            case 77:
	                                _context3.prev = 77;

	                                if (!_didIteratorError3) {
	                                    _context3.next = 80;
	                                    break;
	                                }

	                                throw _iteratorError3;

	                            case 80:
	                                return _context3.finish(77);

	                            case 81:
	                                return _context3.finish(74);

	                            case 82:
	                                inputFiles = {};

	                                inputFiles[this.inputFile] = this._convertOptions(this.inputOptions, inputVideo);

	                                _context3.next = 86;
	                                return (0, _avconv2.default)(inputFiles, outputFiles, this._convertOptions({}, inputVideo));

	                            case 86:
	                            case 'end':
	                                return _context3.stop();
	                        }
	                    }
	                }, _callee3, this, [[19, 23, 27, 35], [28,, 30, 34], [39, 70, 74, 82], [49, 53, 57, 65], [58,, 60, 64], [75,, 77, 81]]);
	            }));

	            function transcode() {
	                return _ref4.apply(this, arguments);
	            }

	            return transcode;
	        }()
	    }, {
	        key: '_bitrate',
	        value: function _bitrate(streams) {
	            var bitrate = 0;

	            var _iteratorNormalCompletion5 = true;
	            var _didIteratorError5 = false;
	            var _iteratorError5 = undefined;

	            try {
	                for (var _iterator5 = streams[Symbol.iterator](), _step5; !(_iteratorNormalCompletion5 = (_step5 = _iterator5.next()).done); _iteratorNormalCompletion5 = true) {
	                    var stream = _step5.value;

	                    bitrate += stream.codec.bit_rate;
	                }
	            } catch (err) {
	                _didIteratorError5 = true;
	                _iteratorError5 = err;
	            } finally {
	                try {
	                    if (!_iteratorNormalCompletion5 && _iterator5.return) {
	                        _iterator5.return();
	                    }
	                } finally {
	                    if (_didIteratorError5) {
	                        throw _iteratorError5;
	                    }
	                }
	            }

	            return bitrate;
	        }
	    }, {
	        key: '_convertOptions',
	        value: function _convertOptions(opts, inputVideo) {
	            var result = {};

	            if (opts.width && opts.height) {
	                if (opts.rotate) {
	                    var tmp = opts.width;
	                    opts.width = opts.height;
	                    opts.height = tmp;
	                }

	                result.s = opts.width + 'x' + opts.height;
	            }

	            _underscore2.default.each(opts, function (value, key) {
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

	module.exports = exports['default'];

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
	    var args = optionsToArgs(options);

	    var defer = {};
	    defer.promise = new Promise(function (resolve, reject) {
	        return Object.assign(defer, { resolve: resolve, reject: reject });
	    });

	    _underscore2.default.each(infiles, function (opts, filename) {
	        if (!_underscore2.default.isEmpty(opts)) {
	            args.push.apply(args, optionsToArgs(opts));
	        }
	        args.push('-i', filename);
	    });

	    _underscore2.default.each(outfiles, function (opts, filename) {
	        if (!_underscore2.default.isEmpty(opts)) {
	            args.push.apply(args, optionsToArgs(opts));
	        }
	        args.push('-strict', 'experimental', filename);
	    });

	    // args.unshift('--adjustment=15', 'avconv') // '-n 15'
	    //args.unshift('avconv')
	    args.unshift('ffmpeg');
	    var _avconv = _child_process.child.spawn('nice', args);

	    _avconv.stdout.on('data', function (data) {
	        //console.log('stdout: ' + data);
	    });

	    _avconv.stderr.on('data', function (data) {
	        // var status = parseStatus('' + data);
	        //
	        // if (status) {
	        //     // cb(null, false, status);
	        // } else {
	        //     // console.log(""+data);
	        // }
	    });

	    _avconv.on('exit', function (code) {
	        if (code !== 0) {
	            defer.reject(new Error('Exited with error code:' + code));
	        } else {
	            defer.resolve(null, true);
	        }
	    });

	    return defer.promise;
	}

	function optionsToArgs(opts) {
	    var args = [];

	    if (opts) {
	        _underscore2.default.each(opts, function (value, key) {
	            args.push('-' + key);
	            if (value !== true) {
	                args.push(value);
	            }
	        });
	    }

	    return args;
	}

	var statusRegExp = /frame=\s*([0-9]+)\s*fps=\s*([0-9]+)\s*/;

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
	module.exports = exports['default'];

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
/* 6 */,
/* 7 */,
/* 8 */
/***/ function(module, exports) {

	module.exports = require("printf");

/***/ }
/******/ ]);