// SplashWnd.cpp : implementation file
//
// ?998-2001 Codejock Software, All Rights Reserved.
// Based on the Visual C++ splash screen component.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SplashWnd.h"
#include "TCRunTimeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL        CSplashWnd::m_bShowSplashWnd;
CSplashWnd* CSplashWnd::m_pSplashWnd;

CSplashWnd::CSplashWnd()
{
	m_DrawPos = CPoint(149,80);
	m_nCurNum = 0;
	m_nPointNum = 12;
	m_colText = RGB(126,130,133);
	m_hThread = NULL;
	//m_Gif = new CTuiPictureCtrl;
}

CSplashWnd::~CSplashWnd()
{
	//if(m_Gif)
	//	delete m_Gif;
	// Clear the static window pointer.
	ASSERT(m_pSplashWnd == this);
	if(!m_pSplashWnd->m_img.IsNull())
		m_pSplashWnd->m_img.Destroy();	
	m_pSplashWnd = NULL;
	if (m_MemDC.GetSafeHdc())
	{
		m_MemDC.DeleteDC();
	}
	
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	m_bShowSplashWnd = bEnable;
}

void CSplashWnd::HideSplashScreen()
{
	if (m_pSplashWnd != NULL)
	{
		m_pSplashWnd->DestroyWindow();
		AfxGetMainWnd()->UpdateWindow();
	}
}

/********************************************************************
    函数名称  : LoadImageFromResource
    函数描述  : 从资源中加载图片
    输入参数  : pImage -- CImage对象指针
				nResID -- 资源ID
				lpTyp -- 图片类型
    输出参数  : N/A
    返回值    : 真 假 是否加载到图片
    备注      : N/A
*********************************************************************/
BOOL LoadImageFromResource(ATL::CImage *pImage, UINT uResID,LPCTSTR lpTyp)
{
	if ( pImage == NULL) 
	{
		return false;
	}
	pImage->Destroy();
	
	CString strType = lpTyp;
	strType.MakeLower();
	if (strType.CompareNoCase(_T("bmp")) ==0 || strType.Compare(_T("bitmap"))==0)
	{
		pImage->LoadFromResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(uResID));
		if (pImage == NULL || pImage->IsNull())
		{
			return false;
		}
		// 查找资源
		HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(uResID), lpTyp);
		return true;
	}

	// 查找资源
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(uResID), lpTyp);
	if (hRsrc == NULL) return false;

	// 加载资源
	HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid    = ::LockResource(hImgData);

	LPSTREAM pStream = NULL;
	DWORD dwSize    = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew    = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte    = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);

	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if ( ht != S_OK )
	{
		GlobalFree(hNew);
	}
	else
	{
		// 加载图片
		pImage->Load(pStream);

		GlobalFree(hNew);
	}
	// 释放资源
	::FreeResource(hImgData);
	return true;
}

BOOL CSplashWnd::ShowSplashScreen(/*UINT uTimeOut, */UINT uBitmapID, UINT uGifID, CWnd* pParentWnd /*= NULL*/)
{
	ASSERT(/*uTimeOut && */uBitmapID);
	
	if (!m_bShowSplashWnd || m_pSplashWnd != NULL) {
		return FALSE;
	}

	m_pSplashWnd = new CSplashWnd;


	if( !LoadImageFromResource(&(m_pSplashWnd->m_img), uBitmapID,_T("PNG")) )
		return FALSE;

	if( m_pSplashWnd->m_img.IsNull() )
	{
		// Allocate a new splash screen, and create the window.
		return FALSE;
	}

	m_pSplashWnd->m_GifResID = uGifID;


	CString strWndClass = AfxRegisterWndClass(0,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	if (!m_pSplashWnd->CreateEx(0, strWndClass, NULL, WS_POPUP | WS_VISIBLE | WS_EX_LAYERED,
		0, 0, m_pSplashWnd->m_img.GetWidth(), m_pSplashWnd->m_img.GetHeight(), pParentWnd->GetSafeHwnd(), NULL))
	{
		TRACE0("Failed to create splash screen.\n");
		delete m_pSplashWnd;
		return FALSE;
	}
	HRGN cRgn;
	cRgn = CreateRoundRectRgn(0, 0, m_pSplashWnd->m_img.GetWidth(), m_pSplashWnd->m_img.GetHeight(),7,7);
	m_pSplashWnd->SetWindowRgn(cRgn,TRUE);
	DeleteObject(cRgn);
	// Center the window.
	m_pSplashWnd->CenterWindow();
	m_pSplashWnd->UpdateWindow();

	
	// Set a timer to destroy the splash screen.
	//m_pSplashWnd->SetTimer(1, uTimeOut, NULL);

	return TRUE;
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (m_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		m_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}

//void CSplashWnd::HideSplashScreen()
//{
//	// Destroy the window, and update the mainframe.
//	DestroyWindow();
//	AfxGetMainWnd()->UpdateWindow();
//}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);
 	/*CRect rect;
 	GetClientRect(&rect);
 	if( !m_pSplashWnd->m_img.IsNull() )
 	{
 		m_pSplashWnd->m_img.AlphaBlend(dc.GetSafeHdc(),rect,CRect(0,0,m_pSplashWnd->m_img.GetWidth(),m_pSplashWnd->m_img.GetHeight()));
 	}*/
	DrawEllipsis(&dc);
}

void CSplashWnd::DrawEllipsis(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	if (m_MemDC == NULL)
	{
		m_MemDC.CreateCompatibleDC(pDC);
	}	
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	m_MemDC.SelectObject(&bitmap);
	BitBlt(m_MemDC.GetSafeHdc(),0,0,rect.Width(),rect.Height(),pDC->GetSafeHdc(),0,0,SRCCOPY);
	if( !m_pSplashWnd->m_img.IsNull() )
	{
		m_pSplashWnd->m_img.AlphaBlend(m_MemDC.GetSafeHdc(),rect,CRect(0,0,m_pSplashWnd->m_img.GetWidth(),m_pSplashWnd->m_img.GetHeight()-1 ));
	}
	CFont* pOldFont = NULL;
	if (m_font.GetSafeHandle())
	{
		pOldFont = m_MemDC.SelectObject(&m_font);
	}
	else
	{
		pOldFont = m_MemDC.SelectObject(GetParent()->GetFont());
	}
	CPoint pt = m_DrawPos;
	int nMode = m_MemDC.SetBkMode(TRANSPARENT);
	CString strCopyID;
	strCopyID = _T("TC") + CTCRunTimeManager::g_TCVersion + _T("正在初始化工具...");
	CSize szTypeCopy = m_MemDC.GetTextExtent(strCopyID);
	m_MemDC.DrawState(pt, szTypeCopy, strCopyID, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
	CPen cPen;
	CPen* pOldPen = NULL;
	cPen.CreatePen(PS_SOLID,1,m_colText);
	pOldPen = m_MemDC.SelectObject(&cPen);
	int nSpace = (szTypeCopy.cy - 3)/2;
	CRect rcGif(pt.x+szTypeCopy.cx + 5,pt.y + nSpace,pt.x + szTypeCopy.cx + 12 + 5,pt.y + nSpace + 2);
	//if (m_Gif)
	//{
	//	CRect rc;
	//	m_Gif->GetClientRect(&rc);
	//	if (rc.IsRectNull())
	//	{
	//		m_Gif->MoveWindow(rcGif,FALSE);
	//	}
	//}
// 	if (m_nCurNum<=m_nPointNum)
// 	{
// 		CString strText = _T(".");
// 		CSize szExtent = m_MemDC.GetTextExtent(strText);
// 		pt.x+=szTypeCopy.cx + 2;
// 		for (int i=0;i<m_nCurNum;i++)
// 		{
// 			m_MemDC.DrawState(pt, szExtent, strText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
// 			pt.x+=szExtent.cx;
// 		}
// 	}
// 	else
// 	{
// 		m_nCurNum = 0;
// 	}
	m_MemDC.SetBkMode(nMode);
	m_MemDC.SelectObject(pOldPen);
	if (pOldFont)
	{
		m_MemDC.SelectObject(pOldFont);
	}
	cPen.DeleteObject();
	BitBlt(pDC->GetSafeHdc(),0,0,rect.Width(),rect.Height(),m_MemDC.GetSafeHdc(),0,0,SRCCOPY);
	bitmap.DeleteObject();
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//SetTimer(101,500,NULL);
	if (m_font.GetSafeHandle() == NULL)
	{
		VERIFY(m_font.CreateFont(14,0,0,0,10,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,
			_T("宋体"))); 
	}
	//m_hThread = AfxBeginThread(ThreadAnimation,this,0,0,0,NULL);
	//CRect rect;
	//if (m_Gif->Create(_T(""),WS_VISIBLE|WS_CHILD,rect,this,1001))
	//{
	//	if (m_Gif->Load(m_GifResID,_T("gif")))
	//	{
	//		m_Gif->Draw();
	//	}
	//}
	return 0;
}

void CSplashWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
