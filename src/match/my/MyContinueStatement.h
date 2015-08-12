//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成continue语句的抽象类MyContinueStatement的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCONTINUESTATEMENT_H
#define MYCONTINUESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//我们把 continue语句 抽象成 MyStatement 的继承类
class PARSERS_EXPORT MyContinueStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
	MyContinueStatement(const unsigned int    id = 0
		                  ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //析构函数
	virtual ~MyContinueStatement();

private:
    //初始化
	void initialize();                 

    //清楚变量内存
    void cleanUp();    

    /**
    * Copy constructor
    */
	MyContinueStatement(const MyContinueStatement&);
    MyContinueStatement& operator=(const MyContinueStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif