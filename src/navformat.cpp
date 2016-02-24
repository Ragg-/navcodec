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

#include <string.h>

#include "navformat.h"
#include "navframe.h"
#include "navdictionary.h"
#include "navutils.h"

#define MAX_NUM_STREAMFRAMES  16

using namespace v8;

class DecoderNotifier;

typedef struct {
  Persistent<Object> stream;
  Persistent<Object> frame;
  AVStream *pStream;
  AVFrame *pFrame;
} StreamFrame;

struct Baton {
  uv_work_t request;
  Persistent<Object> navformat;
  Persistent<Function> callback;
  Persistent<Object> notifier;

  // Input
  AVFormatContext *pFormatCtx;
  unsigned int numStreams;
  StreamFrame streamFrames[MAX_NUM_STREAMFRAMES];

  // Output
  int result;
  int streamIndex;

  const char *error;
};

//
// Decode Frame
//

static int decodeFrame(AVFormatContext *pFormatCtx,
                       unsigned int numStreams,
                       StreamFrame *streamFrames,
                       Baton *pBaton){
  AVPacket packet;

  int ret, finished;

  pBaton->error = NULL;
  pBaton->streamIndex = -1;

  while((ret=av_read_frame(pFormatCtx, &packet))>=0){

    unsigned int i;
    int res;

    AVStream *pStream = NULL;
    AVFrame *pFrame = NULL;

    for(i=0;i<numStreams; i++){
      if (streamFrames[i].pStream->index == packet.stream_index) {
        pStream = streamFrames[i].pStream;
        pFrame = streamFrames[i].pFrame;
        break;
      }
    }

    if(pStream){
      switch(pStream->codec->codec_type){
        case AVMEDIA_TYPE_AUDIO:
          res = avcodec_decode_audio4(pStream->codec, pFrame, &finished, &packet);
          break;
        case AVMEDIA_TYPE_VIDEO:
          res = avcodec_decode_video2(pStream->codec, pFrame, &finished, &packet);
          break;
        default:
          res = -1;
      }
      av_free_packet(&packet);

      if(res<0){
        pBaton->error = "Error decoding frame";
        break;
      }

      if(finished){
        streamFrames[i].pFrame->owner = pStream->codec;
        pBaton->streamIndex = i;
        pFrame->pts = packet.pts;
        break;
      }
    }
  }

  return ret;
}

static void AsyncWork(uv_work_t* req) {
  Baton* pBaton = static_cast<Baton*>(req->data);

  decodeFrame(pBaton->pFormatCtx,
              pBaton->numStreams,
              pBaton->streamFrames,
              pBaton);
}

static void CleanUp(Baton *pBaton){
  for(unsigned int i=0;i<pBaton->numStreams; i++){
    avcodec_close(pBaton->streamFrames[i].pStream->codec);
    pBaton->streamFrames[i].stream.Reset();
    pBaton->streamFrames[i].frame.Reset();
  }

  pBaton->navformat.Reset();
  pBaton->callback.Reset();
  pBaton->notifier.Reset();
  delete pBaton;
}

static void AsyncAfter(uv_work_t* req) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  Baton* pBaton = static_cast<Baton*>(req->data);

  v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, pBaton->callback);

  if (pBaton->error) {
    Local<Value> err = Exception::Error(String::NewFromUtf8(isolate, pBaton->error));
    Local<Value> argv[] = { err };

    //TryCatch try_catch;
    cb->Call(isolate->GetCurrentContext()->Global(), 1, argv);

    /*
    if (try_catch.HasCaught()) {
      node::FatalException(try_catch);
    }
    */

    CleanUp(pBaton);
  } else {
    if(pBaton->streamIndex >= 0){
       int i = pBaton->streamIndex;

      // Call callback with decoded frame
      Handle<Value> argv[] = {
        v8::Local<v8::Object>::New(isolate, pBaton->streamFrames[i].stream),
        v8::Local<v8::Object>::New(isolate, pBaton->streamFrames[i].frame),
        Integer::New(isolate, pBaton->pFormatCtx->pb->pos),
        v8::Local<v8::Object>::New(isolate, pBaton->notifier)
      };

      cb->Call(isolate->GetCurrentContext()->Global(), 4, argv);
    }else{
      Local<Value> argv[] = { Null(isolate) };
      cb->Call(isolate->GetCurrentContext()->Global(), 1, argv);

      // Close all the codecs from the given streams & dispose V8 objects
     CleanUp(pBaton);
    }
  }
}

// DecoderNotifyer

v8::Persistent<v8::ObjectTemplate> DecoderNotifier::constructor;

DecoderNotifier::DecoderNotifier(Baton *pBaton){
  this->pBaton = pBaton;
}

DecoderNotifier::~DecoderNotifier(){
  printf("DecoderNotifier destructor\n");
}

void DecoderNotifier::Init(v8::Isolate *isolate) {
  v8::Local<v8::ObjectTemplate> tpl = v8::ObjectTemplate::New(isolate);
  tpl->SetInternalFieldCount(1);

  constructor.Reset(isolate, tpl);
}

v8::Local<v8::Object> DecoderNotifier::New(Baton *pBaton) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(isolate);

  DecoderNotifier *instance = new DecoderNotifier(pBaton);

  v8::Local<v8::ObjectTemplate> cons = v8::Local<v8::ObjectTemplate>::New(isolate, constructor);
  v8::Local<v8::Object> obj = cons->NewInstance();
  instance->Wrap(obj);

  SET_KEY_VALUE(obj, "done", v8::FunctionTemplate::New(isolate, Done)->GetFunction());

  return scope.Escape(obj);
}

void DecoderNotifier::Done(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();

  DecoderNotifier* obj = ObjectWrap::Unwrap<DecoderNotifier>(args.This());

 if(args.Length() == 0){
    // Process next frame
    uv_queue_work(uv_default_loop(),
                  &(obj->pBaton->request),
                  AsyncWork,
                  (uv_after_work_cb)AsyncAfter);
  }

  args.GetReturnValue().Set(v8::Undefined(isolate));
}

//-------------------------------------------------------------------------------------------

NAVFormat::NAVFormat(){
  filename = NULL;
  pFormatCtx = NULL;
}

NAVFormat::~NAVFormat(){
  fprintf(stderr, "NAVFormat destructor\n");
  avformat_close_input(&pFormatCtx);
  free(filename);
}

v8::Persistent<v8::Function> NAVFormat::constructor;

void NAVFormat::Init(v8::Local<v8::Object> target) {
  v8::Isolate *isolate = target->GetIsolate();

  // Our constructor
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);

  tpl->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
  tpl->SetClassName(String::NewFromUtf8(isolate, "NAVFormat"));

  NODE_SET_PROTOTYPE_METHOD(tpl, "dump", Dump);
  NODE_SET_PROTOTYPE_METHOD(tpl, "decode", Decode);

  // Binding our constructor function to the target variable
  constructor.Reset(isolate, tpl->GetFunction());
  target->Set(String::NewFromUtf8(isolate, "NAVFormat"), tpl->GetFunction());
}

void NAVFormat::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();
  AVFormatContext *pFormatCtx;

  v8::Local<v8::Object> self = args.This();

  NAVFormat* instance = new NAVFormat();

  // Wrap our C++ object as a Javascript object
  instance->Wrap(self);

  if(args.Length() < 1){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Missing input filename")));
    return;
  }

  String::Utf8Value v8str(args[0]);
  instance->filename = strdup(*v8str);
  if(instance->filename == NULL){
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error allocating filename string")));
    return;
  }

  int ret = avformat_open_input(&(instance->pFormatCtx), instance->filename, NULL, NULL);
  if(ret<0){
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error Opening Intput")));
    return;
  }

  pFormatCtx = instance->pFormatCtx;

  ret = avformat_find_stream_info(pFormatCtx, NULL);
  if(ret<0){
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error Finding Streams")));
    return;
  }

  if(pFormatCtx->nb_streams>0){
    Handle<Array> streams = Array::New(isolate, pFormatCtx->nb_streams);
    for(unsigned int i=0; i < pFormatCtx->nb_streams;i++){
      AVStream *stream = pFormatCtx->streams[i];
      streams->Set(i, NAVStream::New(isolate, stream));
    }
    SET_KEY_VALUE(self, "streams", streams);
    SET_KEY_VALUE(self, "duration", Number::New(isolate, pFormatCtx->duration / (float) AV_TIME_BASE));
  }

  SET_KEY_VALUE(self, "metadata", NAVDictionary::New(isolate, pFormatCtx->metadata));

  args.GetReturnValue().Set(self);
}

// ([streams], cb(stream, frame))
void NAVFormat::Decode(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();
  Local<Array> streams;
  Local<Function> callback;

  StreamFrame streamFrames[MAX_NUM_STREAMFRAMES];

  if(!(args[0]->IsArray())){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "First parameter must be an array")));
    return;
  }
  if(!(args[1]->IsFunction())){
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Second parameter must be a funcion")));
    return;
  }

  Local<Object> self = args.This();

  NAVFormat* instance = UNWRAP_OBJECT(NAVFormat, args);

  streams = Local<Array>::Cast(args[0]);
  callback = Local<Function>::Cast(args[1]);

  //
  // Create the required frames and associate every frame to a stream.
  // And open codecs.
  //
  for(unsigned int i=0;i<streams->Length(); i++){
    AVStream *pStream;

    v8::Local<v8::Object> stream = streams->Get(i)->ToObject();
    streamFrames[i].stream.Reset(isolate, stream);
    pStream = node::ObjectWrap::Unwrap<NAVStream>(stream)->pContext;

    streamFrames[i].pStream = pStream;
    streamFrames[i].pFrame = avcodec_alloc_frame();

    v8::Local<v8::Object> frame = NAVFrame::New(isolate, streamFrames[i].pFrame);
    streamFrames[i].frame.Reset(isolate, frame);

    AVCodecContext *pCodecCtx = streamFrames[i].pStream->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Could not find decoder!")));
      return;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Could not open decoder!")));
      return;
    }
  }

  //
  // Start decoding
  //

  Baton* pBaton = new Baton();
  pBaton->navformat.Reset(isolate, self);
  pBaton->pFormatCtx = instance->pFormatCtx;
  pBaton->numStreams = streams->Length();

  memcpy((void*)&(pBaton->streamFrames), (void*)&streamFrames, sizeof(streamFrames));

  pBaton->notifier.Reset(isolate, DecoderNotifier::New(pBaton));
  pBaton->request.data = pBaton;
  pBaton->callback.Reset(isolate, callback);

  uv_queue_work(uv_default_loop(),
                &pBaton->request,
                AsyncWork,
                (uv_after_work_cb)AsyncAfter);

  args.GetReturnValue().Set(Undefined(isolate));
}

void NAVFormat::Dump(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = args.GetIsolate();

  NAVFormat* instance = UNWRAP_OBJECT(NAVFormat, args);

  av_dump_format(instance->pFormatCtx, 0, instance->filename, 0);

  args.GetReturnValue().Set(Integer::New(isolate, 0));
}
