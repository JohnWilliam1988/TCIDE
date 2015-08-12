// UpdateSettingPage.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "UpdateSettingPage.h"
#include "afxdialogex.h"


// UpdateSettingPage 对话框

IMPLEMENT_DYNAMIC(CUpdateSettingPage, CDialogEx)

CUpdateSettingPage::CUpdateSettingPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateSettingPage::IDD, pParent)
{

}

CUpdateSettingPage::~CUpdateSettingPage()
{
}

void CUpdateSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIOUPDATE_TRUE, m_updateTrue);
	DDX_Control(pDX, IDC_RADIOUPDATE_FALSE, m_updateFalse);
}


BEGIN_MESSAGE_MAP(CUpdateSettingPage, CDialogEx)
END_MESSAGE_MAP()

// UpdateSettingPage 消息处理程序



BOOL CUpdateSettingPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CTCRunTimeManager::g_UpdateCheck ? m_updateTrue.SetCheck(TRUE) : m_updateFalse.SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
