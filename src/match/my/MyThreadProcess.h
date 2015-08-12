//这里是框架内部的线程处理
#if !defined(MYTHREADPROCESS_H)
#define MYTHREADPROCESS_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyGlobalVarList.h"
#include "MyBlockListPool.h"

MATCH_CPP_NAMESPACE_BEGIN

class TC_THREAD_INFOR
{
public:
	XMLCh *pFun;
	XMLCh *pInitArg;
	XMLCh *pFunDestory;
	XMLCh *pDesArg;
public:
	TC_THREAD_INFOR()
	{
		pFun=NULL;
		pFunDestory=NULL;
		pInitArg=NULL;
		pDesArg=NULL;
	}
	~TC_THREAD_INFOR()
	{
		if(pFun)
			XMLString::release(&pFun);
		if(pFunDestory)
			XMLString::release(&pFunDestory);
		if(pInitArg)
			XMLString::release(&pInitArg);
		if(pDesArg)
			XMLString::release(&pDesArg);
	}
};

//线程启动控制类
class PARSERS_EXPORT MyThreadProcess
{
public:
    MyThreadProcess(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    ~MyThreadProcess(void);

    void ThreadfunRun(const XMLCh *funcname,const XMLCh *arg);

    static void initialize();

    static void cleanUp();

    static int BeginThread(const XMLCh *const function,const XMLCh *const arg);

    static int BeginThreadEx(const XMLCh *const function,const XMLCh *const initArg,const XMLCh *const funDestory,const XMLCh *const desArg);

    static unsigned __stdcall MatchThreadFunc(void* pArguments);

	static unsigned __stdcall MatchThreadFuncEx(void* pArguments);

private:
    const BaseBlock *findThreadfun(const XMLCh* namesp,const XMLCh *funcname,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    MemoryManager*      fMemoryManager;      //内存管理器
};

MATCH_CPP_NAMESPACE_END

#endif
