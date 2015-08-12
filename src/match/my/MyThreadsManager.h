//这里是框架内部的线程处理
#if !defined(_MYTHREADMANAGER_H_)
#define _MYTHREADMANAGER_H_

#include <match/util/MatchDefs.hpp>
#include <windows.h>

MATCH_CPP_NAMESPACE_BEGIN

//线程管理节点
struct CMyThreadsNode
{
public:
	XMLCh                fFunDestory[NAMEMAXLENGTH];
	XMLCh                fFunArg[VALUEMAXLENGTH];
	XMLCh                fLable[NAMEMAXLENGTH];
	bool                 fIsGoto;
    HANDLE               fHandle;
    DWORD                fThreadID;
    bool                 fStop;
    CMyThreadsNode()
	{
		::memset(fFunArg,0,VALUEMAXLENGTH*sizeof(XMLCh));
		::memset(fFunDestory,0,NAMEMAXLENGTH*sizeof(XMLCh));
		::memset(fLable,0,NAMEMAXLENGTH*sizeof(XMLCh));
		fIsGoto=false;
		fStop=false;
		fHandle=NULL;
		fThreadID=NULL;
	}
};

//======================================================================================================
//运行时,线程管理类,这个类主要是用来管理运行时,线程的停止,和记录线程的ID,句柄
//======================================================================================================
class PARSERS_EXPORT CMyThreadsManager
{
public:
    CMyThreadsManager(void);

    ~CMyThreadsManager(void);

    //初始化
    void initialize();
    //清空
    void cleanUp();

    void addNewHandle(HANDLE handle,DWORD id);

	bool makeSureNewThread();

    HANDLE getThreadHandle(DWORD threadID);

	//根据线程ID添加线程的销毁功能
	void  setDestoryByID(DWORD threadID,const XMLCh * const fun,const XMLCh * const arg);

	//根据线程ID添加线程的销毁功能
	void  getDestoryByID(DWORD threadID, XMLCh *  fun, XMLCh *  arg);

	void removeThreadID(DWORD threadID);

	void removeThreadByHandle(HANDLE handle);

	bool setThreadStopFlag(HANDLE handle,bool stop);

	bool isCloseThread(DWORD threadID);
		
	//设置GOTO标记
	void  setGotoLable(DWORD threadID,const XMLCh *  lable);

	//重置GOTO标记
	void  resetGotoLable(DWORD threadID);

	//判断并返回goto指定的句柄
	bool getGotoLable(DWORD threadID, XMLCh *  lable);

    bool haveDestoryFun(DWORD threadID);

private:
	CMyThreadsNode*   fThreadPool;
	int               fLength;
	int               fMaxLength;
};

MATCH_CPP_NAMESPACE_END

#endif
