#if !defined(MYIFPARSER_H)
#define MYIFPARSER_H

#include "MyDataList.h"

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"

#include "MyWhileParser.h"


MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyIfParser  : public AbsBlockParser
{
public:
	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	MyIfParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyIfParser();

	//方法描述:查找else块
    //返回值:找到的块
    //返回类型                描述
    //const BaseBlock *       返回找到的块,返回 NULL,表示失败
    //参数说明:
    //参数名称          参数类型                    描述
    //helpBlock         const BaseBlock *           要查找的模块
	const BaseBlock* FindElseSta(const BaseBlock * helpBlock) const;

	//方法描述:执行if块
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
    //helpBlock         const BaseBlock *           当前模块
	//varableList       MyVarListContainer*         变量链表
	//isFindReturn      bool                        return标识，有return就为真，否则为假
	//isFindLoopReturn  LoopReturnType              特殊控制
	void ProcessIfBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,LoopReturnType &isFindLoopReturn,_variant_t* result);

	//方法描述:执行else块
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
    //helpBlock         const BaseBlock *           当前模块
	//varableList       MyVarListContainer*         变量链表
	//isFindReturn      bool                        return标识，有return就为真，否则为假
	//isFindLoopReturn  LoopReturnType              特殊控制
	void ProcessElseBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,LoopReturnType &isFindLoopReturn,_variant_t* result);

	//方法描述:if块执行入口
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
	//varableList       MyVarListContainer*         变量链表
	//isFindReturn      bool                        return标识，有return就为真，否则为假
	//isFindLoopReturn  LoopReturnType              特殊控制
	void run(MyVarListContainer* varableList,bool &isFindReturn,LoopReturnType &isFindLoopReturn,_variant_t* result);



	//下面是三个重载函数,主要用来在function块中调用的

	//方法描述:执行if块
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
    //helpBlock         const BaseBlock *           当前模块
	//varableList       MyVarListContainer*         变量链表
	//isFindReturn      bool                        return标识，有return就为真，否则为假
	void ProcessIfBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

	//方法描述:执行else块
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
    //helpBlock         const BaseBlock *           当前模块
	//varableList       MyVarListContainer*         变量链表
	//isFindReturn      bool                        return标识，有return就为真，否则为假
	void ProcessElseBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);


	//方法描述:if块执行入口
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
	//varableList       MyVarListContainer*         变量链表
	//isFindReturn      bool                        return标识，有return就为真，否则为假
	void run(MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

private:
	void initialize();                         //初始化
    void cleanUp();                            //清除工作

	MyVarList       fOwnvarlist;               //局部变量列表

	//CCF 拷贝构造函数
	MyIfParser(const MyIfParser& pool);
	MyIfParser& operator=(const MyIfParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif