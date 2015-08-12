//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成namespace执行块的抽象类MyNameSpaceBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数，addEndNameSpaceBlock方法中也添加了行号和文件路径两个参数
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYNAMESPACEBLOCK_H
#define MYNAMESPACEBLOCK_H

#include "BlockList.h"
#include "MyNameSpaceStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//namespace命名空间抽象类
class PARSERS_EXPORT MyNameSpaceBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称         参数类型                    描述
    //NSStatement      MyStatement *               namespace语句
    //id               const unsigned int          块的ID
    //manager          MemoryManager* const        内存管理器
    MyNameSpaceBlock(MyStatement * NSStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyNameSpaceBlock();                   

    //方法描述:得到namespace块包含的块链表的头节点
    //返回值::
    //返回类型                    描述
    //const BaseBlock*            块链表的头节点
    const BaseBlock* GetStartBlock() const;

    //方法描述:向namespace块包含的块链表添加一个块
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //block         BaseBlock *                 执行块指针
    void addBlock(BaseBlock * block);

    //方法描述:添加namespace结束语句,会在内部实例化一个namespace结束块添到链表中
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //end         MyStatement *                 namespace结束语句指针
	//linenum	  const unsigned int            endnamespace的行号
	//filepath    const XMLCh*                  文件的路径
    void addEndNameSpaceBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);

    //方法描述:得到命名空间名称
    //返回值::
    //返回类型                       描述
    //const XMLCh*                   条件表达试
    const XMLCh* getSapceNameString() const;

    //方法描述:返回namespace块包含的执行块的链表
    //返回值::
    //返回类型                       描述
    //const BlockList*               块链表指针
    const BlockList * GetBlockList() const;
private:
    //初始化块
    void initialize();                           

    //清理块
    void cleanUp();                              

    bool isSetEnd;                                //此bool变量标识了是不是已经设置了namespace结束语句
    MyNameSpaceStatement* fNameSpaceStatement;             //namespace块头
    /*MyStatement* fEndNameSpaceStatement;        //namespace块尾*/
    BlockList *  fBlockList;                      //包含的块链表

    /**
    * Copy constructor
    */
    MyNameSpaceBlock(const MyNameSpaceBlock& node);
    MyNameSpaceBlock& operator=(const MyNameSpaceBlock& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif

