#pragma once

#include "TCHtmlExample.h"

// CTCFunDecWnd

class CTCFunDecWnd : public CDockablePane
{
	//DECLARE_DYNAMIC(CTCFunDecWnd)

public:
	CTCFunDecWnd();
	 void AdjustLayout();

public:
	 //¹¦ÄÜÃèÊöº¯Êý
	void SetDecText(CString csNS,CString csFunName,CString csFunProto,CString csFunDes,CString csExample);

	virtual ~CTCFunDecWnd();
private:
	CTCHtmlExample m_FunDecWnd;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


