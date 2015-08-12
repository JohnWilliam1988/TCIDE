#if !defined(MYGOTOPARSER_H)
#define MYGOTOPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyValue.h"
#include"MyDataList.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN


//goto执行块的抽象类,继承于AbsBlockParser
class PARSERS_EXPORT MyGotoParser : public AbsBlockParser
{

public:

	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	MyGotoParser(const XMLCh * const nameSpace,
		             const BaseBlock * Block,
		             MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
	~MyGotoParser();

	//方法描述: goto执行接口。
	//返回值    无
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称      参数类型                    描述
	//varableList   MyVarListContainer*         实参变量链表指针
	void run(MyVarListContainer* varableList);
private:
    void initialize();   //初始化
    void cleanUp();     //清除工作


	MyVarList  fOwnVarList;                            //用来存放函数快的局部变量

	//CCF 拷贝构造函数
	MyGotoParser(const MyGotoParser& pool);
	MyGotoParser& operator=(const MyGotoParser& toAssign);
};


MATCH_CPP_NAMESPACE_END



#endif