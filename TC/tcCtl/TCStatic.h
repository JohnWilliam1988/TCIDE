/////////////////////////////////////////////////////////////////////////////
//
//   Filename: TCStatic.h
//  Classname: CTCStatic
//
// Written by: Patrik Svensson (patrik.svensson@home.se)
//
//             This class can be used by anyone for any purpose, 
//             but if you like it, then send me a mail and tell me so ;)
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCSTATIC_H__2125167D_A024_4688_89DC_47BD19919DAA__INCLUDED_)
#define AFX_CTCSTATIC_H__2125167D_A024_4688_89DC_47BD19919DAA__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

// Defines for the font style
#define FS_NORMAL			0x00
#define FS_BOLD				0x01
#define FS_ITALIC			0x02
#define FS_UNDERLINED		0x04
#define FS_STRIKETHROUGH	0x08
#define FS_ANTIALIAS		0x10

// Defines for horizontal alignment
#define FS_CENTER			0x20
#define FS_LEFT				0x40
#define FS_RIGHT			0x80

/////////////////////////////////////////////////////////////////////////////

class CTCStatic : public CStatic
{

// Construction
public:
	CTCStatic();

// Attributes
public:

	bool m_bBold;
	bool m_bItalic;
	bool m_bUnderlined;
	bool m_bStrikethrough;
	bool m_bAntialias;
	bool m_bBgColor;

	bool m_bCenter;
	bool m_bLeft;
	bool m_bRight;

	CString m_szFont;
	DWORD m_dwColor;
	DWORD m_dwBgColor;
	int m_nSize;

    CString m_eClick;  //记录点击事件的处理

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
    void TCSetClickEvent(CString csClick){m_eClick=csClick;}
	void SetFontStyle(DWORD dwStyle);
	void SetBackground(DWORD dwBgColor);
	void SetFontStatic(CString szFont, int nSize, DWORD dwColor, DWORD dwStyle);
	void SetText(CString text);
    void ResetStyle();

    CString TCGetClickEvent(){return m_eClick;}
    DWORD GetBackground() const;
    CString GetFontName();
    int GetFontSize();
    DWORD GetTextColor();
    DWORD GetFontStyle();
    void UpdateSurface();

	virtual ~CTCStatic();
	// Generated message map functions
protected:
	//{{AFX_MSG(CTCStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTSTATIC_H__2125167D_A024_4688_89DC_47BD19919DAA__INCLUDED_)
