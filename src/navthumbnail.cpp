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
#include "navthumbnail.h"

#include "navformat.h"
#include "navframe.h"
#include "navutils.h"

using namespace v8;


#define MAX_BPP 4

NAVThumbnail::NAVThumbnail(){
  pContext = NULL;
  pBuffer = NULL;
}

NAVThumbnail::~NAVThumbnail(){
  printf("NAVThumbnail destructor\n");
  av_free(pBuffer);
  if(pContext){
    avcodec_close(pContext);
    av_free(pContext);
  }
}

Persistent<Function> NAVThumbnail::constructor;

void NAVThumbnail::Init(v8::Local<v8::Object> target){
  v8::Isolate *isolate = target->GetIsolate();

  // Our constructor
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);

  tpl->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
  tpl->SetClassName(String::NewFromUtf8(isolate, "NAVThumbnail"));

  NODE_SET_PROTOTYPE_METHOD(tpl, "write", Write);

  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "NAVThumbnail"), tpl->GetFunction());
}

// (options)
void NAVThumbnail::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();

  NAVThumbnail* instance = new NAVThumbnail();
  instance->Wrap(args.This());

  if (args.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Missing input parameter (srcStream)")));
    return;
  }

  Local<Object> options = Local<Object>::Cast(args[0]);

  AVCodec *pCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
	if (!pCodec) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Could not alloc codec")));
    return;
	}

  instance->pContext = avcodec_alloc_context3(pCodec);
	if (!instance->pContext) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Could not alloc codec context")));
		return;
	}

	instance->pContext->width = GET_OPTION_UINT32(options, width, 128);
	instance->pContext->height = GET_OPTION_UINT32(options, height, 128);;
	instance->pContext->pix_fmt = (PixelFormat) GET_OPTION_UINT32(options, pix_fmt, PIX_FMT_YUVJ420P);

  instance->pContext->time_base.num = 1;
  instance->pContext->time_base.den = 1;

  AVDictionary *pDict = NAVDictionary::New(options);
	if (avcodec_open2(instance->pContext, pCodec, &pDict) < 0) {
    av_dict_free(&pDict);
    delete instance;
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Could not open codec")));
    return;
	}
  av_dict_free(&pDict);

  instance->bufferSize = (instance->pContext->width * instance->pContext->height * MAX_BPP) / 8;
  instance->bufferSize = instance->bufferSize < FF_MIN_BUFFER_SIZE ?
                         FF_MIN_BUFFER_SIZE:instance->bufferSize;

	instance->pBuffer = (uint8_t *) av_mallocz(instance->bufferSize);
  if (instance->pBuffer == NULL){
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error allocating buffer")));
    return;
  }

  args.GetReturnValue().Set(args.This());
}

// (frame, filename, cb(err))
void NAVThumbnail::Write(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();

  Local<Object> frame;
  Local<Function> callback;

  if (args.Length() < 3) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Missing input parameters (frame, filename, cb)")));
    return;
  }
  frame = Local<Array>::Cast(args[0]);

  AVFrame *pFrame = (node::ObjectWrap::Unwrap<NAVFrame>(frame))->pContext;

  String::Utf8Value v8str(args[1]);
  char *filename = *v8str;

  if(!(args[2]->IsFunction())){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Third parameter must be a function")));
    return;
  }

  callback = Local<Function>::Cast(args[2]);

  NAVThumbnail* instance = UNWRAP_OBJECT(NAVThumbnail, args);

	pFrame->pts = 1;
	pFrame->quality = instance->pContext->global_quality;

  AVPacket packet;
  packet.data = instance->pBuffer;
  packet.size = instance->bufferSize;

  av_init_packet(&packet);

  int gotPacket;
  if(avcodec_encode_video2(instance->pContext, &packet, pFrame, &gotPacket) < 0){
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error encoding thumbnail")));
    return;
  }
  int encodedSize = packet.size;

	FILE *fileHandle = fopen(filename, "wb");
  if(fileHandle == NULL){
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error opening thumbnail file")));
    return;
  }

	int ret = fwrite(instance->pBuffer, 1, encodedSize, fileHandle);
	fclose(fileHandle);

  // TODO: Make asynchronous
  if (ret < encodedSize) {
    Local<Value> err = Exception::Error(String::NewFromUtf8(isolate, "Error writing thumbnail"));
    Local<Value> argv[] = { err };
    callback->Call(isolate->GetCurrentContext()->Global(), 1, argv);
  } else {
    Local<Value> argv[] = { Null(isolate) };
    callback->Call(isolate->GetCurrentContext()->Global(), 1, argv);
  }

  args.GetReturnValue().Set(Undefined(isolate));
}
