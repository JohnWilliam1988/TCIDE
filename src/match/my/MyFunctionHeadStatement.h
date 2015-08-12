/////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成function语句的抽象类MyFunctionHeadStatement的设计
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYFUNCTIONHEADSTATEMENT_H
#define MYFUNCTIONHEADSTATEMENT_H

#include "match/My/MyValue.h"
#include "MyStatement.h"


MATCH_CPP_NAMESPACE_BEGIN
//我们这里 把函数定义的函数头定义语句 抽象成 mystatement 继承类
//MyFunctionHeadStatement 记录了函数定义的名字,形参列表,和函数返回类型
class PARSERS_EXPORT MyFunctionHeadStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //name           const XMLCh* const          函数名
    //pramList       const XMLCh* const          参数列表
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyFunctionHeadStatement(
		               const XMLCh* const    name
					  ,const XMLCh* const    pramList
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //方法描述:返回函数名
    //返回值::
    //返回类型                描述
    //const XMLCh*            函数名字符串
	const XMLCh* getFunctionNameString() const;

	//方法描述:返回函数的参数列表
    //返回值::
    //返回类型                       描述
    //const XMLCh*                   得到函数的参数列表
	const XMLCh* getFunctionPramString() const;

    //析构函数
	virtual ~MyFunctionHeadStatement();
private:
    //初始化
	void initialize();                 

	//清理内存
     void cleanUp();

     //方法描述:设置函数名
     //
     //返回值:
     //返回类型                描述
     //void                    无返回类型
     //
     //参数说明:
     //参数名称      参数类型                    描述
     //name          const XMLCh* const          函数名
	void setFunctionNameString(const XMLCh* const name);

     //方法描述:设置参数列表名
     //
     //返回值:
     //返回类型                描述
     //void                    无返回类型
     //
     //参数说明:
     //参数名称      参数类型                    描述
     //pramList          const XMLCh* const      函数名
	void setFunctionPramString(const XMLCh* const pramList);

	XMLCh             fFunctionName[NAMEMAXLENGTH];//记录函数名
	XMLCh*            fFunctionPram;               //记录函数参数列表
    /**
    * Copy constructor
    */
	MyFunctionHeadStatement(const MyFunctionHeadStatement&);
    MyFunctionHeadStatement& operator=(const MyFunctionHeadStatement&);
};



MATCH_CPP_NAMESPACE_END

#endif