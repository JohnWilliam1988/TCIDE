// LzmaRegister.cpp

#include "StdAfx.h"

#include "../Common/RegisterCodec.h"

#include "../Common/LzmaDecoder.h"

static void *CreateCodec() { return (void *)(ICompressCoder *)(new NCompress::NLzma::CDecoder); }
#ifndef EXTRACT_ONLY
#include "../Common/LzmaEncoder.h"
static void *CreateCodecOut() { return (void *)(ICompressCoder *)(new NCompress::NLzma::CEncoder);  }
#else
#define CreateCodecOut 0
#endif

static CCodecInfo g_CodecInfo =
  { CreateCodec, CreateCodecOut, 0x030101, L"LZMA", 1, false };

REGISTER_CODEC(LZMA)
