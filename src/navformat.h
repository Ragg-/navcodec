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

#ifndef _NAVFORMAT_H
#define _NAVFORMAT_H

#include "navstream.h"

#include <v8.h>
#include <node.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

using namespace v8;

class NAVFormat : public node::ObjectWrap {
private:
  AVFormatContext *pFormatCtx;
  char *filename;
  Handle<Array> streams;

public:
  NAVFormat();
  ~NAVFormat();

  static Persistent<FunctionTemplate> constructor;

  static void Init(v8::Local<v8::Object> target);

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  // ([streams], cb(stream, frame))
  static void Version(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Decode(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Dump(const v8::FunctionCallbackInfo<v8::Value>& args);
};


//
// Decoder Notifier class. Used for clients notifying when they are done
// processing a decoded frame.
//
class DecoderNotifier : public node::ObjectWrap {

public:
  struct Baton *pBaton;
  DecoderNotifier(Baton *pBaton);
  ~DecoderNotifier();

  static Persistent<ObjectTemplate> constructor;

  static void Init();

  static void New(Baton *pBaton);
  static void Done(const v8::FunctionCallbackInfo<v8::Value>& args);
};

#endif // _NAVFORMAT_H
