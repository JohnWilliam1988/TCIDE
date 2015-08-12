/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: Match_autoconf_config.msvc.hpp 883299 2009-11-23 10:34:54Z amassari $
 */

#if !defined(MATCHC_INCLUDE_GUARD_MATCH_AUTOCONFIG_CONFIG_HPP)
#define MATCHC_INCLUDE_GUARD_MATCH_AUTOCONFIG_CONFIG_HPP

//
// There are two primary match configuration header files:
//
//	Match_autoconf_config.hpp
//
//      For configuration of items that must be accessable
//	through public headers. This file has limited information
//	and carefully works to avoid collision of macro names, etc.
//
//	This file is included by MatchDefs.h.
//	This version of the file is specific for Microsoft Visual C++
//	family of compilers
//
//	config.h
//
//      This file is not used with Microsoft Visual C++; the macros
//	it would specify are instead hardcoded in the makefiles
//

#include <basetsd.h>

// silence the warning "while compiling class-template member function xxxx : identifier was truncated to '255'
// characters in the browser information"
#pragma warning( disable: 4786 )

// ---------------------------------------------------------------------------
//  These defines have been hardcoded for the Microsoft Visual C++ compilers
// ---------------------------------------------------------------------------
#undef MATCH_AUTOCONF
#undef MATCH_HAVE_SYS_TYPES_H
#undef MATCH_HAVE_INTTYPES_H

#define MATCH_S16BIT_INT   signed short
#define MATCH_U16BIT_INT   unsigned short
#define MATCH_S32BIT_INT   INT32
#define MATCH_U32BIT_INT   UINT32

// While VC6 has 64-bit int, there is no support in the libraries
// (e.g., iostream). So we are going to stick to  32-bit ints.
//
#if (_MSC_VER >= 1300)
#  define MATCH_S64BIT_INT   INT64
#  define MATCH_U64BIT_INT   UINT64
#else
#  define MATCH_S64BIT_INT   INT32
#  define MATCH_U64BIT_INT   UINT32
#endif

#ifdef _NATIVE_WCHAR_T_DEFINED
#  define MATCH_XMLCH_T      wchar_t
#else
#  define MATCH_XMLCH_T      unsigned short
#endif

#define MATCH_SIZE_T       SIZE_T
#define MATCH_SSIZE_T      SSIZE_T

#define MATCH_HAS_CPP_NAMESPACE    1
#define MATCH_STD_NAMESPACE        1
#define MATCH_NEW_IOSTREAMS        1
#undef MATCH_NO_NATIVE_BOOL
#define MATCH_LSTRSUPPORT          1

#ifdef MATCH_STATIC_LIBRARY
#  define MATCH_PLATFORM_EXPORT
#  define MATCH_PLATFORM_IMPORT
#else
#  define MATCH_PLATFORM_EXPORT __declspec(dllexport)
#  define MATCH_PLATFORM_IMPORT __declspec(dllimport)
#  define DLL_EXPORT
#endif

#define MATCH_MFC_SUPPORT

#if (_MSC_VER >= 1400)
#  define MATCH_HAVE_INTRIN_H 1
#  define MATCH_HAVE_EMMINTRIN_H 1
#  define MATCH_HAVE_CPUID_INTRINSIC
#  define MATCH_HAVE_SSE2_INTRINSIC
#endif

// ---------------------------------------------------------------------------
//  XMLSize_t is the unsigned integral type.
// ---------------------------------------------------------------------------
typedef MATCH_SIZE_T				XMLSize_t;
typedef MATCH_SSIZE_T				XMLSSize_t;

// ---------------------------------------------------------------------------
//  Define our version of the XML character
// ---------------------------------------------------------------------------
typedef MATCH_XMLCH_T				XMLCh;

// ---------------------------------------------------------------------------
//  Define unsigned 16, 32, and 64 bit integers
// ---------------------------------------------------------------------------
typedef MATCH_U16BIT_INT			XMLUInt16;
typedef MATCH_U32BIT_INT			XMLUInt32;
typedef MATCH_U64BIT_INT			XMLUInt64;

// ---------------------------------------------------------------------------
//  Define signed 16, 32, and 64 bit integers
// ---------------------------------------------------------------------------
typedef MATCH_S16BIT_INT			XMLInt16;
typedef MATCH_S32BIT_INT			XMLInt32;
typedef MATCH_S64BIT_INT			XMLInt64;

// ---------------------------------------------------------------------------
//  XMLFilePos is the type used to represent a file position.
// ---------------------------------------------------------------------------
typedef XMLUInt64			        XMLFilePos;

// ---------------------------------------------------------------------------
//  XMLFileLoc is the type used to represent a file location (line/column).
// ---------------------------------------------------------------------------
typedef XMLUInt64			        XMLFileLoc;

// ---------------------------------------------------------------------------
//  Force on the Match debug token if it is on in the build environment
// ---------------------------------------------------------------------------
#if defined(_DEBUG)
#  define MATCH_DEBUG
#endif

#endif
