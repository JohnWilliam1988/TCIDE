#if !defined(MYCALLPARSER_H)
#define MYCALLPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"
#include"MyDataList.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyCallParser  : public AbsBlockParser
{
public:
	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	MyCallParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
	~MyCallParser();

    //方法描述:把实参转换为参数之后加入到包括链表
    //构造函数
    //返回值        void
    //参数说明:
    //参数名称      参数类型                    描述
    //nameSpace     const XMLCh* const          命名空间名
    //argumentStr   const XMLCh* const          实参字符串
    //varableList   MyVarListContainer*         变量链表
    //argumentList  MyDataList*                 变量值链表
    //Block         const BaseBlock *           模块指针
    //manager       MemoryManager* const        内存管理器,
    static void argumentTransportNoCheck(const XMLCh* nameSp, XMLCh * const argumentStr,MyVarListContainer* varableList,MyDataList* argumentList,const BaseBlock * Block,MemoryManager* const memoryManager = XMLPlatformUtils::fgMemoryManager);

	//方法描述: 用来处理函数赋值语句。
	//返回值    函数的返回值
    //返回类型                描述
    //MyValue*                具体类型的数据
    //参数说明:
    //参数名称      参数类型                    描述
	//varableList   MyVarListContainer*          变量链表指针
	 void run(MyVarListContainer* varableList,_variant_t* result);

	//方法描述: 用来处理直接调用函数的语句，不需要返回值
	//返回值        void
    //参数说明:
    //参数名称      参数类型                    描述
	//varableList   MyVarListContainer*          变量链表指针
	void  NoReturnrun(MyVarListContainer* varableList);

	static void ArrayGetMsg(const XMLCh* namesp,MyVarListContainer* varableList,MyDataList* list,MyValue* ret);

private:
	void initialize();                                             //初始化
    void cleanUp();                                                //清除工作

	//内存管理成员


	//CCF 拷贝构造函数
	MyCallParser(const MyCallParser& pool);
	MyCallParser& operator=(const MyCallParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif