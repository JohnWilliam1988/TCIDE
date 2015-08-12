/////////////////////////////////2009.12.20  肖秀范修订//////////////////////////

#ifndef MYRETURNSTATEMENT_H
#define MYRETURNSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们把 return 语句 抽象成 mystatement  继承类
//return语句主要是用来处理 函数表达试返回的
class PARSERS_EXPORT MyReturnStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //expression     const XMLCh* const          计算表达试
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
    MyReturnStatement(const XMLCh* const     expression
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //方法描述:返回return语句的计算表达试
    //返回值::
    //返回类型                描述
    //const XMLCh*            计算表达试
    const XMLCh* getReturnExpressionString() const;

    //析构
    virtual ~MyReturnStatement();

private:
    //初始化
    void initialize();                 

    //清楚变量内存
    void cleanUp();

    //方法描述:设置return计算表达试的字符串
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //exp           const XMLCh* const          return的计算表达试
    void setReturnExpressionString(const XMLCh* const  expression);

    XMLCh*            fReturnExpression;         //记录flag包含文件的路径

    /**
    * Copy constructor
    */
    MyReturnStatement(const MyReturnStatement&);
    MyReturnStatement& operator=(const MyReturnStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif