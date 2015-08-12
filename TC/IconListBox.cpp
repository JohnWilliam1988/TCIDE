// IconListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "MainFrm.h"
#include "TCView.h"
#include <afxtempl.h>
#include "matchLib.h"
#include "IconListBox.h"
#include "TCRunTimeManager.h"

// CIconListBox

IMPLEMENT_DYNAMIC(CIconListBox, CListBox)

CIconListBox::CIconListBox()
{
	m_pImageList = NULL;
	m_isShow = false;
	::memset(m_tempFilterStr,0,256*sizeof(TCHAR));
	m_focusItemRect.SetRectEmpty();
	KeywordsInit(keyWordStr);
	m_ShowExtip = false;
	m_curNamespace.Empty();
}

CIconListBox::~CIconListBox()
{
}


BEGIN_MESSAGE_MAP(CIconListBox, CListBox)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, &CIconListBox::OnLbnSelchange)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



// CIconListBox 消息处理程序

void CIconListBox::ShowWindow()
{
	m_isShow=true;
	::ShowWindow(m_ParentHandle,SW_SHOWNA);
	CListBox::ShowWindow(TRUE);
}

void CIconListBox::HideWindow()
{
	m_isShow=false;
	::ShowWindow(m_ParentHandle,SW_HIDE);
	CListBox::ShowWindow(SW_HIDE);
}

bool CIconListBox::doFilter(const TCHAR * toFilter,int nStart,int nEnd)
{
    if(!toFilter||!(*toFilter))
        return false;
    ::memset(m_tempFilterStr,0,1024*sizeof(TCHAR));

	while( GetCount() > 0 )
	{
		DeleteString( 0 );
	}
    CString csNSFUN=toFilter;
    bool hasData=false;
    int dotIndex=csNSFUN.Find(_T('.'));
	if(dotIndex!=-1)
	{
		CString nameSP=csNSFUN.Mid(0,dotIndex);
		CString nameFun=csNSFUN.Mid(dotIndex+1,csNSFUN.GetLength()-dotIndex);
		::wcsncpy(m_tempFilterStr,nameFun,1024);
		for( int size = 0; size < CTCRunTimeManager::g_TCAip.m_allApi.size(); size++ )
		{
			for( int nspa = 0; nspa <  CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp.size();  nspa++ )
			{
				CString helpSPName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].namespaceName;
				//if( 0 == helpSPName.CompareNoCase(nameSP) )
				if( 0 == helpSPName.Compare(nameSP) )
				{
					for( int func = 0; func < CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun.size(); func++ )
					{
						CString csFun = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun[ func ].funcName;
						CString temp = csFun.Left(nameFun.GetLength());
						//if ( temp.CompareNoCase(nameFun) == 0 && nameFun.GetLength()!=::wcslen(csFun))
						if ( temp.Compare(nameFun) == 0 && nameFun.GetLength()!=::wcslen(csFun))
						{
							AddString(csFun,1);
	                        hasData=true;
							m_ShowExtip = true;
							m_curNamespace = nameSP;
						}
					}
				}

			}
		}
	}
	else
	{
	    CString nameSP=csNSFUN; 
	    ::wcscpy(m_tempFilterStr,nameSP);
		for( int size = 0; size < CTCRunTimeManager::g_TCAip.m_allApi.size(); size++ )
		{
			for( int nspa = 0; nspa <  CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp.size();  nspa++ )
			{
				CString helpSPName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].namespaceName;
				CString temp = helpSPName.Left(nameSP.GetLength());
				//if( 0 == temp.CompareNoCase(nameSP) )
				if( 0 == temp.Compare(nameSP) )
				{
					//this->InsertItem(this->GetItemCount(),helpSPName,0);
					AddString(helpSPName,0);
					hasData=true;
					m_ShowExtip = false;
				}

			}
		}   
	}

	//关键字补全 王崧百 2013.1.16

	if(KeyComplemented(csNSFUN,0))
	{
		hasData = true;
		m_ShowExtip = false;
	}

    m_Start=nStart;
    m_End=nEnd;
    //this->SetItemState(0,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED); 
    return hasData;
}

bool CIconListBox::TCGetFunList(CString namespDot,/*,CString &csList*/int nStart,int nEnd)
{
	//csList.Empty();
	namespDot.Trim();

	m_Start=nStart;
    m_End=nEnd;

	if(namespDot.Right(1)!=_T("."))
		return false;

	bool hasData=false;

	//this->DeleteAllItems();
	while( GetCount() > 0 )
	{
		DeleteString(0);
	}

	namespDot.Delete(namespDot.GetLength()-1);

	for( int size = 0; size < CTCRunTimeManager::g_TCAip.m_allApi.size(); size++ )
	{
		CString helpDllName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].name;
		for( int nspa = 0; nspa <  CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp.size();  nspa++ )
		{
			CString helpSPName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].namespaceName;
			if( 0 == helpSPName.CompareNoCase(namespDot) )
			{
				for( int func = 0; func < CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun.size(); func++ )
				{
					CString csFun = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun[ func ].funcName;
					//this->InsertItem(this->GetItemCount(),csFun,0);
					AddString(csFun,0);
				}
			}
			
		}
	}
	return hasData;
}

void CIconListBox::inserSelected(long nStart,long nEnd,int &d_Value)
{
	int   nItem=-1;
	CString strFun;
	this->GetText(GetCurSel(),strFun);

	//关键字补全 王崧百 2013.1.16
	int linecur = m_pView->SendEditor(SCI_LINEFROMPOSITION,nEnd); //取得当前行
	int spaceCount =  m_pView->SendEditor(SCI_GETLINEINDENTATION, linecur); //所在行的缩进值  
	int linepos = m_pView->SendEditor(SCI_POSITIONFROMLINE,linecur);   //当前行起始位置   
	bool iskeyword = isSpecialKeyWord(strFun,spaceCount);

	if((*m_tempFilterStr))
		if(::wcsncmp(strFun,m_tempFilterStr,lstrlen(m_tempFilterStr))==0)
		{
			strFun.Delete(0,lstrlen(m_tempFilterStr));
		}
		if(iskeyword)//计算光标位置
		{
			int funlength = strFun.GetLength();
			for(int i=0;i<funlength;i++)
			{
				if(strFun[i]==L')')
				{
					d_Value = nEnd + i;
					break;
				}
			}
		}
		wchar_t space[1024] = {0};   
		::wcscat(space,strFun);
		m_pView->SendEditor(SCI_SETSEL,nEnd,nEnd);
		m_pView->SendEditor(SCI_REPLACESEL, 0, (sptr_t)space);
		m_pView->SendEditor(SCI_SETSEL,nEnd+lstrlen(strFun),nEnd+lstrlen(strFun));
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RemoveTool(m_pView);
}

int CIconListBox::AddString(LPCTSTR lpszItem)
{
	int iRet = CListBox::AddString(lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, -1);
	return iRet;
}

int CIconListBox::AddString(LPCTSTR lpszItem, int iImg)
{
	int iRet = CListBox::AddString(lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, iImg);
	return iRet;
}

int CIconListBox::InsertString(int iIndex, LPCTSTR lpszItem)
{
	int iRet = CListBox::InsertString(iIndex,lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, -1);
	return iRet;
}

int CIconListBox::InsertString(int iIndex, LPCTSTR lpszItem, int iImg)
{
	int iRet = CListBox::InsertString(iIndex,lpszItem);
	if (iRet >= 0)
		SetItemData(iRet, iImg);
	return iRet;
}

void CIconListBox::SetItemImage(int iIndex, int iImg)
{
	SetItemData(iIndex, iImg);
	RedrawWindow();
}
/************************************************************************/
/* 调用以在所有者描述的控件中绘制项                             */
/************************************************************************/
void CIconListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC    = CDC::FromHandle(lpDrawItemStruct->hDC);

	if ((int)lpDrawItemStruct->itemID < 0)
	{
		// If there are no elements in the List Box 
		// based on whether the list box has Focus or not 
		// draw the Focus Rect or Erase it,
		if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
			(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
			!(lpDrawItemStruct->itemState & ODS_FOCUS)) 
		{
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
		}
		return;
	}

	CRect  rcItem(lpDrawItemStruct->rcItem); // To draw the focus rect.
	CRect  rClient(rcItem); // Rect to highlight the Item
	CRect  rText(rcItem); // Rect To display the Text
	CPoint Pt( rcItem.left , rcItem.top+2 ); // Point To draw the Image


	// if the Image list exists for the list box
	// adjust the Rect sizes to accomodate the Image for each item.
	if(m_pImageList)
	{
		rClient.left += 18;
		rText.left += 20;
		//rText.top += 1;
	}
	/*else
	{
		rText.top += 1;
	}*/


	COLORREF crText;
	CString strText;

	// Image information in the item data.
	int iImg = (int)lpDrawItemStruct->itemData;

	// If item selected, draw the highlight rectangle.
	// Or if item deselected, draw the rectangle using the window color.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush br(RGB(255,236,181)/*::GetSysColor(COLOR_HIGHLIGHT)*/);
		pDC->FillRect(&rClient, &br);
		m_focusItemRect = rClient;
	}
	else if (!(lpDrawItemStruct->itemState & ODS_SELECTED) && 
		(lpDrawItemStruct->itemAction & ODA_SELECT)) 
	{
		CBrush br(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&rClient, &br);
	}

	// If the item has focus, draw the focus rect.
	// If the item does not have focus, erase the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&rcItem); 
		m_focusItemRect = rClient;
	}
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
		!(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&rcItem); 
	}

	// To draw the Text set the background mode to Transparent.
	int iBkMode = pDC->SetBkMode(TRANSPARENT);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	else
		crText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	// Get the item text.得到项的文本
	GetText(lpDrawItemStruct->itemID, strText);

	// Setup the text format.安装文本格式
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;


	// if the ImageList is Existing and there is an associated Image
	// for the Item, draw the Image.
	if(m_pImageList && (iImg != -1 ) )
		m_pImageList->Draw(pDC,iImg,Pt,ILD_NORMAL);

	CFont font;
	VERIFY(font.CreateFont(
		16,                    
		0,                    
		0,                    
		0,                    
		FW_NORMAL,             
		FALSE,                 
		FALSE,                 
		0,                    
		ANSI_CHARSET,           
		OUT_DEFAULT_PRECIS,     
		CLIP_DEFAULT_PRECIS,    
		DEFAULT_QUALITY,        
		DEFAULT_PITCH | FF_SWISS,
		_T("MS Shell Dlg 2"))); 

	CFont* OldFont = pDC->SelectObject(&font);
	CSize szText = pDC->GetTextExtent(strText);
	int nSpace = (rText.Height() - szText.cy)/2;
	rText = CRect(rText.left,rText.top + nSpace,rText.right,rText.top + nSpace + szText.cy);
	//Draw the Text画文本
	pDC->DrawText(strText, -1, &rText, nFormat | DT_CALCRECT);
	pDC->DrawText(strText, -1, &rText, nFormat);

	pDC->SetTextColor(crText); 
	pDC->SetBkMode(iBkMode);	

	pDC->SelectObject(OldFont);
	font.DeleteObject();
}

void CIconListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
	LPCTSTR lpszText = (LPCTSTR) lpMeasureItemStruct->itemData;
	ASSERT(lpszText != NULL);
	CSize sz;
	CDC* pDC = GetDC();

	sz = pDC->GetTextExtent(lpszText);

	ReleaseDC(pDC);

	lpMeasureItemStruct->itemHeight = 20;
}

void CIconListBox::OnSetFocus(CWnd* pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);
	RedrawWindow();	
}

void CIconListBox::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);
	//RedrawWindow();	
	HideWindow();
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RemoveTool(m_pView);
}


BOOL CIconListBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return TRUE;
	//return CListBox::OnSetCursor(pWnd, nHitTest, message);
}


void CIconListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nItem = GetCurSel();
	if((nItem >= 0)&&(nItem < this->GetCount()))//选中某一行
	{
		int d_value = 0;
		inserSelected(m_Start,m_End,d_value);
		HideWindow();
		//m_pView->SetFocus();
		m_pView->SendEditor(SCI_GRABFOCUS);
		if(d_value>0)//关键字位移处理 王崧百 2013.1.16
		{
			m_pView->SendEditor(SCI_GOTOPOS,d_value);
		}
	}
	return;
	// CListBox::OnLButtonDblClk(nFlags, point);
}


BOOL CIconListBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RelayEvent(pMsg);

	if( (pMsg->wParam == VK_TAB) && m_isShow)
	{
		int d_value = 0;
		inserSelected(m_Start,m_End,d_value);
		HideWindow();
		m_pView->SendEditor(SCI_GRABFOCUS);
		if(d_value>0)//关键字位移处理 王崧百 2013.1.16
		{
			m_pView->SendEditor(SCI_GOTOPOS,d_value);
		}
		return true;
	}
	if( pMsg->message == WM_KEYDOWN )
	{	
		int listIndex = 0;
		if( GetCurSel() > 0 )
		{
			listIndex = GetCurSel();
		}

		if(VK_UP == pMsg->wParam)
		{
			if( listIndex >= 1 )
				listIndex -= 1;
		}
		else if(VK_DOWN == pMsg->wParam)
		{
			listIndex += 1;
		}
		SetCurSel(listIndex);
		if(!m_curNamespace.IsEmpty())
		{
			CString strSel;
			GetText(GetCurSel(),strSel);
			if( m_curNamespace != strSel)
			{
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RemoveTool(m_pView);
				m_pView->ShowIntelisenseFunExampleWithoutPos(m_curNamespace);
			}		
		}
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RelayEvent(pMsg);
		return true;
	}
	return CListBox::PreTranslateMessage(pMsg);
}


void CIconListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);

	if(nChar == VK_RETURN)
	{
		int d_value = 0;
		inserSelected(m_Start,m_End,d_value);
		HideWindow();
		//m_pView->SetFocus();
		m_pView->SendEditor(SCI_GRABFOCUS);
		if(d_value>0)//关键字位移处理 王崧百 2013.1.16
		{
			m_pView->SendEditor(SCI_GOTOPOS,d_value);
		}
		return ;
	}
	
	//HideWindow();
	//m_pView->SendEditor(SCI_GRABFOCUS);
	//m_pView->m_ScintillaWnd.PostMessageW(WM_KEYDOWN,nChar,0);
}
 

//关键字补全 王崧百 2013.1.16
void CIconListBox::KeywordsInit(KEYWORDS &keyWordStr)
{
	wcscpy(keyWordStr.keyword[0],L"if");
	wcscpy(keyWordStr.keyword[1],L"while");
	wcscpy(keyWordStr.keyword[2],L"for");
	wcscpy(keyWordStr.keyword[3],L"select");
	wcscpy(keyWordStr.keyword[4],L"如果");
	wcscpy(keyWordStr.keyword[5],L"循环");
	wcscpy(keyWordStr.keyword[6],L"遍历");
	wcscpy(keyWordStr.keyword[7],L"选择");
	//wcscpy(keyWordStr.keyword[8],L"else");
	//wcscpy(keyWordStr.keyword[9],L"endif");
	//wcscpy(keyWordStr.keyword[10],L"endwhile");
	//wcscpy(keyWordStr.keyword[11],L"endfor");
	//wcscpy(keyWordStr.keyword[12],L"endselect");
	//wcscpy(keyWordStr.keyword[13],L"如果结束");
	//wcscpy(keyWordStr.keyword[14],L"循环结束");
	//wcscpy(keyWordStr.keyword[15],L"遍历结束");
	//wcscpy(keyWordStr.keyword[16],L"选择结束");
	wcscpy(keyWordStr.keyword[8],L"if()...endif");
	wcscpy(keyWordStr.keyword[9],L"if()...else...endif");
	wcscpy(keyWordStr.keyword[10],L"while()...endwhile");
	wcscpy(keyWordStr.keyword[11],L"for()...endfor");
	wcscpy(keyWordStr.keyword[12],L"select()...endselect");
	wcscpy(keyWordStr.keyword[13],L"如果()...如果结束");
	wcscpy(keyWordStr.keyword[14],L"如果()...否则...如果结束");
	wcscpy(keyWordStr.keyword[15],L"循环()...循环结束");
	wcscpy(keyWordStr.keyword[16],L"遍历()...遍历结束");
	wcscpy(keyWordStr.keyword[17],L"选择()...选择结束");
	wcscpy(keyWordStr.keyword[18],L"array");
	wcscpy(keyWordStr.keyword[19],L"数组");
}

bool CIconListBox::isSpecialKeyWord(CString &str,int spaceCount)
{
	bool ret = false;

	wchar_t space[1024]={0};   
	if(spaceCount < 1024)
	{
		::wmemset(space,L' ',1024);
		space[spaceCount] = 0;
	}

	if(str.Compare(L"if()...endif")==0)
	{
		str = L"if()\n";
		str += space;
		str += L"endif";
		ret = true;
	}
	else if(str.Compare(L"if()...else...endif")==0)
	{
		str = L"if()\n";
		str += space;
		str += L"else\n";
		str += space;
		str += L"endif";
		ret = true;
	}
	else if(str.Compare(L"while()...endwhile")==0)
	{
		str = L"while()\n";
		str += space;
		str += L"endwhile";
		ret = true;
	}
	else if(str.Compare(L"for()...endfor")==0)
	{
		str = L"for()\n";
		str += space;
		str += L"endfor";
		ret = true;
	}
	else if(str.Compare(L"select()...endselect")==0)
	{
		str = L"select()\n";
		str += space;
		str += L"endselect";
		ret = true;
	}
	else if(str.Compare(L"如果()...如果结束")==0)
	{
		str = L"如果()\n";
		str += space;
		str += L"如果结束";
		ret = true;
	}
	else if(str.Compare(L"如果()...否则...如果结束")==0)
	{
		str = L"如果()\n";
		str += space;
		str += L"否则\n";
		str += space;
		str += L"如果结束";
		ret = true;
	}
	else if(str.Compare(L"循环()...循环结束")==0)
	{
		str = L"循环()\n";
		str += space;
		str += L"循环结束";
		ret = true;
	}
	else if(str.Compare(L"遍历()...遍历结束")==0)
	{
		str = L"遍历()\n";
		str += space;
		str += L"遍历结束";
		ret = true;
	}
	else if(str.Compare(L"选择()...选择结束")==0)
	{
		str = L"选择()\n";
		str += space;
		str += L"选择结束";
		ret = true;
	}
	return ret;
}

bool CIconListBox::KeyComplemented(CString keyStr,int offsetPos)
{
	bool isok = false;
	for(int i = 0;i<20;i++)
	{
		if(wcsncmp(keyStr,keyWordStr.keyword[i],wcslen(keyStr))==0)
		{
			//this->InsertItem(this->GetItemCount(),keyWordStr.keyword[i],1);
			AddString(keyWordStr.keyword[i],2);
			isok = true;
		}
	}
	return isok;
}
//关键字补全 王崧百 2013.1.16

void CIconListBox::OnLbnSelchange()
{
	// TODO: 在此添加控件通知处理程序代
}


BOOL CIconListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RemoveTool(m_pView);
	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}


void CIconListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CListBox::OnLButtonDown(nFlags, point);
	if(!m_curNamespace.IsEmpty())
	{
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RemoveTool(m_pView);
		m_pView->ShowIntelisenseFunExampleWithoutPos(m_curNamespace);
	}
}


void CIconListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RemoveTool(m_pView);
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}
