#if !defined(MYRETURNPARSER_H)
#define MYRETURNPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarListContainer.h"
#include"match/My/MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyReturnParser  : public AbsBlockParser
{
public:
	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	MyReturnParser(const XMLCh *nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	~MyReturnParser();

	//方法描述:return块执行入口
    //返回值:   MyValue
    //返回类型                描述
    //MyValue*              MyValue的具体类型数据
    //参数说明:
    //参数名称          参数类型                    描述
	//varableList       MyVarListContainer*         变量链表
	void run(MyVarListContainer* varableList,_variant_t* result);

private:
	void initialize();                                             //初始化
    void cleanUp();                                                //清除工作

	//内存管理成员


	//CCF 拷贝构造函数
	MyReturnParser(const MyReturnParser& pool);
	MyReturnParser& operator=(const MyReturnParser& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif