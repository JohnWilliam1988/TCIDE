#pragma once

struct TCTABPAGEPERPROTY
{
	COLORREF m_color;	//TabPage的颜色
	CString  m_pic;		//TabPage的图片名称
	TCTABPAGEPERPROTY()
	{
		m_color = ::GetSysColor(COLOR_BTNFACE);
		m_pic = _T("");
	}
};
// CTCTabListBox

class CTCTabListBox : public CVSListBox
{

public:
	CTCTabListBox();
	virtual ~CTCTabListBox();

	TCTABPAGEPERPROTY m_pagePerty[32];
private:
	bool m_strateRemove;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnSelectionChanged();
	virtual void OnAfterMoveItemUp(int iItem);
	virtual void OnAfterMoveItemDown(int iItem);
	virtual void OnClickButton(int iButton);
	virtual BOOL RemoveItem(int iIndex);
};


