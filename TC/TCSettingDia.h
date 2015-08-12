#pragma once

// CTCSettingDia 对话框
#include "EditorSettingPage.h"
#include "UpdateSettingPage.h"
#include "afxcmn.h"


class CTCSettingDia : public CDialogEx
{
	DECLARE_DYNAMIC(CTCSettingDia)

public:
	CTCSettingDia(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCSettingDia();

// 对话框数据
	enum { IDD = IDD_TCSETTINGDIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEditorSettingPage *m_editorPage;
	CUpdateSettingPage *m_updatePage;
	CDialogEx *m_tabPage[6];			//所有子页面

	CTabCtrl m_setTab;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult);

	void TCAdjustRect();
	afx_msg void OnBnClickedOk();
};
