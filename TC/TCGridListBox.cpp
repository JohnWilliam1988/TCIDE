// TCGridListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCGridListBox.h"
#include "TCGridItems.h"


// CTCGridListBox

//IMPLEMENT_DYNAMIC(CTCGridListBox, CVSListBox)

CTCGridListBox::CTCGridListBox()
{
	m_strateRemove = true;
}

CTCGridListBox::~CTCGridListBox()
{
}


BEGIN_MESSAGE_MAP(CTCGridListBox, CVSListBox)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CTCGridListBox 消息处理程序




int CTCGridListBox::GetCount() const
{
	return CVSListBox::GetCount();
}

void CTCGridListBox::TCSetItemText(int index,CString value)
{
	SetItemText(index,value);
}


void CTCGridListBox::OnSelectionChanged()
{
	int sel = GetSelItem();
	if(sel!=-1)
	{	
		((CTCGridItems*)GetParent())->m_gridProperty->TCSetGridColumPerproty(this);
		((CTCGridItems*)GetParent())->m_curSelectItem = sel;
	}
}


////////////////添加项////////////////
void CTCGridListBox::OnAfterAddItem(int iItem)
{
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].title = GetItemText(iItem);
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].colwidth = 44;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].type = TCGRID_CTRL_NOMAL;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].readonly = false;
}

////////////////上下移动的时候会先删除选中项，导致进来这儿/////////////
BOOL CTCGridListBox::RemoveItem(int iIndex)
{
	if(m_strateRemove)
	{
		for(int i = iIndex;i < GetCount(); i++)
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[i] = ((CTCGridItems*)GetParent())->m_TCGridStruct[i+1];
		}	
	}
	
	return CVSListBox::RemoveItem(iIndex);
}

void CTCGridListBox::OnEndEditLabel(LPCTSTR lpszLabel)
{
	CString lable = lpszLabel;
	if(lable.IsEmpty())
		return;
	((CTCGridItems*)GetParent())->m_TCGridStruct[((CTCGridItems*)GetParent())->m_curSelectItem].title = lpszLabel;
	CVSListBox::OnEndEditLabel(lpszLabel);
}

void CTCGridListBox::OnClickButton(int iButton)
{
	if(iButton == 2||iButton == 3)
		m_strateRemove = false;
	CVSListBox::OnClickButton(iButton);
}

void CTCGridListBox::OnAfterMoveItemUp(int iItem)
{
	TCGRIDINFO temp;
	temp.colwidth = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].colwidth;
	temp.title = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].title;
	temp.type = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].type;
	temp.readonly = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].readonly;

	int width = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem+1].colwidth;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].colwidth = width;
	CString text = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem+1].title;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].title = text;
	TCGRIDCTRLTYPE ctrlType = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem+1].type;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].type = ctrlType;
	bool readonly = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem+1].readonly;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].readonly = readonly;

	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem+1].colwidth = temp.colwidth;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem+1].title = temp.title;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem+1].type = temp.type;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem+1].readonly = temp.readonly;
}

void CTCGridListBox::OnAfterMoveItemDown(int iItem)
{
	
	TCGRIDINFO temp;
	temp.colwidth = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].colwidth;
	temp.title = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].title;
	temp.type = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].type;
	temp.readonly = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].readonly;

	int width = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem-1].colwidth;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].colwidth = width;
	CString text = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem-1].title;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].title = text;
	TCGRIDCTRLTYPE ctrlType = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem-1].type;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].type = ctrlType;
	bool readonly = ((CTCGridItems*)GetParent())->m_TCGridStruct[iItem-1].readonly;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem].readonly = readonly;

	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem-1].colwidth = temp.colwidth;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem-1].title = temp.title;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem-1].type = temp.type;
	((CTCGridItems*)GetParent())->m_TCGridStruct[iItem-1].readonly = temp.readonly;
}