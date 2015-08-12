// MyListBox.cpp : implementation file
//

#include "stdafx.h"
#include "MyListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListBox

CMyListBox::CMyListBox()
{
	m_clrFor=RGB(255,255,0);     
	m_clrBack=RGB(135,206,135);     
    m_brush.CreateSolidBrush(m_clrBack);  
	
}

CMyListBox::~CMyListBox()
{
	if (m_brush.GetSafeHandle())
		m_brush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	//{{AFX_MSG_MAP(CMyListBox)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListBox message handlers

HBRUSH CMyListBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{

	pDC->SetTextColor(m_clrFor);   
	pDC->SetBkColor(m_clrBack);
    return  (HBRUSH) m_brush.GetSafeHandle();  
}

