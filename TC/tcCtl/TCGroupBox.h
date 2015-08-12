#if !defined(AFX_TCGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_)
#define AFX_TCGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TCGroupBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTCGroupBox window

class CTCGroupBox : public CButton
{
	DECLARE_DYNAMIC(CTCGroupBox);

// Construction
public:
	CTCGroupBox();

	enum TCGroupBoxStyle
		{ TCGB_FRAME,  
		  TCGB_WINDOW};

// Attributes
public:

// Operations
public:
   virtual CTCGroupBox&	SetBorderColor(COLORREF clrBorder);
   virtual CTCGroupBox&	SetCatptionTextColor(COLORREF clrText);
   virtual CTCGroupBox& SetBackgroundColor(COLORREF clrBKClient);
   virtual CTCGroupBox&	SetBackgroundColor(COLORREF clrBKTilte,  COLORREF clrBKClient);
   virtual CTCGroupBox& SetClientBackgroundColor(COLORREF clrBKClient);
   virtual CTCGroupBox&	SetTCGroupStyle(TCGroupBoxStyle eStyle); 
    
   virtual CTCGroupBox& SetText(LPCTSTR lpszTitle);
   virtual CTCGroupBox& SetFontBold(BOOL bBold);
   virtual CTCGroupBox& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
   virtual CTCGroupBox& SetFontUnderline(BOOL bSet);
   virtual CTCGroupBox& SetFontItalic(BOOL bSet);
   virtual CTCGroupBox& SetFontSize(int nSize);
   virtual CTCGroupBox& SetFont(LOGFONT lf);

   virtual CTCGroupBox& SetAlignment(DWORD dwType);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCGroupBox)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTCGroupBox();

	// Generated message map functions
protected:
	void UpdateSurface();
	void ReconstructFont();
	
	//{{AFX_MSG(CTCGroupBox)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
    CString		m_strTitle;
    void GetText(CString &str);
	void SetFontFaceName(CString str);
	void GetFontFaceName(CString &str);

	LOGFONT GetGroupFont(){
		return m_lf;   }
    DWORD       m_dwAlignment;
    DWORD GetAlignment() const;
    COLORREF	GetBorderColor() const;
    COLORREF	GetTitleBackgroundColor() const;
    COLORREF	GetClientBackgroundColor() const;
    COLORREF	GetTitleTextColor() const;
    TCGroupBoxStyle GetGroupBoxStyle() const;
private:
	COLORREF	m_clrBorder;
	COLORREF	m_clrTitleBackground;
	COLORREF	m_clrClientBackground;
	COLORREF	m_clrTitleText;
	
	TCGroupBoxStyle		m_nType;

	LOGFONT			m_lf;
	CFont			m_font;
	CString			m_fontFaceName;			//保存字体名称的成员

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XPGROUPBOX_H__F70D755B_9C4B_4F4A_A1FB_AFF720C29717__INCLUDED_)
