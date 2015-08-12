// TCRunConfim.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCRunConfim.h"
#include "afxdialogex.h"
#include "TCRunTimeManager.h"


// CTCRunConfim 对话框

IMPLEMENT_DYNAMIC(CTCRunConfim, CDialogEx)

CTCRunConfim::CTCRunConfim(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCRunConfim::IDD, pParent)
{

}

CTCRunConfim::~CTCRunConfim()
{
}

void CTCRunConfim::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_popup);
}


BEGIN_MESSAGE_MAP(CTCRunConfim, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTCRunConfim::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTCRunConfim::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTCRunConfim 消息处理程序


void CTCRunConfim::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CTCRunTimeManager::g_ShowRunConfim = m_popup.GetCheck()? false : true ;
	if( CTCRunTimeManager::g_ShowRunConfim )
		CTCRunTimeManager::setMainSettingIni(_T("Debug"),_T("Runconfirm"),_T("True"));
	else
		CTCRunTimeManager::setMainSettingIni(_T("Debug"),_T("Runconfirm"),_T("False"));
	CDialogEx::OnOK();
}

void CTCRunConfim::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}









BOOL CTCRunConfim::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetBackgroundColor(0xf5e7dd);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
