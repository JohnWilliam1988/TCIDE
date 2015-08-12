#pragma once

#include <vector>

#include "match/util/MatchDefs.hpp"
#include "Markup.h"

using namespace std;

//������Ϣ�ṹ��
struct FunctionNode
{
	CString funcName;	//��������
	CString funcPram;	//��������
	CString funcExample;	//����ʵ��
	FunctionNode()
	{
		funcName = _T("");
		funcPram = _T("");
		funcExample = _T("");
	}
};

//�ռ���Ϣ�ṹ��
struct NamespaceNode
{
	CString namespaceName;					//�ռ�����
	vector< FunctionNode > fun;				//�ռ亯���б�
	NamespaceNode()
	{
		namespaceName = _T("");
	}
};

//�����б�ṹ��
struct dllNode
{
	CString name;						//�б�����
	vector< NamespaceNode > nasp;		//�ռ�����
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
	//���ر���APIXML�������
	CMarkup xml;
	
public:
	//���пռ亯������
	vector< dllNode > m_allApi;	

public:
	~CMyApiManager(void);
};

