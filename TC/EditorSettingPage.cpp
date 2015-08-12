// EditorSettingPage.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "EditorSettingPage.h"
#include "afxdialogex.h"


// CEditorSettingPage 对话框
int CALLBACK MyEnumFontProc(ENUMLOGFONTEX* lpelf,NEWTEXTMETRICEX* lpntm,DWORD nFontType,long lParam) 
{ 
	CComboBox* pWnd=(CComboBox*)lParam; 
	if(pWnd) 
	{  
		if (-1 == pWnd->FindString(0, lpelf->elfLogFont.lfFaceName))//重复的就不加
		{
			pWnd->AddString(lpelf->elfLogFont.lfFaceName);//加入到下拉列表中
		}
		return 1; 
	} 
	return 0; 
}

IMPLEMENT_DYNAMIC(CEditorSettingPage, CDialogEx)

CEditorSettingPage::CEditorSettingPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditorSettingPage::IDD, pParent)
{

}

CEditorSettingPage::~CEditorSettingPage()
{
}

void CEditorSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_fontSizeCtrl);
	DDX_Control(pDX, IDC_EDITOR_BOLDTRUE, m_editorBoldTrue);
	DDX_Control(pDX, IDC_EDITOR_BOLDFALSE, m_editorBoldFalse);
	DDX_Control(pDX, IDC_EDIT_TABCOUNT, m_tabWidthCtrl);
	DDX_Control(pDX, IDC_SPIN_TABWIDTH, m_tabWidthSplinCtrl);
	DDX_Control(pDX, IDC_EDITORSIGN_TRUE, m_editorSignTrue);
	DDX_Control(pDX, IDC_EDITORSIGN_FALSE, m_editorSignFalse);

	DDX_Control(pDX, IDC_COMBOFONT, m_fontCombox);
}


BEGIN_MESSAGE_MAP(CEditorSettingPage, CDialogEx)
	ON_BN_CLICKED(IDC_EDITOR_RESET, &CEditorSettingPage::OnBnClickedEditorReset)
	ON_BN_CLICKED(IDOK, &CEditorSettingPage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditorSettingPage::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEditorSettingPage 消息处理程序


void CEditorSettingPage::OnBnClickedEditorReset()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_fontNameCtrl.SelectFont(_T("新宋体"));
	if (CB_ERR == m_fontCombox.SelectString(-1, _T("新宋体")))
	{
		if (CB_ERR == m_fontCombox.SelectString(-1, _T("宋体")))
		{
			AfxMessageBox(_T("字体初始化失败"));
		}
	}
	m_fontSizeCtrl.SetCurSel(5);
	m_editorBoldTrue.SetCheck(FALSE);
	m_editorBoldFalse.SetCheck(TRUE);
	m_tabWidthCtrl.SetWindowTextW(_T("8"));
	m_editorSignTrue.SetCheck(FALSE);
	m_editorSignFalse.SetCheck(TRUE);
}


BOOL CEditorSettingPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	LOGFONT lf; 
	lf.lfCharSet=DEFAULT_CHARSET; 
	lf.lfFaceName[0]=NULL; 
	lf.lfPitchAndFamily=0;

	CClientDC dc(this); 
	::EnumFontFamiliesExW((HDC)dc,&lf,(FONTENUMPROC) MyEnumFontProc,(LPARAM)&m_fontCombox,0); 

	// TODO:  在此添加额外的初始化
	for( int i = 0; i < 19; i++ )
	{
		CString range;
		range.Format(_T("%d"),i + 6);
		m_fontSizeCtrl.InsertString(i,range);
	}

	//初始化微调控件范围 
	m_tabWidthSplinCtrl.SetRange(1,8);
	m_tabWidthSplinCtrl.SetBuddy(&m_tabWidthCtrl);

	m_EditInfo.fontFace = CTCRunTimeManager::g_EditorFontName;
	m_EditInfo.fontSize = CTCRunTimeManager::g_EditorFontSize - 6;
	m_EditInfo.fontBold = CTCRunTimeManager::g_EditorFontBold;
	m_EditInfo.tabWidth = CTCRunTimeManager::g_EditorTabWidth;

	if (CB_ERR == m_fontCombox.SelectString(-1, CTCRunTimeManager::g_EditorFontName))
	{
		CTCRunTimeManager::g_EditorFontName = _T("新宋体");
		m_EditInfo.fontFace = CTCRunTimeManager::g_EditorFontName;
	}

	m_fontSizeCtrl.SetCurSel(CTCRunTimeManager::g_EditorFontSize - 6);
	CTCRunTimeManager::g_EditorFontBold ? m_editorBoldTrue.SetCheck(TRUE) : m_editorBoldFalse.SetCheck(TRUE);
	CString widthStr;
	widthStr.Format(_T("%d"),CTCRunTimeManager::g_EditorTabWidth);
	m_tabWidthCtrl.SetWindowTextW(widthStr);

	CTCRunTimeManager::g_EditorSignReplace ? m_editorSignTrue.SetCheck(TRUE) : m_editorSignFalse.SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEditorSettingPage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CEditorSettingPage::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}
