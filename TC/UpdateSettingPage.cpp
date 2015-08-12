// UpdateSettingPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "UpdateSettingPage.h"
#include "afxdialogex.h"


// UpdateSettingPage �Ի���

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

// UpdateSettingPage ��Ϣ�������



BOOL CUpdateSettingPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CTCRunTimeManager::g_UpdateCheck ? m_updateTrue.SetCheck(TRUE) : m_updateFalse.SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
