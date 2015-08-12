// TCTabListBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "TCTabListBox.h"

#include "TCTableProperty.h"


// CTCTabListBox

CTCTabListBox::CTCTabListBox()
{
	///////ȫ����ʼ��Ϊϵͳ������ɫ/////////////
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



// CTCTabListBox ��Ϣ�������




BOOL CTCTabListBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

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



////////////////�����ƶ���ʱ�����ɾ��ѡ������½������/////////////
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