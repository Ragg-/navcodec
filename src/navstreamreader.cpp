#include "navstreamreader.h"
#include <iostream>
#include <stdio.h>

using namespace v8;

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

#define INBUF_SIZE 4096

NAVStreamReader::NAVStreamReader(const char *inputFilePath) {
    this->inputFilePath = inputFilePath;
    this->frameCursor = 0;

    this->pFormatCtx = NULL;

    this->videoStreamIdx = -1;
    this->audioStreamIdx = -1;
    this->pVideoStream = NULL;
    this->pAudioStream = NULL;

    // this->inputHandle = NULL;
    this->pVideoCodec = NULL;
    this->pVideoCodecCtx = NULL;
    this->pAudioCodec = NULL;
    this->pAudioCodecCtx = NULL;
}

NAVStreamReader::~NAVStreamReader() {
    if (this->pVideoCodecCtx) {
        avcodec_close(this->pVideoCodecCtx);
    }

    if (this->pAudioCodecCtx) {
        avcodec_close(this->pAudioCodecCtx);
    }

    if (this->pFormatCtx) {
        avformat_close_input(& this->pFormatCtx);
    }
}

Persistent<Function> NAVStreamReader::constructor;

void NAVStreamReader::Init(const Local<Object> target) {
    Isolate *isolate = target->GetIsolate();

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, NAVStreamReader::New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "NAVStreamReader"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "next", NAVStreamReader::Next);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setMetaData", NAVStreamReader::SetMetadata);

    constructor.Reset(isolate, tpl->GetFunction());
    target->Set(String::NewFromUtf8(isolate, "NAVStreamReader"), tpl->GetFunction());
}

void NAVStreamReader::New(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "input file path must be specified")));
    }

    String::Utf8Value _v8FileName(args[0]);
    char *filename = *_v8FileName;

    NAVStreamReader *instance = new NAVStreamReader(filename);
    instance->Wrap(args.This());

    std::cout << "constructing...: " << instance->inputFilePath << std::endl;
    if (avformat_open_input(&instance->pFormatCtx, instance->inputFilePath, NULL, NULL) != 0) {
        std::cout << "failed to open format" << std::endl;
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "failed to open format")));
        return;
    }

    for (int streamIdx = 0; streamIdx < instance->pFormatCtx->nb_streams; streamIdx++) {
        //
        // Found first video stream
        //
        if (
            instance->videoStreamIdx == -1
            && instance->pFormatCtx->streams[streamIdx]->codec->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO
        ) {
            std::cout << "Found video stream :" << std::dec << streamIdx << std::endl;
            instance->videoStreamIdx = streamIdx;
            instance->pVideoStream = instance->pFormatCtx->streams[streamIdx];
            instance->pVideoCodecCtx = instance->pFormatCtx->streams[streamIdx]->codec;

            AVCodec *pVideoCodec = avcodec_find_decoder(instance->pVideoCodecCtx->codec_id);
            if (!pVideoCodec) {
                isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to find video encoder.")));
                return;
            }

            if (avcodec_open2(instance->pVideoCodecCtx, pVideoCodec, NULL) < 0) {
                isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open video codec.")));
                return;
            }

            continue;
        }

        //
        // Found first audio stream
        //
        if (
            instance->audioStreamIdx == -1
            && instance->pFormatCtx->streams[streamIdx]->codec->codec_type == AVMediaType::AVMEDIA_TYPE_AUDIO
        ) {
            std::cout << "Found audio stream :" << std::dec << streamIdx << std::endl;
            instance->audioStreamIdx = streamIdx;
            instance->pAudioStream = instance->pFormatCtx->streams[streamIdx];
            instance->pAudioCodecCtx = instance->pFormatCtx->streams[streamIdx]->codec;

            if (! instance->pAudioCodecCtx) {
                isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to find audio encoder.")));
                return;
            }

            AVCodec *pAudioCodec = avcodec_find_decoder(instance->pAudioCodecCtx->codec_id);
            if (!pAudioCodec) {
                isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to find video encoder.")));
                return;
            }

            if (avcodec_open2(instance->pAudioCodecCtx, pAudioCodec, NULL) < 0) {
                isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open audio codec.")));
                return;
            }
            continue;
        }
    }

    // printf("format:%d , RGBA:%d\n", instance->pFormatCtx->iformat->raw_codec_id, AVPixelFormat::AV_PIX_FMT_RGBA);

    // instance->pVideoCodecCtx = avcodec_alloc_context3(instance->pVideoCodec);
    // if (! instance->pVideoCodecCtx) {
    //     isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc encoder context.")));
    //     return;
    // }

    // if (instance->pAudioCodecCtx != NULL && avcodec_open2(instance->pAudioCodecCtx, instance->pAudioCodec, NULL) < 0) {
    //     isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open codec.")));
    //     return;
    // }

    //
    // Open file
    //
    // instance->inputHandle = fopen(filename, "rb");
    // if (instance->inputHandle == NULL) {
    //     isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open file.")));
    // }

    args.GetReturnValue().Set(args.This());
}

void console_log_json(Isolate* isolate, const char* message, v8::Local<v8::Value> target) {
    v8::EscapableHandleScope scope(isolate);

    Local<Object> JSON = isolate->GetCurrentContext()->Global()->Get(String::NewFromUtf8(isolate, "JSON"))->ToObject();
    Local<Function> stringify = Local<Function>::Cast(JSON->Get(String::NewFromUtf8(isolate, "stringify")));
    Local<v8::Value> args[1] = {target};
    Local<Value> value = stringify->Call(JSON, 1, args);

    char script[10000];
    sprintf(script, "console.log('%s', %s);", message, *String::Utf8Value(value->ToString(isolate)));
    Script::Compile(String::NewFromUtf8(isolate, script))->Run();
    scope.Escape(Null(isolate));
}

void console_log(Isolate* isolate, const char* message) {
    char script[2000];
    sprintf(script, "console.log('%s');", message);
    Script::Compile(String::NewFromUtf8(isolate, script))->Run();
}

void NAVStreamReader::SetMetadata(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    NAVStreamReader *instance = ObjectWrap::Unwrap<NAVStreamReader>(args.Holder());

    Local<Object> options = args[0]->ToObject(isolate);
    // console_log_json(isolate, "meta", options);

    // instance->pVideoCodecCtx->pix_fmt = AV_PIX_FMT_RGBA;
    // instance->pVideoCodecCtx->width = (int) options->Get(String::NewFromUtf8(isolate, "width"))->NumberValue();
    // instance->pVideoCodecCtx->height = (int) options->Get(String::NewFromUtf8(isolate, "height"))->NumberValue();
    // int framerate = (int) options->Get(String::NewFromUtf8(isolate, "framerate"))->NumberValue();
    // instance->pVideoCodecCtx->time_base = (AVRational) {1, framerate};

    args.GetReturnValue().Set(true);
}

void NAVStreamReader::Next(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    NAVStreamReader* instance = ObjectWrap::Unwrap<NAVStreamReader>(args.Holder());
    // console_log_json(isolate, "me", args.This());

    // if (avcodec_open2(instance->pVideoCodecCtx, instance->pVideoCodecCtx->codec, NULL) < 0) {
    //     isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open codec")));
    //     return;
    // }

    int frame_count = 0;
    // char script[1000];

    AVFrame *pVideoInputFrame = av_frame_alloc();
    AVFrame *pAudioInputFrame = av_frame_alloc();
    AVFrame *pExportFrame = av_frame_alloc();

    AVPacket avImagePkt;
    AVPacket avAudioPkt;
    av_init_packet(&avImagePkt);
    av_init_packet(&avAudioPkt);

    unsigned char* videoBuffer = (unsigned char *) av_malloc(
        av_image_get_buffer_size(
            instance->pVideoCodecCtx->pix_fmt == -1 ? AV_PIX_FMT_YUV420P : instance->pVideoCodecCtx->pix_fmt,
            instance->pVideoCodecCtx->width,
            instance->pVideoCodecCtx->height,
            1 // via avpicture_get_size implementation
        )
    );

    av_image_fill_arrays(
        pExportFrame->data,
        pExportFrame->linesize,
        videoBuffer,
        AVPixelFormat::AV_PIX_FMT_RGBA,
        instance->pVideoCodecCtx->width,
        instance->pVideoCodecCtx->height,
        1 // via avpicture_fill implementation
    );

    // sprintf(script, "Channels: %d", pExportFrame->channels);
    // console_log(isolate, script);

    // std::cout << "fill_arrays" << std::endl;
    //
    // std::cout << "fill_arrays" << std::endl;

    SwsContext* pSWSCtxForExport = sws_getContext(
        instance->pVideoCodecCtx->width,
        instance->pVideoCodecCtx->height,
        instance->pVideoCodecCtx->pix_fmt == -1 ? AV_PIX_FMT_YUV420P : instance->pVideoCodecCtx->pix_fmt,
        instance->pVideoCodecCtx->width,
        instance->pVideoCodecCtx->height,
        AVPixelFormat::AV_PIX_FMT_RGBA,
        SWS_FAST_BILINEAR,
        NULL,
        NULL,
        NULL
    );

    // std::cout << "getContext" << std::endl;

    int imageLen = 0;
    int audioLen = 0;
    int got_video_frame = 0;
    int got_audio_frame = 0;

    Local<ArrayBuffer> imageBuffer;
    Local<ArrayBuffer> audioBuffer;
    Local<Array> returns = Array::New(isolate);

    // std::cout << "Before read video frame" << std::endl;
    while (av_read_frame(instance->pFormatCtx, &avImagePkt) >= 0) {
        // std::cout << "After read video frame" << std::endl;

        if (avImagePkt.stream_index == instance->videoStreamIdx) {
            imageLen = avcodec_decode_video2(instance->pVideoCodecCtx, pVideoInputFrame, &got_video_frame, &avImagePkt);
            std::cout << "Video decoded" << std::endl;
        }

        if (avImagePkt.stream_index == instance->audioStreamIdx) {
            std::cout << "Audio decoding" << std::endl;

            if (!instance->pAudioCodecCtx) {
                std::cout << "missing AudioCodecCtx" << std::endl;
            }

            if (avAudioPkt.size >= 0) {
                std::cout << "missing AudioPacket" << std::endl;
            }

            audioLen = avcodec_decode_audio4(instance->pAudioCodecCtx, NULL, &got_audio_frame, &avAudioPkt);
            std::cout << "Audio decoded" << std::endl;
        }

        // if (got_video_frame) {
        // //     frame_count++;
        // //     std::cout << "got frame" << std::endl;
        // //
        //     int imageSizeBytes = instance->pVideoCodecCtx->width * instance->pVideoCodecCtx->height * 4; // RGBA 4bytes
        // //     int audioSizeBytes = av_samples_get_buffer_size(
        // //         NULL,
        // //         instance->pVideoCodecCtx->channels,
        // //         pAudioInputFrame->nb_samples,
        // //         instance->pVideoCodecCtx->sample_fmt,
        // //         1
        // //     );
        // //
        //     sws_scale(
        //         pSWSCtxForExport,
        //         pVideoInputFrame->data,
        //         pVideoInputFrame->linesize,
        //         0,
        //         instance->pVideoCodecCtx->height,
        //         pExportFrame->data,
        //         pExportFrame->linesize
        //     );
        // //
        // //     // std::cout << std::dec << pVideoInputFrame->width << " x " << std::dec << pVideoInputFrame->height << " * " << 3 << std::endl;
        // //     // std::cout << "sizeof: " << std::dec << sizeof(pVideoInputFrame->data) << std::endl;
        // //
        // //     // console_log(isolate, "\n");
        // //     // for (int _idx = 0; _idx < imageSizeBytes; _idx++) {
        // //     //     // sprintf(script, "%02x ", pExportFrame->data[0][_idx]);
        // //     //     // console_log(isolate, script);
        // //     // }
        // //
        //     imageBuffer = ArrayBuffer::New(isolate, pExportFrame->data[0], imageSizeBytes);
        // //     audioBuffer = ArrayBuffer::New(isolate, pAudioInputFrame->data[0], audioSizeBytes);
        //     returns->Set(0, imageBuffer);
        // //     returns->Set(1, audioBuffer);
        //     args.GetReturnValue().Set(returns);
        //     break;
        // }
    }

    avImagePkt.data = NULL;
    avImagePkt.size = 0;
    avAudioPkt.data = NULL;
    avAudioPkt.size = 0;

    av_packet_unref(&avImagePkt);
    av_packet_unref(&avAudioPkt);

    sws_freeContext(pSWSCtxForExport);

    av_free(videoBuffer);
    av_free(pVideoInputFrame);
    av_free(pAudioInputFrame);
    // av_free(pExportFrame);

    // args.GetReturnValue().Set(true);
    // args.GetReturnValue().Set(Integer::New(isolate, (int32_t) (int8_t) *frame->data[0]));
}
