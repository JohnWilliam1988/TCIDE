#ifndef MYCOMFRAME_H
#define MYCOMFRAME_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <match/framework/MemoryManager.hpp> //内存管理类
#include <match/util/OutOfMemoryException.hpp>//异常类
#include <match/util/Janitor.hpp>


#include "MyApiDllList.h"
#include "MyValue.h"


MATCH_CPP_NAMESPACE_BEGIN 

//代表整个XML文件
class XMLPARSER_EXPORT MyComFrame
{
public:
	//构造函数
	//参数说明:
	//参数名称      参数类型                    描述
	//manager       MemoryManager* const        内存管理器
	MyComFrame(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//析构函数
	~MyComFrame();

	//方法描述：得到访问DLL链表的指针
	///返回值:  函数的头节点
	//返回类型                描述
	//const MyApiDllList *    DLL链表的指针
	const MyApiDllList * getList() const;

	//方法描述：得到访问DLL链表的指针
	///返回值:  函数的头节点
	//返回类型                描述
	//const MyApiDllList *    DLL链表的指针
	 MyApiDllList * getList();


	//方法描述：将一个dll节点,放到dll链表里面去
	///返回值: 无
	//参数说明:
	//参数名称      参数类型                    描述
	//MyApiDll      const MyApiDll *            DLL节点
	void insertDll(MyApiDll * const MyApiDll);

	//方法描述：查找DLL的路径
	///返回值:  DLL路径
	//返回类型                描述
	//const XMLCh *          DLL的路径
	//参数说明:
	//参数名称      参数类型                    描述
	//namesp      const XMLCh * const            命名空间名称
	//funcname    const XMLCh * const            函数命名名字
	const XMLCh* findDllPath(const XMLCh * const namesp,const XMLCh * const funcname) const;

	//方法描述：查找函数举例
	///返回值:  举例
	//返回类型                描述
	//const XMLCh *           举例
	//参数说明:
	//参数名称      参数类型                    描述
	//namesp      const XMLCh * const            命名空间名称
	//funcname    const XMLCh * const            函数命名名字
	const XMLCh* findFunExample(const XMLCh * const namesp,const XMLCh * const funcname) const;

	//
    //方法描述:判断空间名是否已经在本地API中存在
    //返回值:返回真 表示存在,假 表示不存在
    //返回类型                描述
    //bool                   返回真 表示存在,假 表示不存在
    //参数说明:
    //参数名称          参数类型                    描述
    //nameSpace         const XMLCh * const         要查找的命名空间
	bool isNameSpaceExist(const XMLCh * const nameSpace);

	//清空
	void cleanUp(void);

protected:

	//初始化
	void initialize();

	//HINSTANCE g_HDLL;  //dll 的句柄

	MyApiDllList *fList;

	MemoryManager *fMemoryManager; //内存分配

};

MATCH_CPP_NAMESPACE_END

#endif