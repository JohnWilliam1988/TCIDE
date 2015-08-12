#if !defined(MYEXCUTOR_H)
#define MYEXCUTOR_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyGlobalVarList.h"
#include "MyBlockListPool.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyExcutor : public XMemory
{
public :
    static MemoryManager*              fMemoryManager;
    
    ////析构函数
    //virtual ~MyExcutor();

	//方法描述: 解析器的执行接口
	//返回值    void
    //参数说明:
    //参数名称      参数类型                    描述
	//无
    //static void execute();

    //方法描述: 解析器的执行接口
    //返回值    void
    //参数说明:
    //参数名称      参数类型                    描述
    //nameSpace         const XMLCh * const         要执行的命名空间
    //funcName          const XMLCh * const         要执行的函数名
    static bool execute(const XMLCh * const nameSpace,const XMLCh* const funcName);

	//方法描述: 获取全局变量链表
	//返回值    void
    //参数说明:
    //参数名称      参数类型                    描述
	//无
	static void getGlobalVarList() ; 

	//方法描述: 设置当前函数为main函数
	//返回值    模块指针
	//返回类型                描述
    //const BaseBlock *       main的块指针
    //参数说明:
    //参数名称      参数类型                    描述
	//无
    //static const BaseBlock * getMailFcuntion() ; 

    //
    //方法描述:初始化运行管理类
    //返回值:无
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称              参数类型                    描述
    //memoryManager         MemoryManager* const        内存管理成员
    static void initialize(MemoryManager* const memoryManager = XMLPlatformUtils::fgMemoryManager);

    //
    //方法描述:清理运行管理类
    //返回值:无
    //返回类型                描述
    //void                    无
    static void Terminate();
private:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    //构造函数
    //参数说明:
    //参数名称      参数类型                    描述
    //manager       MemoryManager* const        内存管理器
    MyExcutor();

	//MyExcutor(const MyExcutor& excutor);
	//MyExcutor& operator=(const MyExcutor& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif