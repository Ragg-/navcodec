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

#include "navpixformat.h"
#include "navutils.h"

<<<<<<< HEAD
v8::Local<v8::Object> CreatePixelFormatsEnum(v8::Isolate *isolate) {
  v8::EscapableHandleScope scope(isolate);

  Handle<Object> obj = Object::New(isolate);

  OBJECT_SET_ENUM(obj, PIX_FMT_NONE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV420P);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUYV422);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB24);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR24);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV422P);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV444P);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV410P);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV411P);
  OBJECT_SET_ENUM(obj, PIX_FMT_GRAY8);
  OBJECT_SET_ENUM(obj, PIX_FMT_MONOWHITE);
  OBJECT_SET_ENUM(obj, PIX_FMT_MONOBLACK);
  OBJECT_SET_ENUM(obj, PIX_FMT_PAL8);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUVJ420P);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUVJ422P);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUVJ444P);
  OBJECT_SET_ENUM(obj, PIX_FMT_XVMC_MPEG2_MC);
  OBJECT_SET_ENUM(obj, PIX_FMT_XVMC_MPEG2_IDCT);
  OBJECT_SET_ENUM(obj, PIX_FMT_UYVY422);
  OBJECT_SET_ENUM(obj, PIX_FMT_UYYVYY411);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR4_BYTE);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB8);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB4);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB4_BYTE);
  OBJECT_SET_ENUM(obj, PIX_FMT_NV12);
  OBJECT_SET_ENUM(obj, PIX_FMT_NV21);
  OBJECT_SET_ENUM(obj, PIX_FMT_ARGB);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGBA);
  OBJECT_SET_ENUM(obj, PIX_FMT_ABGR);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGRA);
  OBJECT_SET_ENUM(obj, PIX_FMT_GRAY16BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_GRAY16LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV440P);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUVJ440P);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUVA420P);
  OBJECT_SET_ENUM(obj, PIX_FMT_VDPAU_H264);
  OBJECT_SET_ENUM(obj, PIX_FMT_VDPAU_MPEG1);
  OBJECT_SET_ENUM(obj, PIX_FMT_VDPAU_MPEG2);
  OBJECT_SET_ENUM(obj, PIX_FMT_VDPAU_WMV3);
  OBJECT_SET_ENUM(obj, PIX_FMT_VDPAU_VC1);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB48BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB48LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB565BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB565LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB555BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB555LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR565BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR565LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR555BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR555LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_VAAPI_MOCO);
  OBJECT_SET_ENUM(obj, PIX_FMT_VAAPI_IDCT);
  OBJECT_SET_ENUM(obj, PIX_FMT_VAAPI_VLD);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV420P16LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV420P16BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV422P16LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV422P16BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV444P16LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV444P16BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_VDPAU_MPEG4);
  OBJECT_SET_ENUM(obj, PIX_FMT_DXVA2_VLD);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB444LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_RGB444BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR444LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR444BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR48BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_BGR48LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV420P9BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV420P9LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV420P10BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV420P10LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV422P10BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV422P10LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV444P9BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV444P9LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV444P10BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV444P10LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV422P9BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_YUV422P9LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_GBRP);
  OBJECT_SET_ENUM(obj, PIX_FMT_GBRP9BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_GBRP9LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_GBRP10BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_GBRP10LE);
  OBJECT_SET_ENUM(obj, PIX_FMT_GBRP16BE);
  OBJECT_SET_ENUM(obj, PIX_FMT_GBRP16LE);

  return scope.Escape(obj);
=======
Handle<Object> CreatePixelFormatsEnum(){
  HandleScope scope;
  
  Handle<Object> obj = Object::New();
    
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_NONE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV420P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUYV422);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB24);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR24);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV422P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV444P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV410P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV411P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GRAY8);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_MONOWHITE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_MONOBLACK);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_PAL8);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUVJ420P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUVJ422P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUVJ444P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_XVMC_MPEG2_MC);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_XVMC_MPEG2_IDCT);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_UYVY422);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_UYYVYY411);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR4_BYTE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB8);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB4);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB4_BYTE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_NV12);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_NV21);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_ARGB);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGBA);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_ABGR);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGRA);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GRAY16BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GRAY16LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV440P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUVJ440P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUVA420P);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VDPAU_H264);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VDPAU_MPEG1);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VDPAU_MPEG2);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VDPAU_WMV3);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VDPAU_VC1);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB48BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB48LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB565BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB565LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB555BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB555LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR565BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR565LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR555BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR555LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VAAPI_MOCO);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VAAPI_IDCT);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VAAPI_VLD);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV420P16LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV420P16BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV422P16LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV422P16BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV444P16LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV444P16BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_VDPAU_MPEG4);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_DXVA2_VLD);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB444LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_RGB444BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR444LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR444BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR48BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_BGR48LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV420P9BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV420P9LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV420P10BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV420P10LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV422P10BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV422P10LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV444P9BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV444P9LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV444P10BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV444P10LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV422P9BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_YUV422P9LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GBRP);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GBRP9BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GBRP9LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GBRP10BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GBRP10LE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GBRP16BE);
  OBJECT_SET_ENUM(obj, AV_PIX_FMT_GBRP16LE);
  
  return scope.Close(obj);
>>>>>>> caldwell/new-ffmpeg
}

// ?
//PIX_FMT_Y400A
//8bit gray, 8bit alpha
