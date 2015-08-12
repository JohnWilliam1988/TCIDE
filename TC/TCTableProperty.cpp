// TCTableProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCTableProperty.h"
#include "afxdialogex.h"

#include "DiaTCDesign.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"

#include "Docommand.h"

// CTCTableProperty 对话框

IMPLEMENT_DYNAMIC(CTCTableProperty, CDialogEx)

CTCTableProperty::CTCTableProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCTableProperty::IDD, pParent)
{
	m_List = NULL;
	m_ProPerty = NULL;
}

CTCTableProperty::~CTCTableProperty()
{
}

void CTCTableProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_TABLE, m_tabList);
	//DDX_Control(pDX, IDC_TABVSLISTBOX, m_List);
}


BEGIN_MESSAGE_MAP(CTCTableProperty, CDialogEx)
	//ON_LBN_SELCHANGE(IDC_LIST_TABLE, &CTCTableProperty::OnLbnSelchangeListTable)
	//ON_BN_CLICKED(IDC_BTNTAB_FOWARD, &CTCTableProperty::OnBnClickedBtntabFoward)
	//ON_BN_CLICKED(IDC_BTNTAB_BACK, &CTCTableProperty::OnBnClickedBtntabBack)
	//ON_BN_CLICKED(IDC_BTNTAB_ADD, &CTCTableProperty::OnBnClickedBtntabAdd)
	//ON_BN_CLICKED(IDC_BTNTAB_DELETE, &CTCTableProperty::OnBnClickedBtntabDelete)
	ON_BN_CLICKED(IDCANCEL, &CTCTableProperty::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTNTAB_OK, &CTCTableProperty::OnBnClickedBtntabOk)
END_MESSAGE_MAP()


// CTCTableProperty 消息处理程序


BOOL CTCTableProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	///////////////////////创建列表框，跟属性框/////////////////////////////
	m_List = new CTCTabListBox();
	m_List->Create(_T("名称"),WS_VISIBLE|WS_CHILD|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,CRect(10,10,200,260),this);
	m_List->SetStandardButtons();

	m_ProPerty = new CTCTabProperty();
	m_ProPerty->Create(WS_VISIBLE|WS_CHILD|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,CRect(220,10,450,240),this,65535);
	m_ProPerty->EnableHeaderCtrl(TRUE,_T("参数"),_T("值"));
	//激活描述窗口
	m_ProPerty->EnableDescriptionArea();
	///////////////////////创建列表框，跟属性框/////////////////////////////

	// TODO:  在此添加额外的初始化
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

    if(!pCurrent)
    {
        return false;
    }

	//CTCTab *pTabCtrl;

    CControlRect* pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_ctrlID);
	m_pTabCtrl = (CTCTab*)pRect->GetCWnd();

	////把tab控件的标签信息显示到listbox中
	for (int i = 0; i < m_pTabCtrl->getTabPageCount(); i++)
	{
		TCITEM item;
		wchar_t buffer[256] = {0};	
		item.pszText = buffer;
		item.cchTextMax = 100;
		item.mask = TCIF_TEXT;
		m_pTabCtrl->GetItem(i, &item);
		CString tabStr = buffer;
		m_List->AddItem(tabStr,i+1);
		//拷贝TabPage颜色
		m_List->m_pagePerty[i].m_color = ((CTCTabPage*)(m_pTabCtrl->m_Pages[i]))->TCGetBkgrColor();
		m_List->m_pagePerty[i].m_pic = ((CTCTabPage*)(m_pTabCtrl->m_Pages[i]))->TCGetBackgroundImage();
	}

	m_ProPerty->TCSetTabPerproty(m_List);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTCTableProperty::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CTCTableProperty::OnBnClickedBtntabOk()
{

	//这儿触发添加消息
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(!pCurrent)
	{
		return;
	}

	TabPropertyCommand *pCommand = new TabPropertyCommand(m_pTabCtrl->GetParent(),m_pTabCtrl,m_List);

	pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
	//((CTCApp*)AfxGetApp())->m_undoManager.DoCommand(pCommand);

	CDialog::EndDialog(IDC_BTNTAB_OK);

	// TODO: 在此添加控件通知处理程序代码	
	
	/*int oldCount=m_pTabCtrl->getTabPageCount();
	
	m_pTabCtrl->DeleteAllItems();

	bool tabArray[MAXTABPAGELENGTH];
	CDialogEx *pageArray[MAXTABPAGELENGTH];
	::memset(pageArray,0,sizeof(CWnd *)*MAXTABPAGELENGTH);

	for(int m=0;m<MAXTABPAGELENGTH;m++)
		tabArray[m]=true;

	int newCount=m_List.GetCount();
	
	for(int i=0;i<newCount;i++)
	{
		CString text=m_List.GetItemText(i);
		int data=m_List.GetItemData(i);
		if(data==0)
		{
			CTCTabPage *page=new CTCTabPage();
			page->Create(IDD_TCTABPAGE, m_pTabCtrl);
			pageArray[i]=page;
		}
		else
		{
			pageArray[i]=m_pTabCtrl->m_Pages[data-1];
			tabArray[data-1]=false;
		}
		
		m_pTabCtrl->InsertItem(m_pTabCtrl->GetItemCount(),text);
	}

	for(int j=0;j<oldCount;j++)
	{
		if(tabArray[j])
		{
			delete m_pTabCtrl->m_Pages[j];
			m_pTabCtrl->m_Pages[j]=0;
		}
	}

	for(int k=0;k<newCount;k++)
	{

		m_pTabCtrl->m_Pages[k]=pageArray[k];
	}

	m_pTabCtrl->TCAdjustRect();
	int selIndex=m_pTabCtrl->getTabPageCount();
	m_pTabCtrl->SetCurSel(selIndex-1);
	m_pTabCtrl->showIndex(selIndex-1);*/

	//CDialogEx::OnOK();
}
