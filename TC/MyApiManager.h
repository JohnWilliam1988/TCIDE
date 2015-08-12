#pragma once

#include <vector>

#include "match/util/MatchDefs.hpp"
#include "Markup.h"

using namespace std;

//函数信息结构体
struct FunctionNode
{
	CString funcName;	//函数名称
	CString funcPram;	//函数参数
	CString funcExample;	//函数实例
	FunctionNode()
	{
		funcName = _T("");
		funcPram = _T("");
		funcExample = _T("");
	}
};

//空间信息结构体
struct NamespaceNode
{
	CString namespaceName;					//空间名称
	vector< FunctionNode > fun;				//空间函数列表
	NamespaceNode()
	{
		namespaceName = _T("");
	}
};

//功能列表结构体
struct dllNode
{
	CString name;						//列表名称
	vector< NamespaceNode > nasp;		//空间数组
	dllNode()
	{
		name = _T("");
	}
};

class CMyApiManager
{
public:
	CMyApiManager(void);

	BOOL	LoadApiXml(CString xmlPath);
	CString GetExample(CString nameSpace,CString funName);
private:
	//加载本地APIXML管理对象
	CMarkup xml;
	
public:
	//所有空间函数向量
	vector< dllNode > m_allApi;	

public:
	~CMyApiManager(void);
};

