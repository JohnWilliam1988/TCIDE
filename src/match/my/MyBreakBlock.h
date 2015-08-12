//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成break语句执行块的抽象类MyBreakBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYBREAKBLOCK_H
#define MYBREAKBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们这里把break 执行块 抽象成 BaseBlock 的继承类
class PARSERS_EXPORT MyBreakBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //breakState  MyStatement *                 break语句
    //id            const unsigned int          块的ID
    //manager       MemoryManager* const        内存管理器
    MyBreakBlock(MyStatement * breakState
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyBreakBlock();
private:
    //初始化块
    void initialize();        

    //清理
    void cleanUp();                    

    MyStatement* fBreakStatement;                           //break语句

    /**
    * Copy constructor
    */
    MyBreakBlock(const MyBreakBlock& node);
    MyBreakBlock& operator=(const MyBreakBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif