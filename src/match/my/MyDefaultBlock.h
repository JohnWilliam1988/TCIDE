//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成if执行块的抽象类MySelectBlock的设计
//0002      2010-05-17  肖秀范      去掉if执行快的结束语句的记录,因为它已经记录在块链表的结束块中了
//0003      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数，addEndIfBlock方法中也添加了行号和文件路径两个参数
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYDEFAULTBLOCK_H
#define MYDEFAULTBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyDefaultBlock : public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称           参数类型                    描述
    //parentBlock        BaseBlock *                 父块的指针
    //ifStatement        MyStatement *               if定义头语句
    //id                 const unsigned int          块的ID
    //manager            MemoryManager* const        内存管理器
    MyDefaultBlock(BaseBlock * parentBlock
        ,MyStatement * selectStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyDefaultBlock();                   

    //方法描述:得到if块包含的块链表的头节点
    //返回值::
    //返回类型                    描述
    //const BaseBlock*            块链表的头节点
    const BaseBlock* GetStartBlock() const;

    //方法描述:向if块包含的块链表添加一个块
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //block         BaseBlock *                 执行块指针
    void addBlock(BaseBlock * block);

    //方法描述:向default块包含的块链表添加一个块
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                     描述
    void addEndBlock();

    //方法描述:返回if块包含的执行块的链表
    //返回值::
    //返回类型                       描述
    //const BlockList*               块链表指针
    const BlockList * GetBlockList() const;

    //方法描述:得到父块
    //返回值::
    //返回类型                描述
    //const BaseBlock*        父块指针
    const BaseBlock* GetParentBlock() const;
private:
    //初始化块
    void initialize();                 
    //清理块
    void cleanUp();                    

    bool isSetEnd;                     //此bool变量标识了是不是已经设置了IF结束语句

    MyStatement* fSelectStatement;      //IF块头

    BlockList *  fBlockList;           //包含的块链表

    BaseBlock *  fParentBlock;         //记录父块

    /**
    * Copy constructor
    */
    MyDefaultBlock(const MyDefaultBlock& node);
    MyDefaultBlock& operator=(const MyDefaultBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif