#pragma once
#include "afxwin.h"
#include "TC.h"
#include "TCView.h"

// CIconListBox
class CTCView;
class CIconListBox : public CListBox
{
	DECLARE_DYNAMIC(CIconListBox)

public:
	CIconListBox();
	virtual ~CIconListBox();

protected:
	DECLARE_MESSAGE_MAP()
	// 图片列表变量
	CImageList* m_pImageList;
public:

	void ShowWindow();
	void HideWindow();
	bool doFilter(const TCHAR * toFilter,int nStart,int nEnd);
	bool TCGetFunList(CString namespDot,int nStart,int nEnd);
	void inserSelected(long nStart,long nEnd,int &d_Value);

	int AddString(LPCTSTR lpszItem);
	int AddString(LPCTSTR lpszItem, int iImg);
	int InsertString(int iIndex, LPCTSTR lpszItem);
	int InsertString(int iIndex, LPCTSTR lpszItem, int iImg);
	void SetItemImage(int iIndex, int iImg);

	inline void SetImageList(CImageList* pImgList= NULL)
	{m_pImageList = pImgList;}


	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

public:
	CTCView* m_pView;
	bool m_isShow;
	int m_Start;
	int m_End;
	HWND m_ParentHandle;
	TCHAR m_tempFilterStr[1024];
	int m_cout;
	CRect m_focusItemRect;
	bool m_ShowExtip;
	CString m_curNamespace;

	//关键字补全 王崧百 2013.1.16
	struct KEYWORDS
	{
		TCHAR keyword[32][32];
		KEYWORDS()
		{
			memset(this,0,sizeof(KEYWORDS));
		}
	};

	//关键字补全 王崧百 2013.1.16
	bool isSpecialKeyWord(CString &str,int spaceCount);
	void KeywordsInit(KEYWORDS &keyWordStr);
	bool KeyComplemented(CString keyStr,int offsetPos);

	KEYWORDS keyWordStr;
	//关键字补全 王崧百 2013.1.16

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLbnSelchange();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


