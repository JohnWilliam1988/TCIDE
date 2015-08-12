// TCFunDecWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCFunDecWnd.h"


// CTCFunDecWnd

//IMPLEMENT_DYNAMIC(CTCFunDecWnd, CDockablePane)

CTCFunDecWnd::CTCFunDecWnd()
{

}

CTCFunDecWnd::~CTCFunDecWnd()
{
}

void CTCFunDecWnd::AdjustLayout()
{
    if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);	
    m_FunDecWnd.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CTCFunDecWnd::SetDecText(CString csNS,CString csFunName,CString csFunProto,CString csFunDes,CString csExample)
{
	m_FunDecWnd.SetDecText(csNS,csFunName,csFunProto,csFunDes,csExample);
}

int CTCFunDecWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

    m_FunDecWnd.Create(IDD_DIAEXAMPLE,this);
    m_FunDecWnd.ShowWindow(TRUE);
    AdjustLayout();
	return 0;
}

void CTCFunDecWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	// 选项卡控件应覆盖整个工作区:
    AdjustLayout();
}


BEGIN_MESSAGE_MAP(CTCFunDecWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CTCFunDecWnd 消息处理程序


