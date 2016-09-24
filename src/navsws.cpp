/* Copyright(c) 2012 Optimal Bits Sweden AB. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include "navsws.h"

#include "navformat.h"
#include "navframe.h"
#include "navutils.h"

using namespace v8;

NAVSws::NAVSws(){
  pContext = NULL;
  pFrame = NULL;
  pFrameBuffer = NULL;
  passthrough = true;
}

NAVSws::~NAVSws(){
  printf("NAVSws destructor\n");
  sws_freeContext(pContext);
  av_free(pFrameBuffer);

  frame.Reset();
}

v8::Persistent<v8::Function> NAVSws::constructor;

void NAVSws::Init(Handle<Object> target){
  v8::Isolate *isolate = target->GetIsolate();

  // Our constructor
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);

  tpl->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
  tpl->SetClassName(String::NewFromUtf8(isolate, "NAVSws"));

  NODE_SET_PROTOTYPE_METHOD(tpl, "convert", Convert);

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "NAVSws"), tpl->GetFunction());
}

// (srcStream, dstStream | options)
void NAVSws::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();
  v8::Local<v8::Object> options;

  NAVSws* instance = new NAVSws();
  instance->Wrap(args.This());

  if(args.Length()<2){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Missing input parameters (srcStream, dstStream | options)")));
    return;
  }

  v8::Local<v8::Object> stream = v8::Local<v8::Object>::Cast(args[0]);
  AVStream *pSrcStream = (node::ObjectWrap::Unwrap<NAVStream>(stream))->pContext;

  v8::Local<v8::Object> streamOrOptions = v8::Local<v8::Object>::Cast(args[1]);
  v8::Local<v8::String> codecKey = v8::String::NewFromUtf8(isolate, "codec");
  if(streamOrOptions->Has(codecKey)){
    options = v8::Local<v8::Object>::Cast(streamOrOptions->Get(codecKey));
  }else{
    options = streamOrOptions;
  }

  instance->width = GET_OPTION_UINT32(options, width, 480);
  instance->height = GET_OPTION_UINT32(options, height, 480);
  instance->pix_fmt = (AVPixelFormat) GET_OPTION_UINT32(options, pix_fmt, AV_PIX_FMT_YUV420P);

  if( (pSrcStream->codec->width != instance->width) ||
      (pSrcStream->codec->height != instance->height) ||
      (pSrcStream->codec->pix_fmt != instance->pix_fmt) ){

    instance->pContext = sws_getContext(pSrcStream->codec->width,
                                        pSrcStream->codec->height,
                                        pSrcStream->codec->pix_fmt,
                                        instance->width,
                                        instance->height,
                                        instance->pix_fmt,
                                        SWS_BICUBIC, // -> put in options.
                                        NULL, NULL, NULL // Filters & Params (unused for now)
                                        );
    if(instance->pContext == NULL) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Could not init conversion context")));
      return;
    }

    int frameBufferSize;
    instance->pFrame = av_frame_alloc();

    if (!instance->pFrame){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Error Allocating AVFrame")));
      return;
    }

    frameBufferSize = avpicture_get_size(instance->pix_fmt, instance->width, instance->height);

    instance->pFrameBuffer = (uint8_t*) av_mallocz(frameBufferSize);
    if (!instance->pFrameBuffer ){
      av_frame_free(&instance->pFrame);
      return ThrowException(Exception::TypeError(String::New("Error Allocating AVFrame buffer")));
    }

    instance->frame.Reset(isolate, NAVFrame::New(isolate, instance->pFrame));
    instance->passthrough = false;
  }

  args.GetReturnValue().Set(args.This());
}

// ([streams], cb(stream, frame))
void NAVSws::Convert(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();
  Local<Object> srcFrame;

  if(args.Length()<1){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Missing input parameters (srcFrame)")));
    return;
  }
  srcFrame = Local<Array>::Cast(args[0]);

  NAVSws* instance = UNWRAP_OBJECT(NAVSws, args);

  // TODO: check that src frame is really compatible with this converter.

  if(instance->passthrough){
    args.GetReturnValue().Set(srcFrame);
    return;
  } else {
    AVFrame *pSrcFrame = (node::ObjectWrap::Unwrap<NAVFrame>(srcFrame))->pContext;

    instance->pFrame = av_frame_alloc();

    avpicture_fill((AVPicture *)instance->pFrame,
                    instance->pFrameBuffer,
                    instance->pix_fmt,
                    instance->width,
                    instance->height);

    instance->pFrame->width = instance->width;
    instance->pFrame->height = instance->height;

    int ret = sws_scale(instance->pContext,
                        pSrcFrame->data,
                        pSrcFrame->linesize,
                        0,
                        pSrcFrame->height,
                        instance->pFrame->data,
                        instance->pFrame->linesize);
    if(ret==0) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Failed frame conversion")));
      return;
    }

    instance->pFrame->pts = pSrcFrame->pts;

    args.GetReturnValue().Set(instance->frame);
  }
}
