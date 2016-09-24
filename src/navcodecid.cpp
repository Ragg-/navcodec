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
 * FROM); OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "navcodecid.h"
#include "navutils.h"

v8::Local<v8::Object> CreateCodecIdEnum(v8::Isolate *isolate) {
  v8::EscapableHandleScope scope(isolate);
  v8::Local<v8::Object> obj = v8::Object::New(isolate);

  OBJECT_SET_ENUM(obj, AV_CODEC_ID_NONE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MPEG1VIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MPEG2VIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MPEG2VIDEO_XVMC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_H261);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_H263);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RV10);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RV20);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MJPEG);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MJPEGB);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_LJPEG);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SP5X);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_JPEGLS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MPEG4);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RAWVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MSMPEG4V1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MSMPEG4V2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MSMPEG4V3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMV1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMV2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_H263P);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_H263I);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FLV1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SVQ1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SVQ3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DVVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_HUFFYUV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_CYUV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_H264);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_INDEO3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VP3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_THEORA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ASV1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ASV2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FFV1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_4XM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VCR1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_CLJR);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MDEC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ROQ);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_INTERPLAY_VIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_XAN_WC3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_XAN_WC4);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RPZA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_CINEPAK);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WS_VQA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MSRLE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MSVIDEO1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_IDCIN);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_8BPS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SMC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FLIC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TRUEMOTION1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VMDVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MSZH);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ZLIB);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_QTRLE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TSCC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ULTI);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_QDRAW);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VIXL);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_QPEG);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PNG);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PPM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PBM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PGM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PGMYUV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PAM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FFVHUFF);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RV30);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RV40);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VC1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMV3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_LOCO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WNV1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AASC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_INDEO2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FRAPS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TRUEMOTION2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_BMP);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_CSCD);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MMVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ZMBV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AVS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SMACKVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_NUV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_KMVC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FLASHSV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_CAVS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_JPEG2000);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VMNC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VP5);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VP6);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VP6F);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TARGA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DSICINVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TIERTEXSEQVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TIFF);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_GIF);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DXA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DNXHD);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_THP);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SGI);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_C93);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_BETHSOFTVID);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PTX);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TXD);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VP6A);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AMV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VB);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCX);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SUNRAST);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_INDEO4);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_INDEO5);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MIMIC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RL2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_8SVX_EXP);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_8SVX_FIB);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ESCAPE124);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DIRAC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_BFI);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_CMV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MOTIONPIXELS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TGV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TGQ);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TQI);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AURA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AURA2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_V210X);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TMV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_V210);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DPX);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MAD);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FRWU);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FLASHSV2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_CDGRAPHICS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_R210);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ANM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_BINKVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_IFF_ILBM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_IFF_BYTERUN1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_KGV1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_YOP);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VP8);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PICTOR);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ANSI);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_A64_MULTI);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_A64_MULTI5);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_R10K);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MXPEG);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_LAGARITH);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PRORES);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_JV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DFA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMV3IMAGE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VC1IMAGE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_G723_1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_G729);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_UTVIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_BMV_VIDEO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VBLE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DXTORY);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_V410);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FIRST_AUDIO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S16LE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S16BE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_U16LE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_U16BE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S8);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_U8);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_MULAW);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_ALAW);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S32LE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S32BE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_U32LE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_U32BE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S24LE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S24BE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_U24LE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_U24BE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S24DAUD);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_ZORK);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S16LE_PLANAR);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_DVD);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_F32BE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_F32LE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_F64BE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_F64LE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_BLURAY);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_LXF);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_S302M);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PCM_S8_PLANAR);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_QT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_WAV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_DK3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_DK4);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_WS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_SMJPEG);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_MS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_4XM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_XA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_ADX);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_EA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_G726);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_CT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_SWF);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_YAMAHA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_SBPRO_4);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_SBPRO_3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_SBPRO_2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_THP);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_AMV);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_EA_R1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_EA_R3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_EA_R2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_EA_SEAD);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_EA_EACS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_EA_XAS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_EA_MAXIS_XA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_IMA_ISS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ADPCM_G722);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AMR_NB);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AMR_WB);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RA_144);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_RA_288);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ROQ_DPCM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_INTERPLAY_DPCM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_XAN_DPCM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SOL_DPCM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MP2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MP3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AAC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AC3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DTS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VORBIS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DVAUDIO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMAV1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMAV2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MACE3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MACE6);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VMDAUDIO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FLAC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MP3ADU);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MP3ON4);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SHORTEN);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ALAC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WESTWOOD_SND1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_GSM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_QDM2);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_COOK);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TRUESPEECH);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TTA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SMACKAUDIO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_QCELP);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WAVPACK);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DSICINAUDIO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_IMC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MUSEPACK7);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MLP);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_GSM_MS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ATRAC3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_VOXWARE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_APE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_NELLYMOSER);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MUSEPACK8);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SPEEX);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMAVOICE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMAPRO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_WMALOSSLESS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ATRAC3P);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_EAC3);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SIPR);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MP1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TWINVQ);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TRUEHD);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MP4ALS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_ATRAC1);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_BINKAUDIO_RDFT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_BINKAUDIO_DCT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_AAC_LATM);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_QDMC);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_CELT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_BMV_AUDIO);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FIRST_SUBTITLE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DVD_SUBTITLE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DVB_SUBTITLE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TEXT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_XSUB);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SSA);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MOV_TEXT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_HDMV_PGS_SUBTITLE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_DVB_TELETEXT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_SRT);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FIRST_UNKNOWN);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_TTF);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_PROBE);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MPEG2TS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_MPEG4SYSTEMS);
  OBJECT_SET_ENUM(obj, AV_CODEC_ID_FFMETADATA);

  return scope.Escape(obj);
}
