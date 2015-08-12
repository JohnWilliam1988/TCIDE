//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成break表示试语句的抽象类MyBreakStatement的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYBREAKSTATEMENT_H
#define MYBREAKSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//这里我们把 break语句 抽象成 MyStatement 的继承类
class PARSERS_EXPORT MyBreakStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //id            const unsigned int          语句ID
    //manager       MemoryManager* const        内存管理器
	MyBreakStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 

    //析构函数
	virtual ~MyBreakStatement();

private:
    //初始化
	void initialize();                

    //清楚变量内存
    void cleanUp();     

    /**
    * Copy constructor
    */
	MyBreakStatement(const MyBreakStatement&);
    MyBreakStatement& operator=(const MyBreakStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif