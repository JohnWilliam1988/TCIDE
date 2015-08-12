// TCComBoxItem.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "MainFrm.h"
#include "TCComBoxItem.h"
#include "afxdialogex.h"

#include "DiaTCDesign.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"

// CTCComBoxItem �Ի���

IMPLEMENT_DYNAMIC(CTCComBoxItem, CDialogEx)

CTCComBoxItem::CTCComBoxItem(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCComBoxItem::IDD, pParent)
{
    m_ctrlID = _T("");
    m_pCombox = NULL;
}

CTCComBoxItem::~CTCComBoxItem()
{
}

void CTCComBoxItem::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_editItem);
}


BEGIN_MESSAGE_MAP(CTCComBoxItem, CDialogEx)
    ON_BN_CLICKED(IDCANCEL, &CTCComBoxItem::OnBnClickedCancel)
    ON_BN_CLICKED(ID_BTNOK, &CTCComBoxItem::OnBnClickedBtnok)
END_MESSAGE_MAP()


// CTCComBoxItem ��Ϣ�������


void CTCComBoxItem::OnBnClickedCancel()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialogEx::OnCancel();
}


void CTCComBoxItem::OnBnClickedBtnok()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    m_pCombox->ResetContent();
	int count=m_editItem.GetLineCount();

	
    for(int j=0;j<count;j++)
    {
		CString csitem = _T("");
		
		int nCharIndex = m_editItem.LineIndex(j) ;

		int nlen = m_editItem.LineLength(nCharIndex) ;

		m_editItem.GetLine(j,csitem.GetBuffer(nlen),nlen);

		csitem.ReleaseBuffer();
        if(!csitem.IsEmpty())
        {
            m_pCombox->AddString(csitem);
        }
    }
	CDialog::EndDialog(ID_BTNOK);
}


BOOL CTCComBoxItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

    if(!pCurrent)
    {
        return false;
    }

    CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_ctrlID);
    m_pCombox=(CTCComboBox *)pRect->GetCWnd();
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��

    if(m_pCombox)
    {
        int count= m_pCombox->GetCount();

        for(int i=0;i<count;i++)
        {
            CString csItem;
			m_pCombox->GetLBText(i,csItem);
			CString csInput = csItem + _T("\r\n");
			m_editItem.ReplaceSel(csInput);
            //m_ListBoxItem.InsertItem(m_ListBoxItem.GetItemCount(),csItem);
        }
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
