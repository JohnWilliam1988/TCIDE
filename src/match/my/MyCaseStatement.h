/////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成Case语句的抽象类MyCaseStatement的设计
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCASESTATEMENT_H
#define MYCASESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//我们把if语句 抽象成 mystatement 继承类
class PARSERS_EXPORT MyCaseStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                     描述
    //expression     const XMLCh* const          条件表达试
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyCaseStatement(const XMLCh* const    expression
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //方法描述:返回if语句的条件表达试
    //返回值::
    //返回类型                描述
    //const XMLCh*            if语句的条件表达试
	const XMLCh* getExpressionString() const;

    //析构函数
	virtual ~MyCaseStatement();

private:

    //初始化
	void initialize();                 

	//清理内存
    void cleanUp();

    //方法描述:设置if语句的条件表达试
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

    /**
    * Copy constructor
    */
	MyCaseStatement(const MyCaseStatement&);
    MyCaseStatement& operator=(const MyCaseStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif