//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-17  肖秀范      完成endif语句执行块的抽象类MyEndIFBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDSELECTBLOCK_H
#define MYENDSELECTBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//此类用来标识一个IF执行块的结束块.
class PARSERS_EXPORT MyEndSelectBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称         参数类型                    描述
    //endIfStatement   MyStatement *               endif语句
    //id               const unsigned int          块的ID
    //manager          MemoryManager* const        内存管理器
    MyEndSelectBlock(MyStatement * endSelectStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyEndSelectBlock();
private:
    //初始化块
    void initialize();                

    //清理块
    void cleanUp();                   

    MyStatement* fEndSelectStatement;                     //endIf语句 

    /**
    * Copy constructor
    */
    MyEndSelectBlock(const MyEndSelectBlock& node);
    MyEndSelectBlock& operator=(const MyEndSelectBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif