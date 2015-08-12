/////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成while语句的抽象类MyWhileStatement的设计
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYWHILESTATEMENT_H
#define MYWHILESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//这里我们把while语句 抽象成 mystatement 的继承类
//
class PARSERS_EXPORT MyWhileStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //expression     const XMLCh* const          条件表达试
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
    MyWhileStatement(const XMLCh* const    expression
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //方法描述:返回while语句的条件表达试
    //返回值::
    //返回类型                描述
    //const XMLCh*            while语句的条件表达试
    const XMLCh* getExpressionString() const;


    //析构函数
    virtual ~MyWhileStatement();

private:

    //初始化
    void initialize();                 
    //清楚变量内存
    void cleanUp();

    //方法描述:设置while语句的条件表达试
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //exp           const XMLCh* const          while语句的条件表达试
    void setExpressionString(const XMLCh* const expression);

    XMLCh*            fExpression;                          //记录条件表达试

    /**
    * Copy constructor
    */
    MyWhileStatement(const MyWhileStatement&);
    MyWhileStatement& operator=(const MyWhileStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif