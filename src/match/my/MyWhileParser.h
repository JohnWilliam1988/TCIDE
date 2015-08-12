#if !defined(MYWHILEPARSER_H)
#define MYWHILEPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"
#include "MyDataList.h"

MATCH_CPP_NAMESPACE_BEGIN

//此枚举类型主要用来处理循环体内部的执行块的break和continue返回的情况
//主要用在循环操作里面,目前主要用在框架支持的repeat和while块里面
//
enum LoopReturnType
{
	LOOP_BREAK,
	LOOP_CONTINUE,
	LOOP_NORMAL,
};

class PARSERS_EXPORT MyWhileParser  : public AbsBlockParser
{
public:

	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	MyWhileParser(const XMLCh * const nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyWhileParser();

	//方法描述:执行while功能块
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*                MyValue的具体类型数据
    //参数说明:
    //参数名称                 参数类型                     描述
    //helpBlock               const BaseBlock *           当前模块
	//varableList             MyVarListContainer*         变量链表
	//isFindReturn            bool                        return标识,有return就为真,否则为假
	void ProcessWhileBlock(const BaseBlock * helpBlock,MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

	//方法描述:while块执行入口
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*                MyValue的具体类型数据
    //参数说明:
    //参数名称                参数类型                    描述
	//varableList             MyVarListContainer*         变量链表
	//isFindReturn            bool                        return标识，有return就为真，否则为假
	void run(MyVarListContainer* varableList,bool &isFindReturn,_variant_t* result);

private:
	void initialize();                                             //初始化
    void cleanUp();                                                //清除工作

	MyVarList          fOwnvarlist;                  //局部变量列表
	LoopReturnType     fReturnType;                  //while语句中断的类型,break,continue,

	//CCF 拷贝构造函数
	MyWhileParser(const MyWhileParser& pool);
	MyWhileParser& operator=(const MyWhileParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif