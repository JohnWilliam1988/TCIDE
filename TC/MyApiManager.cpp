#include "StdAfx.h"
#include "MyApiManager.h"

using namespace std;

CMyApiManager::CMyApiManager(void)
{
	xml.ResetPos();
	m_allApi.clear();
}

/***********************************************************************/
/*  \函数名称：LoadApiXml
/*	\函数功能：加载XML到API管理对象
/*	\函数返回：BOOL 如果是TRUE，加载成功，否则失败。
/*	\参数说明：
	@param     CString xmlPath XML路径
/***********************************************************************/
BOOL CMyApiManager::LoadApiXml( CString xmlPath )
{
	if( xml.Load( xmlPath ) )
	{	
		int time = 0;
		while ( xml.FindChildElem(_T("dll")) )
		{	
			dllNode temp;
			CString spaceName = xml.GetChildTagName();
			//列表名称
			CString listName = xml.GetChildAttrib(_T("name"));
			temp.name = listName;
			xml.IntoElem();
			while ( xml.FindChildElem(_T("namespace")) )
			{
				NamespaceNode naspNode;
				//空间名称
				CString namespaceName = xml.GetChildAttrib(_T("name"));
				naspNode.namespaceName = namespaceName;
				
				xml.IntoElem();
				//查找函数节点
				while ( xml.FindChildElem(_T("function")) )
				{
					FunctionNode funNode;
					//函数名称
					CString functionName = xml.GetChildAttrib(_T("name"));
					funNode.funcName = functionName;
					
					xml.IntoElem();
					if( xml.FindChildElem( _T("pram")) )
					{
						//函数参数
						CString functionParam = xml.GetChildData();
						funNode.funcPram = functionParam;
					}
					
					if( xml.FindChildElem(_T("example")))
					{
						//函数举例
						CString functionExample = xml.GetChildData();
						functionExample.Trim();
						funNode.funcExample = functionExample;
					}	
					xml.OutOfElem();
					naspNode.fun.push_back(funNode);
				}
				xml.OutOfElem();
				temp.nasp.push_back(naspNode);
			}		
			xml.OutOfElem();

			m_allApi.push_back(temp);
		}
	}
	else
	{
		AfxMessageBox(_T("未能定位到APIXML文件!"));
		return FALSE;
	}
	return TRUE;
}


CString CMyApiManager::GetExample(CString nameSpace,CString funName)
{
	for( int size = 0; size < m_allApi.size(); size++ )
	{
		for( int nspa = 0; nspa <  m_allApi[ size ].nasp.size();  nspa++ )
		{
			CString helpSPName = m_allApi[ size ].nasp[ nspa ].namespaceName;
			if( 0 == helpSPName.CompareNoCase(nameSpace) )
			{
				for( int func = 0; func < m_allApi[ size ].nasp[ nspa ].fun.size(); func++ )
				{
					CString csFun = m_allApi[ size ].nasp[ nspa ].fun[ func ].funcName;
					if( 0 == csFun.CompareNoCase(funName) )
					{
						return  m_allApi[ size ].nasp[ nspa ].fun[ func ].funcExample;
					}
				}
			}

		}
	}
	return _T("");
}


CMyApiManager::~CMyApiManager(void)
{

}
