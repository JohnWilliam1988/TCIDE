#if !defined(MYEVENTSYNC_H)
#define MYEVENTSYNC_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <windows.h>

#include "MyBPList.h"
#include "MyVarListContainer.h"

MATCH_CPP_NAMESPACE_BEGIN


struct PARSERS_EXPORT CMyDebugInfor
{
    int                              fDebugType;			                         //执行类型，是一枚举数据
	int                              fLeng;
	MyBreakPointInfo                 fList[MATCH_MAXBREAKBPNUM];
	MyBreakPointInfo                 fCurrentBP;
	XMLCh                            fVarInfo[MAXDEBUGVARBUFFLENGTH];
};

class PARSERS_EXPORT CMyDebugManager
{
public:
	//用来告诉解析是执行还是调试（单步调试或者断点调试）
	enum MatchDebugFalg
	{
		MATCH_NODEBUG,                //不调试
		MATCH_STEPBYSTEP,             //单步调试
        MATCH_BREAKDEBUG,             //断点调试
	};

	//初始化
	static void initialize();

	//清理
	static void cleanUp();

	//方法描述:解析器调试控制句柄,让解析器继续执行
	//返回值:无
	//返回类型                描述
	//void                    无
	//参数说明:
	//参数名称      参数类型                    描述
	//无
	static void  setLibEvent();

    //方法描述:获取解析器调试控制句柄
    //返回值:   事件句柄
    //返回类型                描述
    //HANDLE                  事件句柄
    //参数说明:
    //参数名称      参数类型                    描述
    //无
    static HANDLE getLibEventHandle();

    //方法描述:控制界面调试时候的句柄
    //返回值:       事件句柄
    //返回类型                 描述
    //HANDLE                   事件句柄
    //参数说明:
    //参数名称      参数类型   描述
    //无
    static HANDLE getWinEventHandle();

	//方法描述:设置调试的类型
	//返回值:  void
	//返回类型                          描述
	//void
	//参数说明:
	//参数名称      参数类型                    描述
	//style         MatchDebugFalg              调试执行的类型
    static void   setDebugStyle(MatchDebugFalg style=MatchDebugFalg::MATCH_NODEBUG);

	//方法描述:获取执行的类型
	//MATCH_NODEBUG,              不调试
	//MATCH_STEPBYSTEP,           单步调试
    //MATCH_BREAKDEBUG,           断点调试
	//返回类型                          描述
	//MatchDebugFalg                    调试执行的类型
	//参数说明:
	//参数名称      参数类型            描述
	//无
	static MatchDebugFalg  getDebugStyle();

    //方法描述:设置当前执行的断点节点
    //返回值:  void
    //返回类型                          描述
    //void
    //参数说明:
    //参数名称      参数类型            描述
    //Node          MyBreakPointInfo*   调试信息节点
    static void  setCurrentBPTransport(const XMLCh* const path,XMLUInt32 lineNum);

	//方法描述:当前有效变量列表的传递，为变量查看功能准备
	//返回值:       void
	//返回类型                          
	///void     
	//参数说明:
	//参数名称      参数类型                    描述
	//namesp        const XMLCh* const          命名空间名
	//VarList       MyVarListContainer*         局部有效变量列表
	static void varListTransport(const XMLCh* const namesp,MyVarListContainer* VarList,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	//方法描述: 判断一个节点是否存在,
	static bool isExist(const XMLCh* const filepath,int linenum);

	// 判断列表是否为空
	static bool isEmpty();

public:
    static HANDLE						   fLibEventHandle;                              //解析器中的调试处理事件,处理断点中断的情况
    static HANDLE						   fWinEventHandle;		                         //与界面通信的事件，告诉界面已经到断点了
    static MatchDebugFalg                  fDebugType;			                         //执行类型，是一枚举数据
	static HANDLE	        m_hFileMap;            //映射文件
	static CMyDebugInfor*	m_pBuffer;             //映射文件首地址

protected:
	//构造函数
	CMyDebugManager();
};

MATCH_CPP_NAMESPACE_END

#endif
