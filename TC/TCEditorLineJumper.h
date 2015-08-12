#pragma once
#include "afxwin.h"
#include "TCView.h"

// CTCEditorLineJumper �Ի���

class CTCEditorLineJumper : public CDialogEx
{
	DECLARE_DYNAMIC(CTCEditorLineJumper)

public:
	CTCEditorLineJumper(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTCEditorLineJumper();

// �Ի�������
	enum { IDD = IDD_DIAEDITORLINEJUMPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_EditorLine;
	CStatic m_EditorStatic;
	CTCView *m_pTView;
	afx_msg void OnBnClickedOk();
};
