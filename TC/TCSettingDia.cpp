// TCSettingDia.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "TCView.h"
#include "TCSettingDia.h"
#include "afxdialogex.h"


// CTCSettingDia �Ի���

IMPLEMENT_DYNAMIC(CTCSettingDia, CDialogEx)

CTCSettingDia::CTCSettingDia(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCSettingDia::IDD, pParent)
{

}

CTCSettingDia::~CTCSettingDia()
{

}

void CTCSettingDia::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_TAB, m_setTab);
}


BEGIN_MESSAGE_MAP(CTCSettingDia, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CTCSettingDia::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SETTING_TAB, &CTCSettingDia::OnTcnSelchangeSettingTab)
END_MESSAGE_MAP()


// CTCSettingDia ��Ϣ�������


BOOL CTCSettingDia::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//�������ô��ڱ���
	SetWindowText(L"���ô���");
	m_setTab.InsertItem(0,_T("�༭��"));
	m_setTab.InsertItem(1,_T("�Զ�����"));

	m_editorPage = new CEditorSettingPage();
	m_editorPage->Create(IDD_EDITORSETTING_PAGE,&m_setTab);

	m_updatePage = new CUpdateSettingPage();
	m_updatePage->Create(IDD_UPDATESETTING_PAGE,&m_setTab);

	m_tabPage[0] = m_editorPage;
	m_tabPage[1] = m_updatePage;

	TCAdjustRect();

	m_editorPage->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CTCSettingDia::TCAdjustRect()
{
	CRect rc(0, 0, 0, 0);
	m_setTab.GetClientRect(&rc);
	m_setTab.AdjustRect(FALSE, &rc);//
	m_editorPage->MoveWindow(&rc);
	m_updatePage->MoveWindow(&rc);
}

void CTCSettingDia::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
}

void CTCSettingDia::OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for( int count = 0; count < m_setTab.GetItemCount(); count++ )
		m_tabPage[count]->ShowWindow(SW_HIDE);
	m_tabPage[m_setTab.GetCurSel()]->ShowWindow(SW_SHOW);
	*pResult = 0;
}


void CTCSettingDia::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CMFCFontInfo* font = ((CMFCFontComboBox*)(GetDlgItem(IDC_MFCFONTCOMBO1)))->GetSelFont();/* = m_editorPage->m_fontNameCtrl.GetSelFont();*/
	//CString fontFace = font->m_strName;
	CString fontFace;
	m_editorPage->m_fontCombox.GetLBText(m_editorPage->m_fontCombox.GetCurSel(),fontFace);
	int curSel = m_editorPage->m_fontSizeCtrl.GetCurSel();
	CString fontSize; 
	m_editorPage->m_fontSizeCtrl.GetLBText(curSel,fontSize);
	bool fontBold = m_editorPage->m_editorBoldTrue.GetCheck();
	bool signReplace = m_editorPage->m_editorSignTrue.GetCheck();
	BOOL fontUpdate = FALSE;
	if ( m_editorPage->m_EditInfo.fontFace.CompareNoCase(fontFace) != 0 )
		fontUpdate = TRUE;
	if( m_editorPage->m_EditInfo.fontSize != _wtoi(fontSize) )
		fontUpdate = TRUE;
	if( m_editorPage->m_EditInfo.fontBold != fontBold )
		fontUpdate = TRUE;
	if( m_editorPage->m_EditInfo.replaceSign != signReplace )
		fontUpdate = TRUE;

	int tabWidth = 8;		//Ĭ����8
	CString tabWidthStr;
	m_editorPage->m_tabWidthCtrl.GetWindowTextW(tabWidthStr);
	tabWidth = _wtoi(tabWidthStr);

	if( m_editorPage->m_EditInfo.tabWidth != tabWidth )
		fontUpdate = TRUE;

	if( fontUpdate )
	{
		EditorSetInfo editorInfo;
		editorInfo.fontFace = fontFace;
		editorInfo.fontSize = _wtoi(fontSize);
		editorInfo.fontBold = fontBold;
		editorInfo.tabWidth = tabWidth;
		editorInfo.replaceSign = signReplace;

		POSITION pos = ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetFirstDocPosition();
		CTCView *pView=NULL;
		CTCDoc* pDoc = NULL;
		while (NULL != pos)
		{
			pDoc = (CTCDoc*)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
			if(pDoc==NULL)
				continue;
			POSITION posView = pDoc->GetFirstViewPosition();
			while (NULL != posView)
			{
				pView = (CTCView *)(pDoc->GetNextView(posView));
				if (NULL == pView )
					continue;
				if( pView != NULL )
				{			 
					pView->SetEditorInfo(editorInfo);
				}
			}
		}	
		//���ܴ�û������,�������ȫ������������,������������Ϣ
		CTCRunTimeManager::SetEditorInfo(editorInfo);
	}
	bool updateCheck = m_updatePage->m_updateTrue.GetCheck();
	CTCRunTimeManager::SetUpdateInfo(updateCheck);

	CDialogEx::OnOK();
	//this->ShowWindow(SW_HIDE);
	
}

