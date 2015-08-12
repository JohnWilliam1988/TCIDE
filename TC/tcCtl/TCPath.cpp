// TCPath.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../TC.h"
#include "TCPath.h"


// CTCPath

//IMPLEMENT_DYNAMIC(CTCPath, CMFCEditBrowseCtrl)

CTCBrowse::CTCBrowse()
{
	m_eEnChange = _T("");
    m_Mode =CMFCEditBrowseCtrl::BrowseMode_File;
	m_isAbled = true;
}

CTCBrowse::~CTCBrowse()
{
}


BEGIN_MESSAGE_MAP(CTCBrowse, CMFCEditBrowseCtrl)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CTCPath ��Ϣ�������

void CTCBrowse::TCSetMode(CMFCEditBrowseCtrl::BrowseMode mode)
{
    m_Mode=mode;
    if(m_Mode==CMFCEditBrowseCtrl::BrowseMode_File)
    {
        this->EnableFileBrowseButton();
    }
    else if(m_Mode==CMFCEditBrowseCtrl::BrowseMode_Folder)
    {
        this->EnableFolderBrowseButton();
    }
}

CMFCEditBrowseCtrl::BrowseMode CTCBrowse::TCGetMode()
{
    return m_Mode;
}


BOOL CTCBrowse::PreTranslateMessage(MSG* pMsg)
{
	 if(pMsg->message==WM_LBUTTONDBLCLK)
		 return TRUE;    
	return CMFCEditBrowseCtrl::PreTranslateMessage(pMsg);
}

void CTCBrowse::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	 
	CRect rect;
	GetWindowRect(&rect);
	point.x+=rect.left;
	point.y+=rect.top;
	GetParent()->ScreenToClient(&point);
	//�򸸴��ڴ�����Ϣ
	::SendMessage(GetParent()->GetSafeHwnd(),WM_RBUTTONUP,nFlags,point.y*65536+point.x);	
	CMFCEditBrowseCtrl::OnRButtonUp(nFlags, point);
}

void CTCBrowse::TCSetAbled(bool set)
{
	if(set)
	{
		ModifyStyle(WS_DISABLED,NULL,NULL);
		m_isAbled = true;
	}
	else
	{
		ModifyStyle(NULL,WS_DISABLED,NULL);
		m_isAbled = false;
	}
	Invalidate(TRUE);
}