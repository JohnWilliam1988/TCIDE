//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成function执行块的抽象类MyFunctionBlock的设计
//0002      2010-05-17  肖秀范      去掉了function结束语句的记录,因为它已经记录在块链表结束块中
//0003      2010-07-16  钟斌        在构造函数中添加了本块的行号和文件路径两个参数，addEndFunctionBlock方法中也添加了行号和文件路径两个参数
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYFUNCTIONBLOCK_H
#define MYFUNCTIONBLOCK_H

#include "BlockList.h"
#include "MyFunctionHeadStatement.h"

#include "MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

//我们把函数头的定义 抽象成 MyFunctionBlock
class PARSERS_EXPORT MyFunctionBlock :  public BaseBlock
{
public:
    //构造函数
    //参数说明:
    //参数名称           参数类型                    描述
    //parentBlock        BaseBlock *                 父块的指针
    //functionStatement  MyStatement *               function定义头语句
    //id                 const unsigned int          块的ID
    //manager            MemoryManager* const        内存管理器
	MyFunctionBlock(BaseBlock *  parentBlock
	         ,MyStatement * functionStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
	~MyFunctionBlock();                   

    //方法描述:得到函数块包含的块链表的头节点
    //返回值::
    //返回类型                    描述
    //const BaseBlock*            块链表的头节点
	const BaseBlock* GetStartBlock() const;

    //方法描述:向函数包含的块链表添加一个块
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //block         BaseBlock *                  执行块指针
    void addBlock(BaseBlock * block);


    //方法描述:添加函数结束语句,会在内部实例化一个函数结束块添到链表中
    //
    //返回值:
    //返回类型                描述
    //void                    无返回
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //end         MyStatement *                 函数结束语句指针
	//linenum	  const unsigned int            endfunction的行号
	//filepath    const XMLCh*                  文件的路径
	void addEndFunctionBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);          

    //方法描述:返回函数名
    //返回值::
    //返回类型                描述
    //const XMLCh*            函数名字符串
	const XMLCh* getFunctionNameString() const;

	//方法描述:返回函数的参数列表
    //返回值::
    //返回类型                       描述
    //const XMLCh*                   得到函数的参数列表
	const XMLCh* getFunctionPramString() const;


	//方法描述:返回函数块包含的执行块的链表
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

	bool isSetEnd;                               //此bool变量标识了是不是已经设置了Function结束块

	BaseBlock *  fParentBlock;                   //记录父块

	MyFunctionHeadStatement* fFunctionStatement; //Function块头

	BlockList *  fBlockList;                     //包含的块链表

    /**
    * Copy constructor
    */
	MyFunctionBlock(const MyFunctionBlock& node);
    MyFunctionBlock& operator=(const MyFunctionBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif