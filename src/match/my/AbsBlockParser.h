#if !defined(ABSBLOCKPARSER_H)
#define ABSBLOCKPARSER_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <match/framework/XMLErrorReporter.hpp>
#include <match/sax/ErrorHandler.hpp>
#include "BaseBlock.h"
#include "MyError.h"
#include <match/util/XMLUniDefs.hpp>
//钟斌 10-8-9 加入调试功能需要的变量列表的头文件
#include"MyVarListContainer.h"
//钟斌 10-8-9 加入调试功能需要的变量列表的头文件

MATCH_CPP_NAMESPACE_BEGIN

//执行块的抽象类
class PARSERS_EXPORT AbsBlockParser
{
public:
	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	AbsBlockParser(const XMLCh* const    nameSpace
		,const BaseBlock *  Block
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	
    //析构
    ~AbsBlockParser();

	//方法描述：得到命名空间
	//返回值:   XMLCh*
    //返回类型                描述
    //const XMLCh*            命名空间名
    //参数说明:
    //无
	const XMLCh* getSpaceName() const;
protected:

	//初始化
	void initialize();                              

	//清除工作
	void cleanUp();

	//方法描述：设置命名空间名
	//返回值:   无
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	void setSpaceName(const XMLCh* const nameSpace);

	//
    //方法描述：得到包含的块指针
	//返回值:   包含块的块指针
    //返回类型                描述
    //BaseBlock*              包含块的块指针
    //参数说明:
    //无
	BaseBlock* getBlock() const;

//调试版本会包含下面的方法
#ifdef TC_DEBUG
    //钟斌 10-8-9 加入判断状态的方法
    //方法描述： 判断调试状态。
    void debug(const XMLCh* const namesp, MyVarListContainer* VarList);
    //钟斌 10-8-9 加入判断状态的方法
#endif

	bool getIsStop() const;
	bool isGoto(const BaseBlock* startBlock,/*MyVarList * const VarList,MyVarListContainer* VarListCon,*/const BaseBlock** gotoBlock) const;

	MemoryManager*              fMemoryManager;	 	//内存管理成员
	BaseBlock *                 fBlock;             //解析的块,内存不是自己分配             
	XMLCh                       fNameSpace[NAMEMAXLENGTH];         //所在的命名空间 

	//CCF 拷贝构造函数
	AbsBlockParser(const AbsBlockParser& pool);
	AbsBlockParser& operator=(const AbsBlockParser& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif
