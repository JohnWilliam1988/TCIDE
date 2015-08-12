//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成call表示试语句的抽象类MyCallStatement的设计
////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYLABLESTATEMENT_H
#define MYLABLESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们这里把 call函数调用表达试 抽象成 MyStatement 的继承类
//call函数调用表达试 主要是用来 调用 本地API和用户API
class PARSERS_EXPORT MyLableStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //lable          const XMLCh* const          函数调用表达试
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyLableStatement(  const XMLCh* const     lable
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //方法描述:返回函数调用表达试的字符串
    //返回值::
    //返回类型                描述
    //const XMLCh*            函数调用表达试字符串
	const XMLCh* getLable() const;

    //析构函数
	virtual ~MyLableStatement();

private:
    //初始化
	void initialize();                 

    //清楚变量内存
    void cleanUp();                    	

    //方法描述:设置函数调用表达试的字符串
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //pram          const XMLCh* const          函数调用表达试的字符串
	void setLable(const XMLCh* const pram);

	XMLCh            fLable[NAMEMAXLENGTH];          //调用的表达试

    /**
    * Copy constructor
    */
	MyLableStatement(const MyLableStatement&);
    MyLableStatement& operator=(const MyLableStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif