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

#include "navcodec.h"

#include <nan.h>
#include <v8.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

//using namespace v8;

// extern "C" { // Cause of name mangling in C++, we use extern C here
NAN_MODULE_INIT(init) {
    Nan::HandleScope scope;

    setbuf(stdout, NULL);

    // Global initiallization of libavcodec.
    av_register_all();
    avformat_network_init();

    v8::Local<v8::Object> version = v8::Object::New();
    version->Set(Nan::New("major").ToLocalChecked(), Nan::New(LIBAVFORMAT_VERSION_MINOR));
    version->Set(Nan::New("minor").ToLocalChecked(), Nan::New(LIBAVFORMAT_VERSION_MINOR));
    version->Set(Nan::New("patch").ToLocalChecked(), Nan::New(LIBAVFORMAT_VERSION_MINOR));

    target->Set(Nan::New("AVFormatVersion").ToLocalChecked(), version);
    target->Set(Nan::New("PixelFormat").ToLocalChecked(), CreatePixelFormatsEnum());
    target->Set(Nan::New("CodecId").ToLocalChecked(), CreateCodecIdEnum());

//    Nan::Set(target,

    NAVFormat::Init(target);
    NAVOutputFormat::Init(target);
    NAVSws::Init(target);
    NAVResample::Init(target);
    NAVThumbnail::Init(target);

    target->Set(Nan::New("relocateMoov").ToLocalChecked(),Nan::New<v8::FunctionTemplate>(RelocateMoov)->GetFunction());

    // Objects only instantiable from C++
    NAVFrame::Init();
    NAVStream::Init();
    NAVCodecContext::Init();
    NAVDictionary::Init();

    DecoderNotifier::Init();
}

NODE_MODULE(navcodec, init);
