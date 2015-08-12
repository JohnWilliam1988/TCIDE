#include "stdafx.h"
#include "Resource.h"
#include "DesignToolBox.h"
#include "TC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

CDesignToolBox::CDesignToolBox()
{
}

CDesignToolBox::~CDesignToolBox()
{
}

BEGIN_MESSAGE_MAP(CDesignToolBox, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDesignToolBox::AdjustLayout()
{
    if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

    m_ToolBox.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CDesignToolBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

    m_ToolBox.Create(IDD_FORMTOOLBOX,this);
    m_ToolBox.ShowWindow(TRUE);
    AdjustLayout();
	return 0;
}

void CDesignToolBox::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
    AdjustLayout();
}

TCTOOLTYPE CDesignToolBox::GetSelectToolType()
{
    return m_ToolBox.m_SelectToolType;
}

void CDesignToolBox::ResetSelectToolType()
{
    m_ToolBox.m_SelectToolType=TCTT_MAX;
}