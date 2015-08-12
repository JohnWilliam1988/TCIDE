/////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成endrepeat语句的抽象类MyEndRepeatStatement的设计
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDREPEATSTATEMENT_H
#define MYENDREPEATSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//我们这里把endrepeat语句抽象成 mystatement 抽象类
class PARSERS_EXPORT MyEndRepeatStatement : public MyStatement
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //id             const unsigned int          语句ID
    //manager        MemoryManager* const        内存管理器
    MyEndRepeatStatement(const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    virtual ~MyEndRepeatStatement();

private:
    //初始化
    void initialize();      

    //清楚变量内存 
    void cleanUp();                     

    /**
    * Copy constructor
    */
    MyEndRepeatStatement(const MyEndRepeatStatement&);
    MyEndRepeatStatement& operator=(const MyEndRepeatStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif