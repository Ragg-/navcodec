#include "navstreamreader.h"
#include <stdio.h>

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
    pContext = NULL;
}

NAVStreamReader::~NAVStreamReader() {
    if (pContext) {
        avcodec_close(pContext);
        av_free(pContext);
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

    AVCodec *pCodec =  avcodec_find_encoder(AVCodecID::AV_CODEC_ID_APNG);
    if (! pCodec) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to initialize encoder.")));
        return;
    }

    instance->pContext = avcodec_alloc_context3(pCodec);
    if (! instance->pContext) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc encoder context.")));
        return;
    }

    instance->inputHandle = fopen(filename, "rb");
    if (instance->inputHandle == NULL) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open file.")));
    }
}

void NAVStreamReader::SetMetadata(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    NAVStreamReader *instance = ObjectWrap::Unwrap<NAVStreamReader>(args.Holder());

    Local<Object> options = args[0]->ToObject(isolate);

    instance->pContext->pix_fmt = AV_PIX_FMT_RGBA;
    instance->pContext->width = (int) options->Get(String::NewFromUtf8(isolate, "width"))->NumberValue();
    instance->pContext->height = (int) options->Get(String::NewFromUtf8(isolate, "height"))->NumberValue();
    int framerate = (int) options->Get(String::NewFromUtf8(isolate, "framerate"))->NumberValue();
    instance->pContext->time_base = (AVRational) {1, framerate};

    args.GetReturnValue().Set(true);
}

void NAVStreamReader::Next(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    NAVStreamReader* instance = ObjectWrap::Unwrap<NAVStreamReader>(args.Holder());

    if (avcodec_open2(instance->pContext, instance->pContext->codec, NULL) < 0) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to open codec")));
        return;
    }

    AVFrame *frame = av_frame_alloc();
    if (! frame) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc frame.")));
        return;
    }

    frame->format = instance->pContext->pix_fmt;
    frame->width = instance->pContext->width;
    frame->height = instance->pContext->height;
    frame->pts = 0;

    int ret = av_image_alloc(frame->data, frame->linesize,
        instance->pContext->width, instance->pContext->height,
        instance->pContext->pix_fmt, 32);

    if (ret < 0) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc image")));
        return;
    }

    int got_packet_ptr;
    AVPacket pkt;
    av_init_packet(&pkt);

    uint8_t inbuf[INBUF_SIZE +  AV_INPUT_BUFFER_PADDING_SIZE];

    for (;;) {
        pkt.size = fread(inbuf, 1, INBUF_SIZE, instance->inputHandle);

        if (pkt.size == 0) {
            break;
        }

        pkt.data = inbuf;
        while (pkt.size > 0) {
            int len = avcodec_decode_video2(instance->pContext, frame, &got_packet_ptr, &pkt);

            if (len < 0)  {
                isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "failed to decode video")));
                return;
            }

            if (got_packet_ptr) {

            }
        }
    }


    // for (int y = 0; y < instance->pContext->height; y++) {
    //     for (int x = 0; x < instance->pContext->width; x++) {
    //         frame->data[0][y * frame->linesize[0] + x] = x + y + 0 * 3;
    //     }
    // }
    // /* Cb and Cr */
    // for (int y = 0; y < instance->pContext->height/2; y++) {
    //     for (int x = 0; x < instance->pContext->width/2; x++) {
    //         frame->data[1][y * frame->linesize[1] + x] = 128 + y + 0 * 2;
    //         frame->data[2][y * frame->linesize[2] + x] = 64 + x + 0 * 5;
    //     }
    // }

    // ret = avcodec_decode_video2(instance->pContext, &pkt, &got_packet_ptr, frame);
    // if (ret < 0) {
    //     isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to encode")));
    // }

    Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, 1);
    // Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, pkt.data, pkt.size);

    // view->Set(Local<v8::Value> key, Local<v8::Value> value)
    // int pkt_size = pkt.size;
    // for (int idx = 0; idx < pkt_size; idx++) {
    //     buffer->Set(idx, Number::New(isolate, pkt.data[idx]));
    // }

    av_packet_unref(&pkt);
    args.GetReturnValue().Set(buffer);
    // args.GetReturnValue().Set(true);
}
