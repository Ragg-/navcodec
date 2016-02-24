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

#include "navcodeccontext.h"
#include "navutils.h"

using namespace v8;

//void NoopSet(Local<String> property, Local<Value> value,
//             const v8::AccessorSetterCallback<v8::Value>& info) {};

AVCodecContext *GetCodecContext(const v8::PropertyCallbackInfo<v8::Value>& info){
  Local<Object> self = info.Holder();
  return node::ObjectWrap::Unwrap<NAVCodecContext>(self)->pContext;
}

#define GET_CODEC_CONTEXT(self)\
    (AVCodecContext*)(Local<External> wrap = Local<External>::Cast(self->GetInternalField(0))->Value())

Persistent<ObjectTemplate> NAVCodecContext::constructor;

NAVCodecContext::NAVCodecContext(AVCodecContext *pContext){
  this->pContext = pContext;
}

NAVCodecContext::~NAVCodecContext(){
  fprintf(stderr, "NAVCodecContext destructor\n");
//  avcodec_close(pContext);
}

void NAVCodecContext::Init(v8::Isolate *isolate) {
  v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New(isolate);
  tpl->SetInternalFieldCount(1);

  // Accessors make us simulate read-only properties.
  tpl->SetAccessor(String::NewFromUtf8(isolate, "codec_type"), GetType, 0);
  tpl->SetAccessor(String::NewFromUtf8(isolate, "width"), GetWidth, 0);
  tpl->SetAccessor(String::NewFromUtf8(isolate, "height"), GetHeight, 0);
  tpl->SetAccessor(String::NewFromUtf8(isolate, "bit_rate"), GetBitRate, 0);
  tpl->SetAccessor(String::NewFromUtf8(isolate, "sample_fmt"), GetSampleFmt, 0);
  tpl->SetAccessor(String::NewFromUtf8(isolate, "sample_rate"), GetSampleRate, 0);
  tpl->SetAccessor(String::NewFromUtf8(isolate, "channels"), GetChannels, 0);
  tpl->SetAccessor(String::NewFromUtf8(isolate, "framerate"), GetFramerate, 0);

//  tpl->SetAccessor(String::NewFromUtf8(isolate, "pix_fmt"), GetPixFmt, NoopSet);
//  tpl->SetAccessor(String::NewFromUtf8(isolate, "id"), GetCodecId, NoopSet);

  constructor.Reset(isolate, tpl);
}

v8::Handle<v8::Value> NAVCodecContext::New(v8::Isolate *isolate, AVCodecContext *pContext) {
  //v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(isolate);

  v8::Local<v8::ObjectTemplate> cons = v8::Local<v8::ObjectTemplate>::New(isolate, constructor);
  v8::Local<v8::Object> obj = cons->NewInstance();
  NAVCodecContext *instance = new NAVCodecContext(pContext);
  instance->Wrap(obj);

  NODE_SET_METHOD(obj, "open", Open);

  v8::Local<v8::Object> timeBase = v8::Object::New(isolate);

  SET_KEY_VALUE(timeBase, "num", v8::Integer::New(isolate, pContext->time_base.num));
  SET_KEY_VALUE(timeBase, "den", v8::Integer::New(isolate, pContext->time_base.den));

  SET_KEY_VALUE(obj, "time_base", timeBase);
  SET_KEY_VALUE(obj, "ticks_per_frame", v8::Integer::New(isolate, pContext->ticks_per_frame));
  SET_KEY_VALUE(obj, "pix_fmt", v8::Integer::New(isolate, pContext->pix_fmt));

  return scope.Escape(obj);
}

void NAVCodecContext::Open(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();
  Local<Object> self = args.This();

  AVCodecContext *pCodecCtx = node::ObjectWrap::Unwrap<NAVCodecContext>(self)->pContext;

  AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL) {
    // Throw error!
  }

  if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
    // Throw error: Could not open codec
  }

  args.GetReturnValue().Set(Undefined(isolate));
}

void NAVCodecContext::GetType(Local<String> property,
                               const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Isolate *isolate = info.GetIsolate();

  AVCodecContext *pCodecCtx = GetCodecContext(info);

  v8::Local<v8::String> codecType;

  switch(pCodecCtx->codec_type){
    case AVMEDIA_TYPE_UNKNOWN: codecType = String::NewFromUtf8(isolate, "Unknown");break;
    case AVMEDIA_TYPE_VIDEO: codecType = String::NewFromUtf8(isolate, "Video");break;
    case AVMEDIA_TYPE_AUDIO: codecType = String::NewFromUtf8(isolate, "Audio");break;
    case AVMEDIA_TYPE_DATA: codecType = String::NewFromUtf8(isolate, "Data");break;
    case AVMEDIA_TYPE_SUBTITLE: codecType = String::NewFromUtf8(isolate, "Subtitle");break;
    case AVMEDIA_TYPE_ATTACHMENT: codecType = String::NewFromUtf8(isolate, "Attachment");break;
    case AVMEDIA_TYPE_NB: codecType = String::NewFromUtf8(isolate, "NB");break;
  }

  info.GetReturnValue().Set(codecType);
}

void NAVCodecContext::GetWidth(Local<String> property,
                                const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Isolate *isolate = info.GetIsolate();
  AVCodecContext *pCodecCtx = GetCodecContext(info);
  Handle<Integer> width = Integer::New(isolate, pCodecCtx->width);
  info.GetReturnValue().Set(width);
}

void NAVCodecContext::GetHeight(Local<String> property,
                                 const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Isolate *isolate = info.GetIsolate();
  AVCodecContext *pCodecCtx = GetCodecContext(info);
  Handle<Integer> height = Integer::New(isolate, pCodecCtx->height);
  info.GetReturnValue().Set(height);
}

void NAVCodecContext::GetBitRate(Local<String> property,
                                         const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Isolate *isolate = info.GetIsolate();
  AVCodecContext *pCodecCtx = GetCodecContext(info);
  Handle<Integer> bit_rate = Integer::New(isolate, pCodecCtx->bit_rate);
  info.GetReturnValue().Set(bit_rate);
}

void NAVCodecContext::GetSampleFmt(Local<String> property,
                                          const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Isolate *isolate = info.GetIsolate();
  AVCodecContext *pCodecCtx = GetCodecContext(info);
  Handle<Integer> sample_fmt = Integer::New(isolate, pCodecCtx->sample_fmt);
  info.GetReturnValue().Set(sample_fmt);
}
void NAVCodecContext::GetSampleRate(Local<String> property,
                                             const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Isolate *isolate = info.GetIsolate();
  AVCodecContext *pCodecCtx = GetCodecContext(info);
  Handle<Integer> sample_rate = Integer::New(isolate, pCodecCtx->sample_rate);
  info.GetReturnValue().Set(sample_rate);
}
void NAVCodecContext::GetChannels(Local<String> property,
                                          const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Isolate *isolate = info.GetIsolate();
  AVCodecContext *pCodecCtx = GetCodecContext(info);
  Handle<Integer> channels = Integer::New(isolate, pCodecCtx->channels);
  info.GetReturnValue().Set(channels);
}
void NAVCodecContext::GetFramerate(Local<String> property,
                                         const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::Isolate *isolate = info.GetIsolate();
  AVCodecContext *pCodecCtx = GetCodecContext(info);

  float framerate = 0.0;
  if (pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
    framerate = (float) pCodecCtx->time_base.den /
      (float) (pCodecCtx->time_base.num * pCodecCtx->ticks_per_frame);
  }

  Handle<Number> val = Number::New(isolate, framerate);
  info.GetReturnValue().Set(val);
}
