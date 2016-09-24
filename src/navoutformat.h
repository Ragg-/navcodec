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

#ifndef _NAV_OUTPUT_FORMAT_H
#define _NAV_OUTPUT_FORMAT_H

#include "navstream.h"
#include "navaudiofifo.h"

#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
}

using namespace v8;

class NAVOutputFormat : public node::ObjectWrap {
private:
  AVOutputFormat *pOutputFormat;

  char *filename;

  uint8_t *pVideoBuffer;
  int videoBufferSize;

  uint8_t *pAudioBuffer;
  int audioBufferSize;
  AVStream *pAudioStream;

  int64_t videoFrame;
  int32_t skipVideo;

public:
  NAVOutputFormat();
  ~NAVOutputFormat();

  // --
  AVFormatContext *pFormatCtx;
  NAVAudioFifo *pFifo;

  int outputAudio(AVFormatContext *pFormatContext,
                  AVStream *pStream,
                  AVFrame *pFrame);
  // --
  const char *EncodeVideoFrame(AVStream *pStream,
                               AVFrame *pFrame,
                               int *outSize);

  static Persistent<Function> constructor;

  static void Init(v8::Local<v8::Object> target);

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void New(AVFormatContext *pContext);

  // (stream_type::String, [codecId::String])
  static void AddStream(const v8::FunctionCallbackInfo<v8::Value>& args);

  // ()
  static void Begin(const v8::FunctionCallbackInfo<v8::Value>& args);

  // (stream::AVStream, frame::AVFrame)
  static void Encode(const v8::FunctionCallbackInfo<v8::Value>& args);

  // ()
  static void End(const v8::FunctionCallbackInfo<v8::Value>& args);
};

#endif // _NAV_OUTPUT_FORMAT_H
