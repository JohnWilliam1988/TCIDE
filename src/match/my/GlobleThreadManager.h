#if !defined(_MYGLOBLETHREADMANAGER_H_)
#define _MYGLOBLETHREADMANAGER_H_

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <windows.h>

#include "MyThreadsManager.h"

MATCH_CPP_NAMESPACE_BEGIN

class XMLPARSER_EXPORT CGlobleThreadManager
{
public:
	//初始化
	static void initialize();
	//清理
	static void cleanUp();

	static HANDLE getThreadHandle(DWORD threadID);

	//将新的线程句柄、ID以及状态加入到线程ID链表
	static void  addNewHandle(HANDLE handle,DWORD id);

	//删除线程管理节点
	static void  removeThreadNode(DWORD threadID);

	//删除线程管理节点
	static void  removeThreadByHandle(HANDLE hnd);

    //将线程ID的状态位置位-执行
    static bool setThreadFlag(HANDLE hnd,bool isStop);

	//置位停止标记符
	static void setStopFlag();

	//获取停止标记符状态
	static bool getStopFlagState();

	//是否关闭线程
	static bool isCloseThread();

	//根据线程ID添加线程的销毁功能
	static void setDestoryByID(DWORD threadID,const XMLCh * const ns,const XMLCh * const fun);

	//
	static void runDestoryFunByID(DWORD threadID);

	//设置GOTO标记
	static void  setGotoLable(DWORD threadID, const XMLCh *  lable);

	//重置GOTO标记
	static void  resetGotoLable(DWORD threadID);

	static bool getGotoLable(DWORD threadID,  XMLCh *  lable);

public:
	static bool                                     gStop;              //控制主执行线程的标记位
    static CMyThreadsManager                        gThreadManager;          //线程管理类

private:
	CGlobleThreadManager(void);

	~CGlobleThreadManager(void);
};

MATCH_CPP_NAMESPACE_END
#endif