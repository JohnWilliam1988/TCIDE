#if !defined(MYAPIPROCESS_H)
#define MYAPIPROCESS_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "AbsBlockParser.h"
#include "BaseBlock.h"
#include "MyVarListContainer.h"
#include "match/My/MyValue.h"
#include "MyDataList.h"
#include <windows.h>

MATCH_CPP_NAMESPACE_BEGIN

//本地默认API的调用方法,调用的matchDll.dll里面的开发的接口
typedef bool (* lpMyCommand)(int index,MyDataList * list,_variant_t * result);
typedef void (* lpMyCtrlWinCwnd)(HANDLE handle);
typedef void (* lpSetTempPath)(const XMLCh * const path);
typedef int  (* lpValidate)(const XMLCh *  nameSpace,const XMLCh * funcName, XMLCh * param);
typedef bool (* lpisNameSpaceExist)(const XMLCh * const nameSpace);
//运行时间dll管理类
class PARSERS_EXPORT MyApiProcess
{
public:
    //方法描述:初始化DLL运行管理类
    //返回值:无
    //返回类型                描述
    //void                    无
	static void initialize();

    //方法描述:销毁DLL运行管理类
    //返回值:无
    //返回类型                描述
    //void                    无
    static void cleanUp();	

	//查找一个空间名是否存在
	static bool IsNameSpExist(const XMLCh *nameSpace);

    //在api.xml里面查找一个API定义
	static int Find(const XMLCh *nameSpace,const XMLCh *funcname, XMLCh *param=0);
	//设置EXE句柄
	static void TCSetExeWinCwnd(HANDLE handle);	
	//设置临时目录
	static void SetTempPath(const XMLCh * const path);
	//执行API接口
	static void run(int index,MyDataList *argumentList,_variant_t*);
private:
    static HMODULE fHDll;
	static lpMyCommand fcmd;
	static lpMyCtrlWinCwnd fsetwinCwnd;
	static lpSetTempPath   fsetTempPath;
	static lpValidate      fValidate;
	static lpisNameSpaceExist fCheckNS;

    MyApiProcess();
};


MATCH_CPP_NAMESPACE_END

#endif
