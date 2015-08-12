#include "StdAfx.h"
#include "MyApiManager.h"

using namespace std;

CMyApiManager::CMyApiManager(void)
{
	xml.ResetPos();
	m_allApi.clear();
}

/***********************************************************************/
/*  \�������ƣ�LoadApiXml
/*	\�������ܣ�����XML��API�������
/*	\�������أ�BOOL �����TRUE�����سɹ�������ʧ�ܡ�
/*	\����˵����
	@param     CString xmlPath XML·��
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
			//�б�����
			CString listName = xml.GetChildAttrib(_T("name"));
			temp.name = listName;
			xml.IntoElem();
			while ( xml.FindChildElem(_T("namespace")) )
			{
				NamespaceNode naspNode;
				//�ռ�����
				CString namespaceName = xml.GetChildAttrib(_T("name"));
				naspNode.namespaceName = namespaceName;
				
				xml.IntoElem();
				//���Һ����ڵ�
				while ( xml.FindChildElem(_T("function")) )
				{
					FunctionNode funNode;
					//��������
					CString functionName = xml.GetChildAttrib(_T("name"));
					funNode.funcName = functionName;
					
					xml.IntoElem();
					if( xml.FindChildElem( _T("pram")) )
					{
						//��������
						CString functionParam = xml.GetChildData();
						funNode.funcPram = functionParam;
					}
					
					if( xml.FindChildElem(_T("example")))
					{
						//��������
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
		AfxMessageBox(_T("δ�ܶ�λ��APIXML�ļ�!"));
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
