#if !defined(MYVARPARSER_H)
#define MYVARPARSER_H

#include"AbsBlockParser.h"
#include"BaseBlock.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyVarParser : public AbsBlockParser
{
public:
	//构造函数
    //参数说明:
    //参数名称      参数类型                    描述
	//nameSpace     const XMLCh* const          命名空间名
	//Block         const BaseBlock *           模块的指针
    //manager       MemoryManager* const        内存管理器
	MyVarParser(const XMLCh* const    nameSpace
		        ,const BaseBlock * Block
		        ,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //析构函数
	~MyVarParser();

	//方法描述:return块执行入口                                                                                    
    //返回值:   void
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称          参数类型                    描述
	//VarList           MyVarList*  const          变量链表
	void run(MyVarList * const VarList,MyVarListContainer* const listCon);

private:
    void initialize();                                             //初始化
    void cleanUp();                                                //清除工作

	//CCF 拷贝构造函数
	MyVarParser(const MyVarParser& pool);
	MyVarParser& operator=(const MyVarParser& toAssign);

};


MATCH_CPP_NAMESPACE_END



#endif