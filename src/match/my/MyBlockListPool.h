//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-16  肖秀范      完成块管理池类MyBlockListPool的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYBLOCKLISTPOOL_H
#define MYBLOCKLISTPOOL_H

#include "BlockList.h"
#include "MyNameSpaceBlock.h"

MATCH_CPP_NAMESPACE_BEGIN


//块管理池的节点
struct PARSERS_EXPORT ListMgr
{
	MyNameSpaceBlock * nameSpace; //命名空间
	ListMgr * pre;                //上一个
	ListMgr * next;               //下一个

    //用构造函数来初始化 
    ListMgr()
    {
        nameSpace=NULL;
        pre=NULL;
        next=NULL;
    }
};

//链表池,链表池是用来存储块链表的,主要是在解析源文件的时候,会把源文件所有的源代码翻译成执行块,
//然后执行块都组合成块链表,然后把块链表存放在这个链表池中

class PARSERS_EXPORT MyBlockListPool : public XMemory
{
public:
	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //manager       MemoryManager* const        内存管理器
	MyBlockListPool(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//析构函数
	~MyBlockListPool();

	//链表初始化
	void initialize();

    //方法描述:添加一个命名空间到块池中
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
    //
    //参数说明:
    //参数名称       参数类型                    描述
    //toAdd          ListMgr *                   节点指针
	void addListToPool(ListMgr * toAdd);

	//
    //方法描述:销毁链表池
    //
    //返回值:
    //返回类型                描述
    //void                    无返回类型
	void delListPool();


    //方法描述:得到链表池头
    //返回值::
    //返回类型                描述
    //const ListMgr*          头指针
	const ListMgr* getPoolHead() const;

	//
    //方法描述:通过空间名查找链表节点
    //
    //返回值:
    //返回类型                描述
    //ListMgr *               成功:返回查到的节点指针. 失败:返回NULL
    //
    //参数说明:
    //参数名称      参数类型                    描述
    //toFind          const XMLCh* const        空间名
	ListMgr * findListByNamespce(const XMLCh * const  toFind);

protected:

private:
	MemoryManager*      fMemoryManager;	            //内存管理成员
	ListMgr *              fListHead;               //链表池头节点
	ListMgr *              fListEnd;                //链表池尾节点

	//链表池销毁
	void cleanUp();	

    /**
    * Copy constructor
    */
	MyBlockListPool(const MyBlockListPool& pool);
	MyBlockListPool& operator=(const MyBlockListPool& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif 























