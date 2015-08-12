#if !defined(MYXMLEXPERROR_H)
#define MYXMLEXPERROR_H

#include <match/util/MatchDefs.hpp>
#include <match/util/XMLException.hpp>

MATCH_CPP_NAMESPACE_BEGIN

//------------------------------------------------------
// 这里是2010-4-8 加入的异常信息的存储结构体,操作人:钟斌
//------------------------------------------------------
struct MyXMLExpError{
    XMLCh                   fSrcFile[256];
    unsigned int            fSrcLine;
    XMLCh                   fMsg[256];
    MyXMLExpError()
    {
        memset(fSrcFile,0,256);
        memset(fSrcFile,0,256);
        fSrcLine=-1;
    }
};
//------------------------------------------------------
// 这里是2010-4-8 加入的异常信息的存储结构体,操作人:钟斌
//------------------------------------------------------


MATCH_CPP_NAMESPACE_END

#endif