// SplashWnd.h : header file
//
// ?998-2001 Codejock Software, All Rights Reserved.
// Based on the Visual C++ splash screen component.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#ifndef __SPLASHWND__
#define __SPLASHWND__

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

class CSplashWnd : public CWnd
{
protected:

    // -> Remarks:  Constructs a CSplashWnd object.
	CSplashWnd();

    // -> Remarks:  Destroys a CSplashWnd object, handles cleanup and de-allocation.
	virtual ~CSplashWnd();
public:
	int				   m_nCurNum;
	CPoint			   m_DrawPos;
	CWinThread*		   m_hThread;

protected:
	static BOOL        m_bShowSplashWnd; // TRUE if the splash screen is enabled.
	static CSplashWnd* m_pSplashWnd;     // Points to the splash screen.
	CImage			   m_img;
	
	UINT			  m_GifResID;

	int				   m_nPointNum;
	COLORREF		   m_colText;
	CFont			   m_font;
	CDC				   m_MemDC;
private:
	void DrawEllipsis(CDC* pDC);
public:

	// -> Input:   uTimeOut   - Amount of time in milliseconds to display the splash screen.
	//             uBitmapID  - Resource ID of the bitmap to display.
	//             pParentWnd - Parent window of the splash screen, can be NULL.
	// -> Returns: TRUE if successful; otherwise FALSE;
	// -> Remarks: This member function is called to create and display the splash screen
	//             window. It is typically called in your OnInitDialog() for dialog based
	//             applications.
	//
	//             Example:
	//
	//             CSplashWnd::ShowSplashScreen(3000, IDB_SPLASH24, this);
	//
	static BOOL ShowSplashScreen(/*UINT uTimeOut,*/ UINT uBitmapID, UINT uGifID,CWnd* pParentWnd = NULL);

	// -> Input:   bEnable - TRUE to enable the splash screen.
	// -> Remarks: This member function is called to enable the splash screen
	//             component. It is typically called from your CWinApp derived
	//             class InitInstance() method, after the call to ParseCommandLine.
	//
	//             Example:
	//
	//			   CCommandLineInfo cmdInfo;
	//			   ParseCommandLine(cmdInfo);
	//			   CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	//
	static void EnableSplashScreen(BOOL bEnable = TRUE);

	static void HideSplashScreen();

	// -> Input:   A pointer to a MSG structure that contains the message to process.
	// -> Returns: TRUE if successful; otherwise FALSE;
	// -> Remarks: This member function is by your CWinApp derived class from the
	//             overridden CWinApp::PreTranslateMessage. This will route messages
	//             to the splash screen while it is active.
	//
	//             Example:
	//
	//			   BOOL CDialogApp::PreTranslateMessage(MSG* pMsg) 
	//	           {
	//                 // Route messages to the splash screen while it is visible
	//                 if (CSplashWnd::PreTranslateAppMessage(pMsg)) {
	//                     return TRUE;
	//                 }
	//		
	//                 return CWinApp::PreTranslateMessage(pMsg);
	//             }
	//
	static BOOL PreTranslateAppMessage(MSG* pMsg);

protected:

	// -> Remarks: This member function is called internally by the CSplashWnd class
	//             to destroy the splash window once the timer has run out.
	//void HideSplashScreen();

	//{{AFX_VIRTUAL(CSplashWnd)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CSplashWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __SPLASHWND__
