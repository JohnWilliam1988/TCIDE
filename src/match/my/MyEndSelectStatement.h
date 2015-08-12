//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成endif语句的抽象类MyEndIfStatement的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDSELECTSTATEMENT_H
#define MYENDSELECTSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//我们这里把endif语句抽象成 mystatement 继承类
class PARSERS_EXPORT MyEndSelectStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyEndSelectStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 

    //析构函数
	virtual ~MyEndSelectStatement();

private:
    //初始化
	void initialize();                 

    //清理内存
	void cleanUp();                    

    /**
    * Copy constructor
    */
	MyEndSelectStatement(const MyEndSelectStatement&);
    MyEndSelectStatement& operator=(const MyEndSelectStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif