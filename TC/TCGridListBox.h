#pragma once

// CTCGridListBox
#include "TCGridProperty.h"



class CTCGridListBox : public CVSListBox
{
	//DECLARE_DYNAMIC(CTCGridListBox)

public:
	CTCGridListBox();
	virtual ~CTCGridListBox();
protected:
	DECLARE_MESSAGE_MAP()
private:
	bool m_strateRemove;
public:
	virtual int GetCount() const;
	virtual void OnSelectionChanged(); 
	virtual BOOL OnBeforeRemoveItem(int /*iItem*/) { return TRUE; }
	virtual void OnAfterAddItem(int iItem);
	virtual void OnAfterMoveItemUp(int iItem);
	virtual void OnAfterMoveItemDown(int iItem);
	virtual void OnClickButton(int iButton);
	virtual void OnEndEditLabel(LPCTSTR lpszLabel);
	virtual BOOL RemoveItem(int iIndex);
	void TCSetItemText(int index,CString value);
};


