#if !defined(MYREPEATPARSER_H)
#define MYREPEATPARSER_H

#include"AbsBlockParser.h"
#include"MyWhileParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"MyValue.h"
#include"MyDataList.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyRepeatParser  : public AbsBlockParser
{
public:
	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	MyRepeatParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyRepeatParser();

	//方法描述:执行repeat功能块
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
    //helpBlock         const BaseBlock *           当前模块
	//varableList       MyVarListContainer*         变量链表
	//isFindReturn      bool                        return标识，有return就为真，否则为假
	void ProcessRepeatBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

	//方法描述:repeat块执行入口
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
	//varableList       MyVarListContainer*         变量链表
	//isFindReturn      bool                        return标识，有return就为真，否则为假
	void run(MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

private:
	void initialize();                                             //初始化
    void cleanUp();                                                //清除工作

	MyVarList      fOwnvarlist;   

	LoopReturnType  fReturnType;

	//CCF 拷贝构造函数
	MyRepeatParser(const MyRepeatParser& pool);
	MyRepeatParser& operator=(const MyRepeatParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif