#pragma once
#ifndef __LISTFARTHER_H__
#define __LISTFARTHER_H__
#include "IconListBox.h"
//#include "MyIntellisenseList.h"

// ListFarther 对话框
class CIconListBox;
class ListFarther : public CDialogEx
{
	//DECLARE_DYNAMIC(ListFarther)

public:
	ListFarther(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ListFarther();

// 对话框数据
	enum { IDD = IDD_DIAINTELLLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	//智能提示窗口对象 
	//CMyIntellisenseList m_listView;
	CIconListBox  m_listBox;
	CImageList	  m_imgNormal;

	virtual BOOL PreTranslateMessage(MSG* pMsg);		
};
#endif