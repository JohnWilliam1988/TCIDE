// TCRunConfim.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "TCRunConfim.h"
#include "afxdialogex.h"
#include "TCRunTimeManager.h"


// CTCRunConfim �Ի���

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


// CTCRunConfim ��Ϣ�������


void CTCRunConfim::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTCRunTimeManager::g_ShowRunConfim = m_popup.GetCheck()? false : true ;
	if( CTCRunTimeManager::g_ShowRunConfim )
		CTCRunTimeManager::setMainSettingIni(_T("Debug"),_T("Runconfirm"),_T("True"));
	else
		CTCRunTimeManager::setMainSettingIni(_T("Debug"),_T("Runconfirm"),_T("False"));
	CDialogEx::OnOK();
}

void CTCRunConfim::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}









BOOL CTCRunConfim::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->SetBackgroundColor(0xf5e7dd);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
