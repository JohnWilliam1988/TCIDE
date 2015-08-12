//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成块链表管理类BlockList的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "BaseBlock.h"

MATCH_CPP_NAMESPACE_BEGIN

//块链表,块链表的作用主要是用来存储一系列执行块的,
class PARSERS_EXPORT BlockList : public XMemory
{
public:
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //manager       MemoryManager* const        内存管理器
	BlockList(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//析构函数
	~BlockList();	

    //方法描述:返回块链表的块头的指针
    //返回值::
    //返回类型                描述
    //const BaseBlock *       返回块的指针
	const BaseBlock * GetTheStartBlock() const;	

    //方法描述:返回块链表的块尾的指针
    //返回值::
    //返回类型                描述
    //const BaseBlock *       返回块的指针
	const BaseBlock * GetTheEndBlock() const;	

	//
    //方法描述:添加一个块到链表中去
    //返回值::
    //返回类型                描述
    //void                    无
    //
    //参数说明:
    //参数名称       参数类型              描述
    //block          BaseBlock*            块指针
	void AddBlockToList(BaseBlock*  block);

private:
	//链表初始化
	void initialize();

    //清理内存
	void cleanUp();

	BaseBlock*        fHead;                  //链表头
	BaseBlock*        fEnd;                   //链表尾
	unsigned int      fIDIndex;               //链表个数
	MemoryManager*    fMemoryManager;         //内存管理

    /**
    * Copy constructor
    */
	BlockList(const BlockList& node);
	BlockList& operator=(const BlockList& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif