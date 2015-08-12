//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成while执行块的抽象类MyWhileBlock的设计
//0002      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数，addEndWhileBlock方法中也添加了行号和文件路径两个参数
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYWHILEBLOCK_H
#define MYWHILEBLOCK_H

#include "BlockList.h"
#include "MyWhileStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//这里是抽象的 while 执行块
class PARSERS_EXPORT MyWhileBlock : public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称           参数类型                    描述
    //parentBlock        BaseBlock *                 父块的指针
    //whileStatement     MyStatement *               while定义语句
    //id                 const unsigned int          块的ID
    //manager            MemoryManager* const        内存管理器
	MyWhileBlock(BaseBlock * parentBlock
		        ,MyStatement * whileStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构
	~MyWhileBlock();                   

    //方法描述:这里是返回while块包含的块链表的头
    //返回值::
    //返回类型                    描述
    //const BaseBlock*            块链表的头节点
	const BaseBlock* GetStartBlock() const;

    //方法描述:向while块包含的块链表添加一个块
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //block         BaseBlock *                 执行块指针
	void addBlock(BaseBlock * block);

    //方法描述:添加while结束语句,会在内部实例化一个while结束块添到链表中
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //end         MyStatement *                 while结束语句指针
	//linenum	  const unsigned int            endwhile的行号
	//filepath    const XMLCh*                  文件的路径
	void addEndWhileBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);

    //方法描述:返回while块包含的执行块的链表
    //返回值::
    //返回类型                       描述
    //const BlockList*               块链表指针
    const BlockList * GetBlockList() const;

    //方法描述:得到父块
    //返回值::
    //返回类型                描述
    //const BaseBlock*        父块指针
	const BaseBlock* GetParentBlock() const;

    //方法描述:返回while的条件表达试
    //返回值::
    //返回类型                       描述
    //const XMLCh*                   条件表达试
	const XMLCh* getExpressionString() const;

private:
    //初始化块
	void initialize();                 

    //清理块
	void cleanUp();                    

	bool isSetEnd;                          //此bool变量标识了是不是已经设置了While结束语句

	BaseBlock *  fParentBlock;              //记录父块

	MyWhileStatement* fWhileStatement;           //Function块头语句 

	/*MyStatement* fEndWhileStatement;      //Function块尾语句*/

    BlockList *  fBlockList;                //包含的块链表
 

	MyWhileBlock(const MyWhileBlock& node);
    MyWhileBlock& operator=(const MyWhileBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif