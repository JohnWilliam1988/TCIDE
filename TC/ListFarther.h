#pragma once
#ifndef __LISTFARTHER_H__
#define __LISTFARTHER_H__
#include "IconListBox.h"
//#include "MyIntellisenseList.h"

// ListFarther �Ի���
class CIconListBox;
class ListFarther : public CDialogEx
{
	//DECLARE_DYNAMIC(ListFarther)

public:
	ListFarther(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ListFarther();

// �Ի�������
	enum { IDD = IDD_DIAINTELLLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	//������ʾ���ڶ��� 
	//CMyIntellisenseList m_listView;
	CIconListBox  m_listBox;
	CImageList	  m_imgNormal;

	virtual BOOL PreTranslateMessage(MSG* pMsg);		
};
#endif