//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成call表示试语句的抽象类MyCallStatement的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYGOTOSTATEMENT_H
#define MYGOTOSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们这里把 goto函数调用表达试 抽象成 MyStatement 的继承类
//goto函数调用表达试 主要是用来 调用 本地API和用户API
class PARSERS_EXPORT MyGotoStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //callExpress    const XMLCh* const          函数调用表达试
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyGotoStatement(  const XMLCh* const    gotoLable
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //方法描述:返回函数调用表达试的字符串
    //返回值::
    //返回类型                描述
    //const XMLCh*            函数调用表达试字符串
	const XMLCh* getGotolable() const;

    //析构函数
	virtual ~MyGotoStatement();

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
	void setGotolable(const XMLCh* const pram);

    XMLCh            fLable[NAMEMAXLENGTH];          //调用的表达试

    /**
    * Copy constructor
    */
	MyGotoStatement(const MyGotoStatement&);
    MyGotoStatement& operator=(const MyGotoStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif