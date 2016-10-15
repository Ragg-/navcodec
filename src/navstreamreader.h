#ifndef _NAVSTREAMREADER_H
#define _NAVSTREAMREADER_H

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>

using namespace v8;

extern "C" {
#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
}

class NAVStreamReader : public node::ObjectWrap {
private:
    const char * inputFilePath;
    uint32_t frameCursor;
    FILE* inputHandle;

    AVCodec *pCodec;
    AVCodecContext *pCodecCtx;
    AVFormatContext *pFormatCtx;

public:
    NAVStreamReader(const char *in_file);
    ~NAVStreamReader();

    static v8::Persistent<v8::Function> constructor;

    static void Init(const Local<Object> target);

    static void New(const FunctionCallbackInfo<Value> &args);
    static void Next(const FunctionCallbackInfo<Value> &args);
    static void SetMetadata(const FunctionCallbackInfo<Value> &args);
};
#endif
