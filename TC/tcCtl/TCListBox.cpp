// TCListBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../TC.h"
#include "TCListBox.h"


// CTCListBox

//IMPLEMENT_DYNAMIC(CTCListBox, CVSListBox)

CTCListBox::CTCListBox()
{
	m_strSelChange = _T("");
	m_strDoubleClick = _T("");
	m_isAbled = true;
	m_isDefaultRoll = false;
}

CTCListBox::~CTCListBox()
{
}


BEGIN_MESSAGE_MAP(CTCListBox, CVSListBox)
END_MESSAGE_MAP()



// CTCListBox ��Ϣ�������
void CTCListBox::TCSetAbled(bool set)
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

void CTCListBox::TCSetDefaultRool(bool rool)
{
	m_isDefaultRoll = rool;
}