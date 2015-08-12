//////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-05-15  肖秀范      完成include预处理管理类DependBlockList的设计
////////////////////////////////////////////////////////////////////////////////////////

#ifndef DEPENDBLOCKLIST_H
#define DEPENDBLOCKLIST_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "BaseBlock.h"
#include "windows.h"

MATCH_CPP_NAMESPACE_BEGIN

const unsigned int MAXHEADFILELENGTH=256;

//DependBlockList只能装includeBlock块,主要是用来记录此项目的依赖文件
class PARSERS_EXPORT DependBlockList : public XMemory
{
public:
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //manager       MemoryManager* const        内存管理器
	DependBlockList(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//析构函数
	~DependBlockList();	

    //方法描述:返回块链表的块头的指针
    //返回值::
    //返回类型                描述
    //const BaseBlock *       返回块的指针
	const BaseBlock * GetTheStartBlock() const;	

	//
    //方法描述:添加一个include块到链表中去
    //返回值::
    //返回类型                描述
    //void                    无
    //
    //参数说明:
    //参数名称       参数类型              描述
    //block          BaseBlock*            块指针
	void addBlockToList(BaseBlock*  block);


	//
    //方法描述:添加一个头文件到链表中去
    //返回值::
    //返回类型                描述
    //void                    无
    //
    //参数说明:
    //参数名称       参数类型              描述
    //file           XMLCh* include       文件路径
	void addIncludeBlockToList(const XMLCh * const file);


	//
    //方法描述:添加一个com组件的导入块到链表中去
    //返回值::
    //返回类型                描述
    //void                    无
    //
    //参数说明:
    //参数名称       参数类型              描述
    //progID        XMLCh* include       prog的ID
	//ns            XMLCh* include       组件的空间名
	void addImportBlockToList(const XMLCh * const progID,const XMLCh * const ns,const XMLCh * const file);

	//
    //方法描述:添加一个include块到链表中去
    //返回值::
    //返回类型                描述
    //bool                    真表示:已存在,假表示不存在
    //
    //参数说明:
    //参数名称       参数类型              描述
    //include          XMLCh* include      包含的文件路径
	bool isExistIncludeByPath(const XMLCh * const include);

	//
    //方法描述:判断一个import是否存在
    //返回值::
    //返回类型                描述
    //bool                    真表示:已存在,假表示不存在
    //
    //参数说明:
    //参数名称       参数类型              描述
    //import          XMLCh*              com导入
	bool isExistImport(const XMLCh * const id,const XMLCh * const ns);


	//方法描述:添加一个头文件名到链表中去
    //返回值::
    //返回类型                描述
    //void                    无
    //
    //参数说明:
    //参数名称       参数类型              描述
    //scannerFile   const XMLCh * const   块指针
	//bool addFileToArray(const XMLCh * const scannerFile);

	//
    //方法描述:添加一个include块到链表中去
    //返回值::
    //返回类型                描述
    //bool                    真表示:已存在,假表示不存在
    //
    //参数说明:
    //参数名称       参数类型              描述
    //include          XMLCh* include      包含的文件路径
	//bool isExistScannerFile(const XMLCh * const include);

	//
    //方法描述:从空间名得到com id
	//返回值:找到的块
	//返回类型                描述
	//bool                   返回真 表示ID存在,返回假L 表示ID不存在
	//参数说明:
	//参数名称          参数类型                    描述
	//nameSpace        const XMLCh * const         命名空间
	//comID             XMLCh *                    comID
    bool getComIDByNs(const XMLCh * const nameSpace,XMLCh* comID);


	 //清理内存
	void cleanUp();

private:

	//链表初始化
	void initialize();

   

	BaseBlock*        fHead;                  //块头
	BaseBlock*        fEnd;                   //块尾
	unsigned int      fIDIndex;               //块的节点数
	MemoryManager*    fMemoryManager;         //内存管理

	//unsigned int      fScannerFileLength;      //块的节点数
	//XMLCh             fHeadFileList[MAXHEADFILELENGTH][MAX_PATH];

    /**
    * Copy constructor
    */
	DependBlockList(const DependBlockList& node);
	DependBlockList& operator=(const DependBlockList& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif