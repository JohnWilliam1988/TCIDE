#if !defined(MYERROR_H)
#define MYERROR_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyError
{
public:
	//方法描述:错误处理                                                                                    
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称      参数类型                    描述
	//msg           const XMLCh * const         错误信息
	static void err(int line,const XMLCh * const file,const XMLCh * const msg);
private:
	MyError();

	//CCF 拷贝构造函数
	MyError(const MyError& pool);
	MyError& operator=(const MyError& toAssign);

};


MATCH_CPP_NAMESPACE_END



#endif