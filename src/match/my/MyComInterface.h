#if !defined(MYCOMINTERFACE_H)
#define MYCOMINTERFACE_H

#include <atlbase.h>
#include <atlstr.h>
#include "comdef.h"
#include "MyVar.h"
#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyUni.h"
#include "MyValue.h"
#include "MyDataList.h"
#include <vector>
MATCH_CPP_NAMESPACE_BEGIN

struct MyComInfor
{
	//void *  fDisp;
	CComPtr <ITypeInfo> pTypeInfo;
	MyComInfor()
	{
		//fDisp=NULL;
	}
	~MyComInfor()
	{
		//((IDispatch*)fDisp)->Release();
	}
};

struct  MyComPathInfor
{
public:
    MyComPathInfor()
	{
		pDisp=NULL;
		::memset(path,0,MAX_PATH*sizeof(XMLCh));
	}
public:
	void   *pDisp;
	XMLCh  path[MAX_PATH];
};



class MyVar;
class MyComInterface
{
public:
	static void initialComInterface();
	//static bool CreateComObject(MyVar* pVar,const XMLCh * const comID);
	static bool CreateComObject(_variant_t* pVar,const XMLCh * const comID);
	//static bool Cmd(MyComInfor  *pCom,const XMLCh * const fun,MyDataList *argumentList,_variant_t* result);
	static bool Cmd(IDispatch* pDis,const XMLCh * const fun,MyDataList *argumentList,_variant_t* result);
		
	static bool PropertyGet(IDispatch* pDis,const XMLCh * const name,_variant_t* result);
	static bool PropertyPut(IDispatch* pDis,const XMLCh * const name,_variant_t* value,_variant_t* result);
		

	//获取注册控件路径
	static bool GetComPath(const XMLCh * const id,XMLCh * path);

	//添加COM的路径信息
	static bool AddComInforPath(void *pDis,XMLCh * path);

	//获得COM的路径信息
	static const XMLCh* GetComInforPath(void *pDis);

public:
	static std::vector<MyComPathInfor>   fComPaths;

private:
	//构造函数
	MyComInterface(void);

	/**
	* Copy constructor
	*/
    MyComInterface(const MyComInterface& pool);
    MyComInterface& operator=(const MyComInterface& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif