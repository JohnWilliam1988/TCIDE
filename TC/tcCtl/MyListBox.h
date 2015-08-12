#if !defined(AFX_MYLISTBOX_H__41550CDC_1BE8_4870_85CC_7FA87EEECEEC__INCLUDED_)
#define AFX_MYLISTBOX_H__41550CDC_1BE8_4870_85CC_7FA87EEECEEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyListBox window

class CMyListBox : public CListBox
{
// Construction
public:
	CMyListBox();
	
	COLORREF   m_clrFor   ;   //   Ç°¾°ÑÕÉ«   
	COLORREF   m_clrBack   ;   //±³¾°ÑÕÉ«   
	CBrush   m_brush   ;   //±³¾°»­Ë¢   

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyListBox();
	/*CBrush	SetBrush(COLORREF nColor);*/

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyListBox)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTBOX_H__41550CDC_1BE8_4870_85CC_7FA87EEECEEC__INCLUDED_)
