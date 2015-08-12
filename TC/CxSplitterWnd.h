#pragma once
// SplitWnd.h : implementation file// 
class CxSplitterWnd : public CSplitterWnd
{
	// Construction
public:
	CxSplitterWnd(void) {  
		//m_cxSplitterGap = 1;  //splitter拖动条的宽度 

		
		// implementation attributes which control layout of the splitter
		//int m_cxSplitter, m_cySplitter;         // size of splitter bar
		//int m_cxBorderShare, m_cyBorderShare;   // space on either side of splitter
		//int m_cxSplitterGap, m_cySplitterGap;   // amount of space between panes
		//int m_cxBorder, m_cyBorder;             // borders in client area
		//m_cyBorderShare=4;
		//m_cyBorderShare=4;
		//m_cxSplitterGap=1;
		m_cySplitter=5;
		m_cySplitterGap=5;
	};
	virtual ~CxSplitterWnd() {};

	// Operationspublic:
	// Overrides// ClassWizard generated virtual function overrides//{{AFX_VIRTUAL(CxSplitterWnd)//}}AFX_VIRTUAL // Implementationpublic:
	// These are the methods to be overridden
	virtual void StartTracking(int ht);
	virtual void StopTracking(BOOL bAccept);
	virtual CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	virtual void SetActivePane( int row, int col, CWnd* pWnd = NULL );
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	// Generated message map functionsprotected:
	//{{AFX_MSG(CxSplitterWnd)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


