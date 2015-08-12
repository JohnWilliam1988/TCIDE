#pragma once
#include "afxwin.h"


// CTCDesktopAlertDia ¶Ô»°¿ò

class CTCDesktopAlertWnd : public CMFCDesktopAlertWnd
{
	DECLARE_DYNCREATE(CTCDesktopAlertWnd)

public:
	CTCDesktopAlertWnd(CWnd* pParent = NULL);

	virtual ~CTCDesktopAlertWnd();
	virtual BOOL OnClickLinkButton(UINT uiCmdID);

private:
	CString m_url;
public:
	void SetAlertUrl(CString url);
	BOOL IsAlertWndActive();
	CMFCDesktopAlertDialog* GetAlertDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

class CTCDesktopAlertDlg : public CMFCDesktopAlertDialog
{
	DECLARE_DYNCREATE(CTCDesktopAlertDlg)

// Construction
public:
	CTCDesktopAlertDlg();

// Dialog Data
	enum { IDD = IDD_TCDESKTOPALERT };
	CStatic	m_wndFrom;
	//CMFCDesktopAlertWndButton	m_btnFlag;
	//CMFCDesktopAlertWndButton	m_btnDelete;
	CMFCLinkCtrl	m_btnRL;
	CString m_url;
	void SetAlertUrl(CString url);
// Overrides
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	virtual BOOL OnInitDialog();

	/*afx_msg void OnDelete();
	afx_msg void OnFlag();*/
	afx_msg void OnButton1();

	DECLARE_MESSAGE_MAP()

	CFont	m_fontBold;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};