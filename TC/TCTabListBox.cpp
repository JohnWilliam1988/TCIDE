// TCTabListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCTabListBox.h"

#include "TCTableProperty.h"


// CTCTabListBox

CTCTabListBox::CTCTabListBox()
{
	///////全部初始化为系统窗口颜色/////////////
	for(int i = 0;i < 32 ; i++)
	{
		m_pagePerty[i].m_color = ::GetSysColor(COLOR_BTNFACE);
		m_pagePerty[i].m_pic = _T("");
	}
}

CTCTabListBox::~CTCTabListBox()
{
}


BEGIN_MESSAGE_MAP(CTCTabListBox, CVSListBox)
END_MESSAGE_MAP()



// CTCTabListBox 消息处理程序




BOOL CTCTabListBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CVSListBox::PreTranslateMessage(pMsg);
}

void CTCTabListBox::OnSelectionChanged()
{
	int sel = GetSelItem();
	if(sel!=-1)
	{	
		((CTCTableProperty*)GetParent())->m_ProPerty->TCSetTabPerproty(this);
	}
}



////////////////上下移动的时候会先删除选中项，导致进来这儿/////////////
BOOL CTCTabListBox::RemoveItem(int iIndex)
{
	if(m_strateRemove)
	{
		for(int i = iIndex;i < GetCount(); i++)
		{
			m_pagePerty[i].m_color = m_pagePerty[i+1].m_color;
			m_pagePerty[i].m_pic = m_pagePerty[i+1].m_pic;
		}	
	}

	return CVSListBox::RemoveItem(iIndex);
}



void CTCTabListBox::OnClickButton(int iButton)
{
	if(iButton == 2||iButton == 3)
		m_strateRemove = false;
	CVSListBox::OnClickButton(iButton);
}

void CTCTabListBox::OnAfterMoveItemUp(int iItem)
{
	TCTABPAGEPERPROTY temp;
	temp.m_color = m_pagePerty[iItem].m_color;
	temp.m_pic = m_pagePerty[iItem].m_pic;

	m_pagePerty[iItem].m_color = m_pagePerty[iItem+1].m_color;
	m_pagePerty[iItem].m_pic = m_pagePerty[iItem+1].m_pic;

	m_pagePerty[iItem+1].m_color = temp.m_color;
	m_pagePerty[iItem+1].m_pic = temp.m_pic;

	((CTCTableProperty*)GetParent())->m_ProPerty->TCSetTabPerproty(this);
}

void CTCTabListBox::OnAfterMoveItemDown(int iItem)
{
	TCTABPAGEPERPROTY temp;
	temp.m_color = m_pagePerty[iItem].m_color;
	temp.m_pic = m_pagePerty[iItem].m_pic;

	 m_pagePerty[iItem].m_color =  m_pagePerty[iItem-1].m_color;
	 m_pagePerty[iItem].m_pic =  m_pagePerty[iItem-1].m_pic;

	m_pagePerty[iItem-1].m_color = temp.m_color;
	m_pagePerty[iItem-1].m_pic = temp.m_pic;
	((CTCTableProperty*)GetParent())->m_ProPerty->TCSetTabPerproty(this);
}