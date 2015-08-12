#pragma once
#include "afxwin.h"
#include "TCView.h"

// CTCEditorLineJumper 对话框

class CTCEditorLineJumper : public CDialogEx
{
	DECLARE_DYNAMIC(CTCEditorLineJumper)

public:
	CTCEditorLineJumper(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCEditorLineJumper();

// 对话框数据
	enum { IDD = IDD_DIAEDITORLINEJUMPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditorLine;
	CStatic m_EditorStatic;
	CTCView *m_pTView;
	afx_msg void OnBnClickedOk();
};
