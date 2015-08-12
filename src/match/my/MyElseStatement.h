//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成else语句的抽象类MyElseStatement的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYELSESTATEMENT_H
#define MYELSESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们把else语句 抽象成 mystatement 继承类
class PARSERS_EXPORT MyElseStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyElseStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 


    //析构函数
	virtual ~MyElseStatement();

private:

	void initialize();                 //初始化

    void cleanUp();                    	//清楚变量内存

	MyElseStatement(const MyElseStatement&);
    MyElseStatement& operator=(const MyElseStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif