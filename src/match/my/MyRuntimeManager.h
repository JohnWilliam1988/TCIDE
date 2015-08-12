///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//文件建立修改记录
//索引      日期        记录人      动作以及目的 
//0001      2010-06-26  肖秀范      完成运行时管理类MyRuntimeManager,运行时所有的操作都是在这个类里面管理的
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYRUNTIMEMANAGER_H
#define MYRUNTIMEMANAGER_H

#include <match/util/PlatformUtils.hpp>
#include <wchar.h>
#include <windows.h>
#include "BaseBlock.h"

MATCH_CPP_NAMESPACE_BEGIN

class MemoryManager;
class MyGlobalVarListManager;
class MyBlockListPool;
class BaseBlock;
class DependBlockList;
class MyFunInfor;
class MyComFunInfor;

//MyRuntimeStruct 用来记录运行时的一些环境数据,比如全局变量,命名空间,当前运行块,my解析器运行前,必须先初试化当前类
class XMLPARSER_EXPORT MyRuntimeManager: public XMemory
{
public:
	static MemoryManager*                     fMemoryManager;                   //内存管理
	static MyGlobalVarListManager *           fGlobalVarList;                   //存储全局变量表
	static MyBlockListPool *                  fListPool;                        //用来存储执行块池塘
	static DependBlockList *                  fDependentFile;                   //用一个块连表得到所有的依赖文件
    static HWND                               fWndHandle;                       //当前界面的句柄,用来控件和界面通讯的
    static XMLCh                              fTempPath[MAX_PATH];              //当前的临时目录的路径

    //获取窗口的句柄
    static HWND getWndHandle();

    //设置窗口的句柄
    static void setWndHandle(HWND handle);

    //
    //方法描述:判断空间名是否已经在本地API中存在
    //返回值:返回真 表示存在,假 表示不存在
    //返回类型                描述
    //bool                   返回真 表示存在,假 表示不存在
    //参数说明:
    //参数名称          参数类型                    描述
    //nameSpace         const XMLCh * const         要查找的命名空间
	static bool isNameSpaceExist(const XMLCh * const nameSpace);

    //
    //方法描述:在块池中查找一个块
    //返回值:找到的块
    //返回类型                描述
    //const BaseBlock *       返回找到的块,返回 NULL,表示失败
    //参数说明:
    //参数名称          参数类型                    描述
    //nameSpace         const XMLCh * const         要查找的命名空间
    //funcName          const XMLCh * const         要查找的函数名
	static const BaseBlock * findFuncBlockByName(const XMLCh * const nameSpace,const XMLCh* const funcName);

	//
    //方法描述:在自身文件,和头文件中查找函数原型,这个主要是为了解决上下互调,两个文件函数互调的问题
	//返回值:找到的块
	//返回类型                描述
	//const BaseBlock *       返回找到的块,返回 NULL,表示失败
	//参数说明:
	//参数名称          参数类型                    描述
	//nameSpace        const XMLCh * const         要查找的命名空间
	//funcName         const XMLCh * const         要查找的函数名
	//pFun             MyFunInfor *                函数原型信息
	static bool findFunInforByName(const XMLCh * const nameSpace,const XMLCh* const funcName,XMLCh * funParam);


	//查找变量
    static bool findVarInforByName(const XMLCh * const nameSpace,const XMLCh* const varName);


    //
    //方法描述:初始化执行基类的静态数据
    //返回值:无
    //返回类型                描述
    //void                    无
    //参数说明:
    //参数名称              参数类型                    描述
	static void initializeAbsBlockStaticData();

    //
    //方法描述:清理执行基类的静态数据
    //返回值:无
    //返回类型                描述
    //void                    无
	static void TerminateAbsBlockStaticData();

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

    //静态类,构造方法私有化
	MyRuntimeManager();
};


//记录函数的标记
struct MyLableInfor
{
    XMLCh fNS[NAMEMAXLENGTH];
    XMLCh fFun[MAXPARAMLENGTH];
	XMLCh fLable[NAMEMAXLENGTH];
	MyLableInfor()
	{
		::wmemset(fNS,0,NAMEMAXLENGTH);
		::wmemset(fFun,0,NAMEMAXLENGTH);
		::wmemset(fLable,0,NAMEMAXLENGTH);
	}
};

struct MyLableStr
{
	XMLCh fLable[NAMEMAXLENGTH];
	MyLableStr()
	{
		::wmemset(fLable,0,NAMEMAXLENGTH);
	}
};

MATCH_CPP_NAMESPACE_END


#endif