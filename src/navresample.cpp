/* Copyright(c) 2012-2013 Optimal Bits Sweden AB. All rights reserved.
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
#include "navresample.h"

#include "navformat.h"
#include "navframe.h"
#include "navutils.h"

using namespace v8;

// Cheap layout guessing
int numChannesToLayout(int numChannels){
  switch(numChannels){
    case 1: return AV_CH_LAYOUT_MONO;
    case 2: return AV_CH_LAYOUT_STEREO;
    case 3: return AV_CH_LAYOUT_2POINT1;
    case 4: return AV_CH_LAYOUT_3POINT1;
    case 5: return AV_CH_LAYOUT_4POINT1;
    case 6: return AV_CH_LAYOUT_5POINT1;
    case 7: return AV_CH_LAYOUT_6POINT1;
    case 8: return AV_CH_LAYOUT_7POINT1;
  }
  return AV_CH_LAYOUT_STEREO;
}

NAVResample::NAVResample(){
  pContext = NULL;
  pFrame = NULL;
  pAudioBuffer = NULL;
  passthrough = true;
}

NAVResample::~NAVResample(){
  fprintf(stderr, "NAVResample destructor\n");

  avresample_free(&pContext);

  frame.Reset();
}

v8::Persistent<v8::Function> NAVResample::constructor;

void NAVResample::Init(v8::Local<v8::Object> target) {
  v8::Isolate* isolate = target->GetIsolate();

  // Our constructor
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "NAVResample"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function

  NODE_SET_PROTOTYPE_METHOD(tpl, "convert", Convert);

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "NAVResample"), tpl->GetFunction());
}

// (srcStream, dstStream, [options])
void NAVResample::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Object> self = args.This();

  NAVResample* instance = new NAVResample();
  instance->Wrap(self);

  if (args.Length() < 2) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Missing input parameters (srcStream, dstStream)")));
    return;
  }

  Local<Object> stream = Local<Object>::Cast(args[0]);
  AVStream *pSrcStream = (node::ObjectWrap::Unwrap<NAVStream>(stream))->pContext;

  stream = Local<Object>::Cast(args[1]);
  AVStream *pDstStream = (node::ObjectWrap::Unwrap<NAVStream>(stream))->pContext;

  instance->pSrcStream = pSrcStream;
  instance->pDstStream = pDstStream;

  AVCodecContext *pSrcCodec = pSrcStream->codec;
  AVCodecContext *pDstCodec = pDstStream->codec;

  if((pSrcCodec->channels != pDstCodec->channels) ||
     (pSrcCodec->sample_rate != pDstCodec->sample_rate) ||
     (pSrcCodec->sample_fmt != pDstCodec->sample_fmt) || // Sample format is irrelevant or not?
     (pSrcCodec->bit_rate > pDstCodec->bit_rate)){

    instance->pContext = avresample_alloc_context();
    if(instance->pContext == NULL) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Could not init resample context")));
      return;
    }

    {
      AVAudioResampleContext *avr  = instance->pContext;

      // TODO: Decide Channel layout based on input and output number of channels.
      av_opt_set_int(avr, "in_channel_layout",  numChannesToLayout(pSrcCodec->channels), 0);
      av_opt_set_int(avr, "out_channel_layout", numChannesToLayout(pDstCodec->channels), 0);

      av_opt_set_int(avr, "in_sample_rate", pSrcCodec->sample_rate, 0);
      av_opt_set_int(avr, "in_sample_fmt",  pSrcCodec->sample_fmt, 0);

      av_opt_set_int(avr, "out_sample_rate", pDstCodec->sample_rate, 0);
      av_opt_set_int(avr, "out_sample_fmt", pDstCodec->sample_fmt, 0);

      if(avresample_open(instance->pContext)){
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Could not open resampler")));
        return;
      }
    }

    instance->pFrame = av_frame_alloc();
    if (!instance->pFrame){
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Error Allocating AVFrame")));
      return;
    }

    instance->frame.Reset(isolate, NAVFrame::New(isolate, instance->pFrame));
    instance->passthrough = false;
  }

  args.GetReturnValue().Set(self);
}

// (frame: AVFrame) -> AVFrame;
void NAVResample::Convert(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  Local<Object> srcFrame;
  Handle<Object> dstFrame;

  if(args.Length()<1){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Missing input parameters (srcFrame)")));
    return;
  }
  srcFrame = Local<Array>::Cast(args[0]);

  NAVResample* instance = UNWRAP_OBJECT(NAVResample, args);

  if(instance->passthrough){
    args.GetReturnValue().Set(srcFrame);
  } else {
    AVFrame *pSrcFrame = (node::ObjectWrap::Unwrap<NAVFrame>(srcFrame))->pContext;

    AVCodecContext *pCodecContext = instance->pDstStream->codec;

    instance->pFrame = av_frame_alloc();

    instance->pFrame->quality = 1;
    instance->pFrame->pts = pSrcFrame->pts;
    instance->pFrame->format = pCodecContext->sample_fmt;

    {
      AVAudioResampleContext *avr  = instance->pContext;

      uint8_t *output;
      int out_linesize;

      int nb_samples = avresample_available(avr) +
                       av_rescale_rnd(avresample_get_delay(avr) +
                                      pSrcFrame->nb_samples,
                                      pCodecContext->sample_rate,
                                      instance->pSrcStream->codec->sample_rate,
                                      AV_ROUND_UP);

      av_samples_alloc(&output,
                       &out_linesize,
                       pCodecContext->channels,
                       nb_samples,
                       pCodecContext->sample_fmt,
                       1);
      if(output == NULL) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Failed allocating output samples buffer")));
        return;
      }

      nb_samples = avresample_convert(avr,
                                      &output,
                                      out_linesize,
                                      nb_samples,
                                      pSrcFrame->data,
                                      pSrcFrame->linesize[0],
                                      pSrcFrame->nb_samples);
      int size = nb_samples *
                 pCodecContext->channels *
                 av_get_bytes_per_sample(pCodecContext->sample_fmt);

      instance->pFrame->nb_samples = nb_samples;

      int ret = avcodec_fill_audio_frame(instance->pFrame,
                                         pCodecContext->channels,
                                         pCodecContext->sample_fmt,
                                         output,
                                         size,
                                         1);

      if(ret<0) {
        fprintf(stderr, "avcodec_fill_audio_frame returned %i\n", ret);
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Failed filling frame")));
        return;
      }
      av_freep(&output);
    }

    return instance->frame;
  }
}

// Available layouts:
/*
   AV_CH_LAYOUT_5POINT1
   AV_CH_LAYOUT_2_1
   AV_CH_LAYOUT_2_2
   AV_CH_LAYOUT_2POINT1
   AV_CH_LAYOUT_3POINT1
   AV_CH_LAYOUT_4POINT0
   AV_CH_LAYOUT_4POINT1
   AV_CH_LAYOUT_5POINT0
   AV_CH_LAYOUT_5POINT0_BACK
   AV_CH_LAYOUT_5POINT1
   AV_CH_LAYOUT_5POINT1_BACK
   AV_CH_LAYOUT_6POINT0
   AV_CH_LAYOUT_6POINT0_FRONT
   AV_CH_LAYOUT_6POINT1
   AV_CH_LAYOUT_6POINT1_BACK
   AV_CH_LAYOUT_6POINT1_FRONT
   AV_CH_LAYOUT_7POINT0
   AV_CH_LAYOUT_7POINT0_FRONT
   AV_CH_LAYOUT_7POINT1
   AV_CH_LAYOUT_7POINT1_WIDE
   AV_CH_LAYOUT_7POINT1_WIDE_BACK
   AV_CH_LAYOUT_HEXAGONAL
   AV_CH_LAYOUT_MONO
   AV_CH_LAYOUT_OCTAGONAL
   AV_CH_LAYOUT_QUAD
   AV_CH_LAYOUT_STEREO
   AV_CH_LAYOUT_STEREO_DOWNMIX
   AV_CH_LAYOUT_SURROUND
*/
