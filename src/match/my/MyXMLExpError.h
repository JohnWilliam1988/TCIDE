#if !defined(MYXMLEXPERROR_H)
#define MYXMLEXPERROR_H

#include <match/util/MatchDefs.hpp>
#include <match/util/XMLException.hpp>

MATCH_CPP_NAMESPACE_BEGIN

//------------------------------------------------------
// ������2010-4-8 ������쳣��Ϣ�Ĵ洢�ṹ��,������:�ӱ�
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
// ������2010-4-8 ������쳣��Ϣ�Ĵ洢�ṹ��,������:�ӱ�
//------------------------------------------------------


MATCH_CPP_NAMESPACE_END

#endif