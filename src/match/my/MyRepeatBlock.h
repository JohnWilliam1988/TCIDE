//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成repeat执行块的抽象类MyRepeatBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数，addEndRepeatBlock方法中也添加了行号和文件路径两个参数
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYREPEATBLOCK_H
#define MYREPEATBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"
#include "MyRepeatStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//repeat执行块的抽象
class PARSERS_EXPORT MyRepeatBlock : public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称           参数类型                    描述
    //parentBlock        BaseBlock *                 父块的指针
    //repeatStatement    MyStatement *               repeat定义头语句
    //id                 const unsigned int          块的ID
    //manager            MemoryManager* const        内存管理器
    MyRepeatBlock(BaseBlock * parentBlock
        ,MyStatement * repeatStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
    ~MyRepeatBlock();  

    //方法描述:得到repeat块包含的块链表的头节点
    //返回值::
    //返回类型                    描述
    //const BaseBlock*            块链表的头节点
    const BaseBlock* GetStartBlock() const;

    //方法描述:向repeat块包含的块链表添加一个块
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //block         BaseBlock *                 执行块指针
    void addBlock(BaseBlock * block);

    //方法描述:添加repeat结束语句,会在内部实例化一个repeat结束块添到链表中
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //end         MyStatement *                 repeat结束语句指针
	//linenum	  const unsigned int            endrepeat的行号
	//filepath    const XMLCh*                  文件的路径
    void addEndRepeatBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);

    //方法描述:返回repeat包含的链表块
    //返回值::
    //返回类型                       描述
    //const BlockList*               块链表指针
    const BlockList * GetBlockList() const;

    //方法描述:得到父块
    //返回值::
    //返回类型                描述
    //const BaseBlock*        父块指针
    const BaseBlock* GetParentBlock() const;

    //
    //方法描述:返回repeat块的循环的次数(一个非负的整数)
    //返回值::
    //返回类型                       描述
    //unsigned int                   repeat循环的次数
    //unsigned int getRepeatNum() const;
	bool isContant();

	unsigned int getContantNum();

	const XMLCh* getVarName();

	const XMLCh* getInitValue();

	const XMLCh* getExp();

	const XMLCh* getStep();

private:
    //初始化块
    void initialize();                 

    //清理块
    void cleanUp();                   

    bool isSetEnd;                     //此bool变量标识了是不是已经设置了repeat结束语句

    BaseBlock *       fParentBlock;    //记录父块

    MyRepeatStatement* fRepeatStatement;         //repeat语句

    /*MyStatement* fEndRepeatStatement;      //endrepeat语句*/

    BlockList *  fBlockList;            //包含的块链表

    /**
    * Copy constructor
    */
    MyRepeatBlock(const MyRepeatBlock& node);
    MyRepeatBlock& operator=(const MyRepeatBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif