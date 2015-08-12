// TCListBoxItem.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "MainFrm.h"
#include "TCListBoxItem.h"
#include "afxdialogex.h"

#include "DiaTCDesign.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"




// CTCListBoxItem 对话框

IMPLEMENT_DYNAMIC(CTCListBoxItem, CDialogEx)

CTCListBoxItem::CTCListBoxItem(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCListBoxItem::IDD, pParent)
{
    m_ctrlID = _T("");
    m_pList = NULL;
}

CTCListBoxItem::~CTCListBoxItem()
{
}

void CTCListBoxItem::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_editItem);
}


BEGIN_MESSAGE_MAP(CTCListBoxItem, CDialogEx)
    ON_BN_CLICKED(IDCANCEL, &CTCListBoxItem::OnBnClickedCancel)
    ON_BN_CLICKED(ID_BTNOK, &CTCListBoxItem::OnBnClickedBtnok)
END_MESSAGE_MAP()


// CTCListBoxItem 消息处理程序


void CTCListBoxItem::OnBnClickedCancel()
{
    // TODO: 在此添加控件通知处理程序代码

    CDialogEx::OnCancel();
}


void CTCListBoxItem::OnBnClickedBtnok()
{
    // TODO: 在此添加控件通知处理程序代码
	int size = m_pList->GetCount();
	for(int i=0;i<size;i++)
	{
		m_pList->RemoveItem(0);
	}

	CString csContent=_T("");
	m_editItem.GetWindowText(csContent);
	csContent.Trim();

	if(csContent.IsEmpty())
	{
		CDialog::EndDialog(IDOK);
		return;
	}

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
			m_pList->AddItem(csitem);
        }
    }

	CDialog::EndDialog(IDOK);
}


BOOL CTCListBoxItem::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

    if(!pCurrent)
    {
        return false;
    }

    CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_ctrlID);
    m_pList=(CTCListBox *)pRect->GetCWnd();

    if(m_pList)
    {
        int count=m_pList->GetCount();

        for(int i=0;i<count;i++)
        {
            CString csItem;
			csItem=m_pList->GetItemText(i);
			//m_pList->GetText(i,csItem);
			CString csInput = csItem + _T("\r\n");
			m_editItem.ReplaceSel(csInput);
            //m_ListBoxItem.InsertItem(m_ListBoxItem.GetItemCount(),csItem);
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
