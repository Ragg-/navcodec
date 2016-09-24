#include "navstreamreader.h"

using namespace v8;

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

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

void NAVStreamReader::Next(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();

    NAVStreamReader* instance = ObjectWrap::Unwrap<NAVStreamReader>(args.This());

    AVFrame *frame = av_frame_alloc();
    if (! frame) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc frame.")));
    }

    int ret = av_image_alloc(frame->data, frame->linesize,
        instance->pContext->width, instance->pContext->height,
        instance->pContext->pix_fmt, 32);

    if (ret < 0) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to alloc image")));
    }

    int got_packet_ptr;
    AVPacket pkt;
    av_init_packet(&pkt);

    pkt.data = NULL;
    pkt.size = 0;

    ret = avcodec_encode_video2(instance->pContext, &pkt, frame, &got_packet_ptr);
    if (ret < 0) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Failed to encode")));
    }

    Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, pkt.size);

    // view->Set(Local<v8::Value> key, Local<v8::Value> value)
    int pkt_size = pkt.size;
    for (int idx = 0; idx < pkt_size; idx++) {
        buffer->Set(idx, Number::New(isolate, pkt.data[idx]));
    }

    av_packet_unref(&pkt);
    args.GetReturnValue().Set(buffer);
}
