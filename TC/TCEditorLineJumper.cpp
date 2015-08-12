// TCEditorLineJumper.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "TCEditorLineJumper.h"
#include "afxdialogex.h"


// CTCEditorLineJumper �Ի���

IMPLEMENT_DYNAMIC(CTCEditorLineJumper, CDialogEx)

CTCEditorLineJumper::CTCEditorLineJumper(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCEditorLineJumper::IDD, pParent)
{
	m_pTView = NULL;
}

CTCEditorLineJumper::~CTCEditorLineJumper()
{
}

void CTCEditorLineJumper::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITOR_LINEINDEX, m_EditorLine);
	DDX_Control(pDX, IDC_STATIC_EDITORLINE, m_EditorStatic);
}


BEGIN_MESSAGE_MAP(CTCEditorLineJumper, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTCEditorLineJumper::OnBnClickedOk)
END_MESSAGE_MAP()


// CTCEditorLineJumper ��Ϣ�������
BOOL CTCEditorLineJumper::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Get the active MDI child window.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();

	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	// Get the active view attached to the active MDI child window.
	CView *pView = (CView *) pChild->GetActiveView();
	if(pView->IsKindOf(RUNTIME_CLASS(CTCView)))
	{
		m_pTView = (CTCView*)pView;
		ASSERT( m_pTView != NULL );
		int count = m_pTView->m_ScintillaWnd.SendMessage(SCI_GETLINECOUNT);
		if( count > 0 )
		{
			CString str;
			str.Format(_T("�к�:(1 - %d)"),count);
			m_EditorStatic.SetWindowTextW(str);

			int pos = m_pTView->m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS); //�������λ��
			int linecur = m_pTView->m_ScintillaWnd.SendMessage(SCI_LINEFROMPOSITION,pos); //ȡ�õ�ǰ��
			str.Format(_T("%d"),linecur + 1);
			m_EditorLine.SetWindowTextW(str);
		}
		
	}
	return TRUE;
}


void CTCEditorLineJumper::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString lineStr;
	m_EditorLine.GetWindowTextW(lineStr);
	int pos = m_pTView->m_ScintillaWnd.SendMessage(SCI_POSITIONFROMLINE,_wtoi(lineStr) - 1);
	m_pTView->m_ScintillaWnd.SendMessage(SCI_GOTOPOS,pos,NULL);
	CDialogEx::OnOK();
}
