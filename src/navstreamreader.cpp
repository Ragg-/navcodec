#include "navstreamreader.h"
#include <iostream>

using namespace v8;

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

#define INBUF_SIZE 4096

NAVStreamReader::NAVStreamReader(const char *inputFilePath) {
    this->inputFilePath = inputFilePath;
    frameCursor = 0;
    inputHandle = NULL;
    pCodec = NULL;
    pCodecCtx = NULL;
    pFormatCtx = NULL;
}

NAVStreamReader::~NAVStreamReader() {
    if (this->pCodecCtx) {
        avcodec_close(this->pCodecCtx);
        av_free(this->pCodecCtx);
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

    if (avformat_open_input(&instance->pFormatCtx, instance->inputFilePath, NULL, NULL) != 0) {
        std::cout << "failed to open format" << std::endl;
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "failed to open format")));
        return;
    }

    instance->pCodecCtx = instance->pFormatCtx->streams[0]->codec;
    instance->pCodec = avcodec_find_decoder(instance->pCodecCtx->codec_id);
    if (! instance->pCodec) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to initialize encoder.")));
        return;
    }

    // instance->pCodecCtx = avcodec_alloc_context3(instance->pCodec);
    // if (! instance->pCodecCtx) {
    //     isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc encoder context.")));
    //     return;
    // }

    if (avcodec_open2(instance->pCodecCtx, instance->pCodec, NULL) < 0) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open codec.")));
        return;
    }

    instance->inputHandle = fopen(filename, "rb");
    if (instance->inputHandle == NULL) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open file.")));
    }

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
    char script[10000];
    sprintf(script, "console.log('%s');", message);
    Script::Compile(String::NewFromUtf8(isolate, script))->Run();
}

void NAVStreamReader::SetMetadata(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    NAVStreamReader *instance = ObjectWrap::Unwrap<NAVStreamReader>(args.Holder());

    Local<Object> options = args[0]->ToObject(isolate);

    console_log_json(isolate, "meta", options);

    // instance->pCodecCtx->pix_fmt = AV_PIX_FMT_RGBA;
    // instance->pCodecCtx->width = (int) options->Get(String::NewFromUtf8(isolate, "width"))->NumberValue();
    // instance->pCodecCtx->height = (int) options->Get(String::NewFromUtf8(isolate, "height"))->NumberValue();
    // int framerate = (int) options->Get(String::NewFromUtf8(isolate, "framerate"))->NumberValue();
    // instance->pCodecCtx->time_base = (AVRational) {1, framerate};

    args.GetReturnValue().Set(true);
}

void NAVStreamReader::Next(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    char script[10000];

    NAVStreamReader* instance = ObjectWrap::Unwrap<NAVStreamReader>(args.Holder());
    console_log_json(isolate, "me", args.This());

    if (avcodec_open2(instance->pCodecCtx, instance->pCodecCtx->codec, NULL) < 0) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open codec")));
        return;
    } else {
        std::cout << "Open codec" << std::endl;
    }

    AVFrame *frame = av_frame_alloc();
    if (! frame) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc frame.")));
        return;
    } else {
        std::cout << "Frame allocated" << std::endl;
    }

    // frame->format = instance->pCodecCtx->pix_fmt;
    // frame->width = instance->pCodecCtx->width;
    // frame->height = instance->pCodecCtx->height;
    // frame->pts = 0;

    // std::cout << "Width: " << std::dec << instance->pCodecCtx->width << std::endl;
    // std::cout << "Height: " << std::dec << instance->pCodecCtx->height << std::endl;

    // int ret = av_image_alloc(frame->data, frame->linesize,
    //     instance->pCodecCtx->width, instance->pCodecCtx->height,
    //     instance->pCodecCtx->pix_fmt, 32);

    // if (ret < 0) {
    //     isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc image")));
    //     return;
    // }

    int frame_count = 0;
    int got_frame = 0;
    long len = 0;

    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    std::cout << "Memset" << std::endl;

    AVPacket avpkt;
    std::cout << "Packet allocate" << std::endl;
    av_init_packet(&avpkt);

    std::cout << "Packet allocated" << std::endl;

    if (instance->pCodecCtx) {
        std::cout << "Context exists." << std::endl;
    } else {
        std::cout << "Context no exists." << std::endl;
    }

    // for (;;) {
        // std::cout << "Before read" << std::endl;
        // avpkt.size = fread(inbuf, 1, INBUF_SIZE, instance->inputHandle);
        // std::cout << "Read " << std::dec << avpkt.size << " bytes." << std::endl;
        // std::cout << "File handle: " << (instance->inputHandle == NULL ? "null" : "set") << std::endl;

        // if (avpkt.size == 0) {
        //     break;
        // }

        // avcodec_find_decoder(this->pFormatCtx->codec_id);

        // avpkt.data = inbuf;
        // while (avpkt.size > 0) {
        while (av_read_frame(instance->pFormatCtx, &avpkt) >= 0) {
            // std::cout << "Decoding..." << std::endl;

            // for (int buf_idx = 0; buf_idx < avpkt.size; buf_idx++) {
            //     std::cout << std::hex << (int) avpkt.data[buf_idx] << " ";
            // }
            //
            // std::cout << std::endl;

            // std::cout << "    start" << std::endl;
            len = avcodec_decode_video2(instance->pCodecCtx, frame, &got_frame, &avpkt);
            // std::cout << "Decoded: " << std::dec << len << " bytes." << std::endl;

            // if (len < 0)  {
            //      if (!instance->pCodecCtx->codec) {
            //         isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Codec not opened, or it is an encoder.")));
            //     } else if (instance->pCodecCtx->codec->type != AVMEDIA_TYPE_VIDEO) {
            //         isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Invalid media type for video")));
            //     } else {
            //         isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "failed to decode video")));
            //     }
            //
            //     return;
            // }

            if (got_frame) {
                frame_count++;
                std::cout << "got frame" << std::endl;

                int size = frame->width * frame->height * 3;
                std::cout << std::dec << frame->width << " x " << std::dec << frame->height << " * " << 3 << std::endl;
                std::cout << "sizeof: " << std::dec << sizeof(frame->data) << std::endl;
                Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, frame->data, size);
                args.GetReturnValue().Set(buffer);
                break;
            }
        }
    // }

    avpkt.data = NULL;
    avpkt.size = 0;
    av_packet_unref(&avpkt);

    // args.GetReturnValue().Set(true);
    // args.GetReturnValue().Set(Integer::New(isolate, (int32_t) (int8_t) *frame->data[0]));
}
