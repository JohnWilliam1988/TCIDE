// TCGridItems.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "MainFrm.h"
#include "TCGridItems.h"
#include "afxdialogex.h"

#include "DiaTCDesign.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"

// CTCGridItems 对话框

IMPLEMENT_DYNAMIC(CTCGridItems, CDialogEx)

CTCGridItems::CTCGridItems(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCGridItems::IDD, pParent)
{
	m_ctrlID = _T("");
	m_columList = NULL;
	m_gridProperty = NULL;
	m_pGridCtrl = NULL;
	m_curSelectItem=0;
}

CTCGridItems::~CTCGridItems()
{
}

void CTCGridItems::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTCGridItems, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTCGridItems::OnBnClickedOk)
END_MESSAGE_MAP()


// CTCGridItems 消息处理程序


BOOL CTCGridItems::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_columList = new CTCGridListBox();
	m_columList->Create(_T("列名"),WS_VISIBLE|WS_CHILD|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,CRect(10,10,200,260),this);
	m_columList->SetStandardButtons();

	m_gridProperty = new CTCGridProperty();
	m_gridProperty->Create(WS_VISIBLE|WS_CHILD|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,CRect(220,10,450,240),this,65535);
	m_gridProperty->EnableHeaderCtrl(TRUE,_T("参数"),_T("值"));
	//激活描述窗口
	m_gridProperty->EnableDescriptionArea();


	// TODO:  在此添加额外的初始化

	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
	if(!pCurrent)
	{
		return false;
	}

	CControlRect *pRect = pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_ctrlID);
	m_pGridCtrl = (CGridCtrl*)pRect->GetCWnd();

	int count = m_pGridCtrl->GetColumnCount();
	
		for(int col = 0;col < count;col ++)
		{
			m_columList->AddItem(m_pGridCtrl->GetItemText(0,col));
			m_TCGridStruct[col].colwidth = m_pGridCtrl->GetColumnWidth(col);
			m_TCGridStruct[col].title = m_pGridCtrl->GetItemText(0,col);
			m_TCGridStruct[col].type = (TCGRIDCTRLTYPE)m_pGridCtrl->GetItemData(0,col);
			if(m_pGridCtrl->TCGetColReadInfo(0,col))
				m_TCGridStruct[col].readonly = true;
			else
				m_TCGridStruct[col].readonly = false;
	}

	m_gridProperty->TCSetGridColumPerproty(m_columList);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CTCGridItems::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTCGridItems::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = m_columList->GetCount();
	m_pGridCtrl->SetColumnCount(count);
	//m_pGridCtrl->SetItemData();
	
	for(int i=0;i<count;i++)
	{
		m_pGridCtrl->SetItemText(0,i,m_TCGridStruct[i].title);
		for(int j=0;j<m_pGridCtrl->GetRowCount();j++)
		{
			switch(m_TCGridStruct[i].type)
			{
			case TCGRID_CTRL_NOMAL:
				m_pGridCtrl->SetCellType(j,i, RUNTIME_CLASS(CGridCell));
				if(m_TCGridStruct[i].readonly)
				{
					m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) | GVIS_READONLY);
					m_pGridCtrl->TCSetColReadInfo(0,i,TRUE);
				}
				else
					m_pGridCtrl->TCSetColReadInfo(0,i,FALSE);
				m_pGridCtrl->SetItemData(0,i,TCGRID_CTRL_NOMAL);
				break;
			case TCGRID_CTRL_COMBOX:
				if(!m_pGridCtrl->IsCellFixed(j,i))
				{		
					if(m_TCGridStruct[i].readonly)
					{
						m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) | GVIS_READONLY);
						m_pGridCtrl->TCSetColReadInfo(0,i,TRUE);
					}
					else
					{
						m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) & ~GVIS_READONLY);
						m_pGridCtrl->TCSetColReadInfo(0,i,FALSE);
					}
					m_pGridCtrl->SetCellType(j,i, RUNTIME_CLASS(CGridCellCombo));			
				}
				m_pGridCtrl->SetItemData(0,i,TCGRID_CTRL_COMBOX);
				break;
			case TCGRID_CTRL_READONLY:
				if(!m_pGridCtrl->IsCellFixed(j,i))
				{
					m_pGridCtrl->SetCellType(j,i, RUNTIME_CLASS(CGridCell));
					m_pGridCtrl->SetItemText(j,i,_T(""));
					m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i)|GVIS_READONLY);
				}			
				m_pGridCtrl->SetItemData(0,i,TCGRID_CTRL_READONLY);
				break;
			case TCGRID_CTRL_CHECK:
				if(!m_pGridCtrl->IsCellFixed(j,i))
				{
					//m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) & ~GVIS_READONLY);
					if(m_TCGridStruct[i].readonly)
					{
						m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) | GVIS_READONLY);
						m_pGridCtrl->TCSetColReadInfo(0,i,TRUE);
					}
					else
						m_pGridCtrl->TCSetColReadInfo(0,i,FALSE);
					m_pGridCtrl->SetItemText(j,i,_T(""));
					m_pGridCtrl->SetCellType(j,i, RUNTIME_CLASS(CGridCellCheck));
				}
				m_pGridCtrl->SetItemData(0,i,TCGRID_CTRL_CHECK);
				break;
			case TCGRID_CTRL_NUM:
				if(!m_pGridCtrl->IsCellFixed(j,i))
				{
					//m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) & ~GVIS_READONLY);
					if(m_TCGridStruct[i].readonly)
					{
						m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) | GVIS_READONLY);
						m_pGridCtrl->TCSetColReadInfo(0,i,TRUE);
					}
					else
						m_pGridCtrl->TCSetColReadInfo(0,i,FALSE);
					m_pGridCtrl->SetItemText(j,i,_T(""));
					m_pGridCtrl->SetCellType(j,i, RUNTIME_CLASS(CGridCellNumeric));
				}
				m_pGridCtrl->SetItemData(0,i,TCGRID_CTRL_NUM);
				break;
			case TCGRID_CTRL_DATE:			
				if(!m_pGridCtrl->IsCellFixed(j,i))
				{
					if(m_TCGridStruct[i].readonly)
					{
						m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) | GVIS_READONLY);
						m_pGridCtrl->TCSetColReadInfo(0,i,TRUE);
					}
					else
						m_pGridCtrl->TCSetColReadInfo(0,i,FALSE);
					m_pGridCtrl->SetCellType(j,i, RUNTIME_CLASS(CGridCellDateTime));
					CGridCellDateTime* pCell = (CGridCellDateTime*)m_pGridCtrl->GetCell(j,i);
					pCell->SetTime(CTime::GetCurrentTime());
					m_pGridCtrl->RedrawCell(j,i);
				}
				m_pGridCtrl->SetItemData(0,i,TCGRID_CTRL_DATE);
				break;
			//case TCGRID_CTRL_URL:
			//	if(!m_pGridCtrl->IsCellFixed(j,i))
			//	{
			//		//m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) & ~GVIS_READONLY);
			//		if(m_TCGridStruct[i].readonly)
			//		{
			//			m_pGridCtrl->SetItemState(j,i,m_pGridCtrl->GetItemState(j,i) | GVIS_READONLY);
			//			m_pGridCtrl->TCSetColReadInfo(0,i,TRUE);
			//		}
			//		else
			//			m_pGridCtrl->TCSetColReadInfo(0,i,FALSE);
			//		m_pGridCtrl->SetCellType(j,i, RUNTIME_CLASS(CGridURLCell));
			//		m_pGridCtrl->SetItemText(j,i,_T("www.tyuyan.com"));
			//	}
			//	m_pGridCtrl->SetItemData(0,i,TCGRID_CTRL_URL);
			//	break;
			default:
				break;
			}		
			m_pGridCtrl->SetColumnWidth(i,m_TCGridStruct[i].colwidth);
		}
	}
	m_pGridCtrl->Invalidate(TRUE);
	CDialogEx::OnOK();
}
