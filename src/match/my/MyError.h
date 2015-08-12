#if !defined(MYERROR_H)
#define MYERROR_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyError
{
public:
	//��������:������                                                                                    
    //��������                ����
    //void                    ��
    //����˵��:
    //��������      ��������                    ����
	//msg           const XMLCh * const         ������Ϣ
	static void err(int line,const XMLCh * const file,const XMLCh * const msg);
private:
	MyError();

	//CCF �������캯��
	MyError(const MyError& pool);
	MyError& operator=(const MyError& toAssign);

};


MATCH_CPP_NAMESPACE_END



#endif