#if !defined(MYASSIGNMENTPARSER_H)
#define MYASSIGNMENTPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarList.h"
#include"MyVarListContainer.h"

MATCH_CPP_NAMESPACE_BEGIN

//负责解析赋值表达试
class PARSERS_EXPORT MyAssignmentParser : public AbsBlockParser
{
public:
	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	MyAssignmentParser(const XMLCh * const nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	~MyAssignmentParser();

	static void initializeStaticData();                                             //初始化

	//方法描述: 赋值语句的执行功能
	//返回值    无
    //参数说明:
    //参数名称      参数类型                    描述
	//VarList     MyVarListContainer*          变量链表指针
	void run(MyVarList * const VarList,MyVarListContainer* VarListCon);

	//方法描述: 赋值语句GOTO的执行部分
	//返回值    无
    //参数说明:
    //参数名称      参数类型                    描述
	//VarList     MyVarListContainer*          变量链表指针
	void gotoRun(MyVarList * const VarList,MyVarListContainer* VarListCon);

private:
    void initialize();                                             //初始化
    void cleanUp();                                                //清除工作
	
	//CCF 拷贝构造函数
	MyAssignmentParser(const MyAssignmentParser& pool);
	MyAssignmentParser& operator=(const MyAssignmentParser& toAssign);
};


MATCH_CPP_NAMESPACE_END



#endif