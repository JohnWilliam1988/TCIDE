//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成endif语句的抽象类MyEndIfStatement的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDIFSTATEMENT_H
#define MYENDIFSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//我们这里把endif语句抽象成 mystatement 继承类
class PARSERS_EXPORT MyEndIfStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyEndIfStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 

    //析构函数
	virtual ~MyEndIfStatement();

private:
    //初始化
	void initialize();                 

    //清理内存
	void cleanUp();                    

    /**
    * Copy constructor
    */
	MyEndIfStatement(const MyEndIfStatement&);
    MyEndIfStatement& operator=(const MyEndIfStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif