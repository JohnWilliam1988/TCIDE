//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-17  肖秀范      完成else语句执行块的抽象类MyElseBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYELSEBLOCK_H
#define MYELSEBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们这里是抽象的是 else 执行块
class PARSERS_EXPORT MyElseBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称       参数类型                    描述
    //parentBlock    BaseBlock *                 父块的指针
    //elseStatement  MyStatement *               else语句
    //id             const unsigned int          块的ID
    //manager        MemoryManager* const        内存管理器
    MyElseBlock(BaseBlock * parentBlock
        ,MyStatement * elseStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyElseBlock();

    //方法描述:得到else块包含的块链表的头节点
    //返回值::
    //返回类型                    描述
    //const BaseBlock*            块链表的头节点
    const BaseBlock* GetStartBlock() const;

    //方法描述:向else块包含的块链表添加一个块
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //block         BaseBlock *                 执行块指针
    void addBlock(BaseBlock * block);

    //方法描述:添加else结束语句,会在内部实例化一个else结束块添到链表中
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //end         MyStatement *                 if结束语句指针
    //linenum	  const unsigned int            endif的行号
    //filepath    const XMLCh*                  文件的路径
    void addEndIfBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);

    //方法描述:返回else块包含的执行块的链表
    //返回值::
    //返回类型                       描述
    //const BlockList*               块链表指针
    const BlockList * GetBlockList() const;

    //方法描述:得到父块
    //返回值::
    //返回类型                描述
    //const BaseBlock*        父块指针
    const BaseBlock* GetParentBlock() const;

    //方法描述:设置已经到了endif结束,这里因为是else语句,所以只做是否已经到了结尾,不需要记录结束块
    //返回值::
    //返回类型                描述
    //void                    无
    void setIfEnd();

private:
    //初始化块
    void initialize();                 

    //清理块
    void cleanUp();                   

    bool isSetEnd;                      //此bool变量标识了是不是已经设置了IF结束语句

    MyStatement* fElseStatement;         //else语句

    BlockList *  fBlockList;             //包含的块链表

    BaseBlock *  fParentBlock;           //记录父块

    /**
    * Copy constructor
    */
    MyElseBlock(const MyElseBlock& node);
    MyElseBlock& operator=(const MyElseBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif