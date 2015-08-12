//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成endfunction语句的抽象类MyEndFunctionStatement的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDFUNCTIONSTATEMENT_H
#define MYENDFUNCTIONSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//我们这里把endfunction语句抽象成 mystatement 继承类
class PARSERS_EXPORT MyEndFunctionStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyEndFunctionStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //析构函数
	virtual ~MyEndFunctionStatement();

private:

    //初始化
	void initialize();                 

    //清理内存
    void cleanUp();      

	MyEndFunctionStatement(const MyEndFunctionStatement&);
    MyEndFunctionStatement& operator=(const MyEndFunctionStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif