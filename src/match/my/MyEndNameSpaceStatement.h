/////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成endnamespace语句的抽象类MyEndNameSpaceStatement的设计
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDNAMESPACESTATEMENT_H
#define MYENDNAMESPACESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//我们这里把endnamespace 抽象成mystatement 继承类
class PARSERS_EXPORT MyEndNameSpaceStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyEndNameSpaceStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //析构函数
	virtual ~MyEndNameSpaceStatement();

private:
    //初始化
	void initialize();                 

    //清理内存
    void cleanUp();                    

    /**
    * Copy constructor
    */
	MyEndNameSpaceStatement(const MyEndNameSpaceStatement&);
    MyEndNameSpaceStatement& operator=(const MyEndNameSpaceStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif