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
 * $Id: MatchDefs.hpp 932887 2010-04-11 13:04:59Z borisk $
 */

#if !defined(MATCHC_INCLUDE_GUARD_MATCHDEFS_HPP)
#define MATCHC_INCLUDE_GUARD_MATCHDEFS_HPP

// ---------------------------------------------------------------------------
//  The file match_hdr_config defines critical configuration information
//	used by the remainder of this file.
//
//	There are two major configuration files:
//		- match_autoconf_config.hpp-- Contains defines that are safe for
//									   access through public headers.
//
//		- config.h					-- Contains defines that may conflict
//									   with other packages; should only be
//									   included by Match implementation files.
//
//	Both of these files are generated through the autoconf/configure process.
// ---------------------------------------------------------------------------

//
// If this is an autoconf configured build, we include Match_autoconf_config.hpp
// Otherwise we include a preconfigured config appropriate for the particular
// platform that the specific makefile should copy over.
//
//  If the next line generates an error then you haven't run ./configure
#include	<match/util/Match_autoconf_config.hpp>

// ---------------------------------------------------------------------------
//  Include the Match version information; this is kept in a separate file to
//  make modification simple and obvious. Updates to the version header file
// ---------------------------------------------------------------------------
#include    <match/util/MatchVersion.hpp>


// ---------------------------------------------------------------------------
//  Some general typedefs that are defined for internal flexibility.
//
//  Note  that UTF16Ch is fixed at 16 bits, whereas XMLCh floats in size per
//  platform, to whatever is the native wide char format there. UCS4Ch is
//  fixed at 32 bits. The types we defined them in terms of are defined per
//  compiler, using whatever types are the right ones for them to get these
//  16/32 bit sizes.
//
// ---------------------------------------------------------------------------
typedef unsigned char       XMLByte;
typedef XMLUInt16           UTF16Ch;
typedef XMLUInt32           UCS4Ch;


// ---------------------------------------------------------------------------
//  Handle boolean. If the platform can handle booleans itself, then we
//  map our boolean type to the native type. Otherwise we create a default
//  one as an int and define const values for true and false.
//
//  This flag will be set in the per-development environment stuff above.
// ---------------------------------------------------------------------------
#if defined(MATCH_NO_NATIVE_BOOL)
  #ifndef bool
    typedef int     bool;
  #endif
  #ifndef true
    #define  true     1
  #endif
  #ifndef false
    #define false 0
  #endif
#endif

// ---------------------------------------------------------------------------
//  According to whether the compiler supports L"" type strings, we define
//  the XMLStrL() macro one way or another.
// ---------------------------------------------------------------------------
#if defined(MATCH_LSTRSUPPORT)
#define XMLStrL(str)  L##str
#else
#define XMLStrL(str)  str
#endif


// ---------------------------------------------------------------------------
// Define namespace symbols if the compiler supports it.
// ---------------------------------------------------------------------------
#if defined(MATCH_HAS_CPP_NAMESPACE)
    #define MATCH_CPP_NAMESPACE_BEGIN namespace MATCH_CPP_NAMESPACE {
    #define MATCH_CPP_NAMESPACE_END  }
    #define MATCH_CPP_NAMESPACE_USE using namespace MATCH_CPP_NAMESPACE;
    #define MATCH_CPP_NAMESPACE_QUALIFIER MATCH_CPP_NAMESPACE::

    namespace MATCH_CPP_NAMESPACE { }
    namespace matchc = MATCH_CPP_NAMESPACE;
#else
    #define MATCH_CPP_NAMESPACE_BEGIN
    #define MATCH_CPP_NAMESPACE_END
    #define MATCH_CPP_NAMESPACE_USE
    #define MATCH_CPP_NAMESPACE_QUALIFIER
#endif

#if defined(MATCH_STD_NAMESPACE)
	#define MATCH_USING_STD(NAME) using std :: NAME;
	#define MATCH_STD_QUALIFIER  std ::
#else
	#define MATCH_USING_STD(NAME)
	#define MATCH_STD_QUALIFIER 
#endif


// ---------------------------------------------------------------------------
//  Set up the import/export keyword  for our core projects. The
//  PLATFORM_XXXX keywords are set in the per-development environment
//  include above.
// ---------------------------------------------------------------------------

// The DLL_EXPORT flag should be defined on the command line during the build of a DLL
// configure conspires to make this happen.

#if defined(DLL_EXPORT)
  #if defined(MATCH_BUILDING_LIBRARY)
    #define XMLUTIL_EXPORT MATCH_PLATFORM_EXPORT
    #define XMLPARSER_EXPORT MATCH_PLATFORM_EXPORT
    #define SAX_EXPORT MATCH_PLATFORM_EXPORT
    #define SAX2_EXPORT MATCH_PLATFORM_EXPORT
    #define CDOM_EXPORT MATCH_PLATFORM_EXPORT
    #define PARSERS_EXPORT  MATCH_PLATFORM_EXPORT
    #define VALIDATORS_EXPORT  MATCH_PLATFORM_EXPORT
    #define XINCLUDE_EXPORT  MATCH_PLATFORM_EXPORT
  #else
    #define XMLUTIL_EXPORT MATCH_PLATFORM_IMPORT
    #define XMLPARSER_EXPORT MATCH_PLATFORM_IMPORT
    #define SAX_EXPORT MATCH_PLATFORM_IMPORT
    #define SAX2_EXPORT MATCH_PLATFORM_IMPORT
    #define CDOM_EXPORT MATCH_PLATFORM_IMPORT
    #define PARSERS_EXPORT  MATCH_PLATFORM_IMPORT
    #define VALIDATORS_EXPORT  MATCH_PLATFORM_IMPORT
    #define XINCLUDE_EXPORT  MATCH_PLATFORM_IMPORT
  #endif
  #if defined(MATCH_BUILDING_DEPRECATED_LIBRARY)
    #define DEPRECATED_DOM_EXPORT MATCH_PLATFORM_EXPORT
  #else
    #define DEPRECATED_DOM_EXPORT MATCH_PLATFORM_IMPORT
  #endif
#else
  #define XMLUTIL_EXPORT 
  #define XMLPARSER_EXPORT 
  #define SAX_EXPORT 
  #define SAX2_EXPORT
  #define CDOM_EXPORT
  #define DEPRECATED_DOM_EXPORT 
  #define PARSERS_EXPORT 
  #define VALIDATORS_EXPORT
  #define XINCLUDE_EXPORT
#endif


/////////////////////这里添加一个用于调试的宏 4-13 肖秀范,定义就是加日志信息,不定义就是没有日志信息//////////////

#define   TC_DEBUG                               //标示是生成的DEBUG版本的,可以做断点调试

/////////////////////TC自定义的宏//////////////////////////////////////////
const int          MAXERRMSGLENGTH=2048;          //错误信息最大长度
const unsigned int MAXTEMPLENGTH=1024;            //临时使用的长度
const unsigned int MAXFUNPARAMCOUNT=48;           //函数参数的最大个数
const unsigned int NAMEMAXLENGTH=32;              //名字最大长度
const unsigned int TYPEMAXLENGTH=8;               //类型最大长度
const unsigned int DEBUGVALUELISTMAXLENGTH=256;   //变量列表的最大长度
const int TCMAXEXPRESSSTRINGLENG=1032;            //表达式的最大的长度
const int MATCH_STARTLINE=0;                      //////////////////
const int MATCH_MAXBREAKBPNUM=256;                //最大的断点个数
const unsigned int MAXPARAMLENGTH=256;            //函数参数的长度
const unsigned int MAXFUNLISTLENGTH=128+32;       //一个空间中最多记录多少个函数
const unsigned int MAXNSLISTLENGTH=8;            //一个文件最多记录多少个空间的
static const int TC_MAXARRAYCOUNT=8;              //TC支持的数组的最大维数
const unsigned int MAXGLOBLEVARLIST=64;           //空间函数下面记录变量的个数
const unsigned int MAXDEBUGVARBUFFLENGTH=3*1024*1024;           //空间函数下面记录变量的个数
const unsigned int INITIALTHREADPOOL=128;           //空间函数下面记录变量的个数
const unsigned int VALUEMAXLENGTH=256;            //变量值最大长度
//////////////////////////////////////////////////////////////////////////

#endif
