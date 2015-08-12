// TCDesktopAlertDia.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCDesktopAlertDia.h"
#include "afxdialogex.h"
//#include "TCStatic.h"


IMPLEMENT_DYNCREATE(CTCDesktopAlertWnd, CMFCDesktopAlertWnd)
CTCDesktopAlertWnd::CTCDesktopAlertWnd(CWnd* pParent /*=NULL*/)
	: CMFCDesktopAlertWnd()
{
	m_url = _T("");
}

CTCDesktopAlertWnd::~CTCDesktopAlertWnd()
{
	
}

BOOL CTCDesktopAlertWnd::OnClickLinkButton(UINT uiCmdID)
{
	if(!m_url.IsEmpty())
		ShellExecute(NULL, _T("open"), m_url, NULL, NULL, SW_SHOWNORMAL); 
	return FALSE;
}

void CTCDesktopAlertWnd::SetAlertUrl(CString url)
{
	m_url = url;
}

BOOL CTCDesktopAlertWnd::IsAlertWndActive()
{
	return m_bIsActive;
}

CMFCDesktopAlertDialog* CTCDesktopAlertWnd::GetAlertDialog()
{
	return m_pWndDlg;
}

// CTCDesktopAlertDia 对话框

IMPLEMENT_DYNCREATE(CTCDesktopAlertDlg, CMFCDesktopAlertDialog)

CTCDesktopAlertDlg::CTCDesktopAlertDlg()
{
	m_url = _T("");
}


void CTCDesktopAlertDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCDesktopAlertDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FROM, m_wndFrom);
	/*DDX_Control(pDX, IDC_FLAG, m_btnFlag);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);*/
	DDX_Control(pDX, IDC_BTN_LINK, m_btnRL);
}


BEGIN_MESSAGE_MAP(CTCDesktopAlertDlg, CMFCDesktopAlertDialog)
	/*ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_FLAG, OnFlag)*/
	ON_BN_CLICKED(IDC_BTN_LINK, OnButton1)
	ON_BN_CLICKED(IDOK, &CTCDesktopAlertDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTCDesktopAlertDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPopupDlg message handlers

BOOL CTCDesktopAlertDlg::OnInitDialog() 
{
	CMFCDesktopAlertDialog::OnInitDialog();
	
	LOGFONT lf;
	m_wndFrom.GetFont ()->GetLogFont (&lf);

	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect (&lf);
	
	m_wndFrom.SetFont (&m_fontBold);
	//m_btnFlag.SetImage (IDB_FLAG);
	//m_btnDelete.SetImage (IDB_DELETE);

	m_btnRL.m_bMultilineText = TRUE;
	m_btnRL.m_bAlwaysUnderlineText = FALSE;
	m_btnRL.m_bDefaultClickProcess = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTCDesktopAlertDlg::SetAlertUrl(CString url)
{
	m_url = url;
}

//void CMyPopupDlg::OnDelete() 
//{
//	GetParent ()->PostMessage (WM_CLOSE);
//}
//
//void CMyPopupDlg::OnFlag() 
//{
//	GetParent ()->PostMessage (WM_CLOSE);
//}

void CTCDesktopAlertDlg::OnButton1() 
{
	if(!m_url.IsEmpty())
		ShellExecute(NULL, _T("open"), m_url, NULL, NULL, SW_SHOWNORMAL); 
	GetParent ()->PostMessage (WM_CLOSE);
}
BEGIN_MESSAGE_MAP(CTCDesktopAlertWnd, CMFCDesktopAlertWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int CTCDesktopAlertWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCDesktopAlertWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


void CTCDesktopAlertDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTCDesktopAlertDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}
