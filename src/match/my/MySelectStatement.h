//////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成select语句的抽象基类MySelectStatement的设计
//////////////////////////////////////////////////////////////////////////
#ifndef MYSELECTSTATEMENT_H
#define MYSELECTSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们把 repeatstatement语句抽象成 mystatement 的继承类
//repeat语句主要是用来执行规定次数的重复操作
class PARSERS_EXPORT MySelectStatement : public MyStatement
{
public:

    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //num           unsigned int                重复次数    
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
	MySelectStatement(const XMLCh* const     expression
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//方法描述:返回Select语句的条件表达试
    //返回值::
    //返回类型                描述
    //const XMLCh*            if语句的条件表达试
	const XMLCh* getExpressionString() const;

    //析构函数
	virtual ~MySelectStatement();

private:
	void initialize();         //初始化

	//清楚变量内存
    void cleanUp();

	//方法描述:设置Select语句的条件表达试
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //exp           const XMLCh* const          if语句的条件表达试
	void setExpressionString(const XMLCh* const exp);


	XMLCh*            fExpression;                        //记录条件表达试

	MySelectStatement(const MySelectStatement&);
    MySelectStatement& operator=(const MySelectStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif