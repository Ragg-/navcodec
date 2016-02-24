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

#include "navstream.h"
#include "navcodeccontext.h"
#include "navutils.h"
#include "navdictionary.h"

using namespace v8;

v8::Persistent<v8::ObjectTemplate> NAVStream::constructor;

NAVStream::NAVStream(AVStream *pStream) {
  this->pContext = pStream;
}
NAVStream::~NAVStream() {
  // TODO: clean ups?
  fprintf(stderr, "NAVStream destroyed\n");
}

void NAVStream::Init(v8::Isolate *isolate) {
  v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New(isolate);
  tpl->SetInternalFieldCount(1);

  constructor.Reset(isolate, tpl);
}

void NAVStream::New(AVStream *pStream) {
  NAVStream *instance = new NAVStream(pStream);

  v8::Local<v8::Object> obj = constructor->NewInstance();
  instance->Wrap(obj);

  Handle<Value> codec = NAVCodecContext::New(pStream->codec);

  SET_KEY_VALUE(obj, "codec", codec);

  float duration = pStream->duration*pStream->time_base.num/
                   (float)pStream->time_base.den;

  duration = duration < 0? -1 : duration;

  SET_KEY_VALUE(obj, "duration", Number::New(duration));
  SET_KEY_VALUE(obj, "metadata", NAVDictionary::New(pStream->metadata));
  SET_KEY_VALUE(obj, "numFrames", Integer::New(pStream->nb_frames));

#if LIBAVFORMAT_VERSION_INT < AV_VERSION_INT(52, 42, 0)
  AVRational fr = pStream->r_frame_rate;
#else
  AVRational fr = pStream->avg_frame_rate;
#endif

  if(fr.num){
    Local<Object> frameRate = Object::New();

    SET_KEY_VALUE(frameRate, "num", Integer::New(fr.num));
    SET_KEY_VALUE(frameRate, "den", Integer::New(fr.den));

    SET_KEY_VALUE(obj, "frameRate", frameRate);
  }

  info.GetReturnValue().Set(obj);
}
