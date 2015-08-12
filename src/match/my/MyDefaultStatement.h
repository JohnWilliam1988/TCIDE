/////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成Default语句的抽象类MyDefaultStatement的设计
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYDEFAULTSTATEMENT_H
#define MYDEFAULTSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//我们把if语句 抽象成 mystatement 继承类
class PARSERS_EXPORT MyDefaultStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                     描述
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyDefaultStatement(const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //析构函数
	virtual ~MyDefaultStatement();

private:

    //初始化
	void initialize();                 

	//清理内存
    void cleanUp();

    /**
    * Copy constructor
    */
	MyDefaultStatement(const MyDefaultStatement&);
    MyDefaultStatement& operator=(const MyDefaultStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif