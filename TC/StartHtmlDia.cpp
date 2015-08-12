// StartHtmlDia.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "MainFrm.h"
#include "TCRunTimeManager.h"
#include "StartHtmlDia.h"
// CStartHtmlDia 对话框

IMPLEMENT_DYNCREATE(CStartHtmlDia, CDHtmlDialog)

CStartHtmlDia::CStartHtmlDia(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CStartHtmlDia::IDD, CStartHtmlDia::IDH, pParent)
{
	m_colsedAfterLoad = TRUE;
	m_showWhenStart = TRUE;
	m_isLoaded = FALSE;
}

CStartHtmlDia::~CStartHtmlDia()
{

}

void CStartHtmlDia::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_DHtml_CheckBox(pDX, _T("ClosedAfterLoadCheck"), m_colsedAfterLoad);
	DDX_DHtml_CheckBox(pDX, _T("ShowWhenStartCheck"), m_showWhenStart);
}

BOOL CStartHtmlDia::OnInitDialog()
{
	UpdateData(TRUE);
	SetHostFlags( DOCHOSTUIFLAG_FLAT_SCROLLBAR | DOCHOSTUIFLAG_NO3DBORDER );
	m_colsedAfterLoad = CTCRunTimeManager::g_StartPageClose;
	m_showWhenStart   = CTCRunTimeManager::g_StartPageShow;
	CDHtmlDialog::OnInitDialog();
	this->m_pBrowserApp->put_Silent(VARIANT_TRUE);
	//this->m_pBrowserApp->Navigate(L"http://pt.tyuyan.com/pagesoft/soft_regmanage.aspx",0,0,0,0);
	//this->m_pBrowserApp->Navigate(L"pt.tyuyan.com/pagesoft/soft_regmanage.aspx",0,0,0,0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CStartHtmlDia, CDHtmlDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CStartHtmlDia)
	/*DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)*/
	DHTML_EVENT_ONCLICK(_T("ClosedAfterLoadCheck"), ClosedAfterLoadCheck)
	DHTML_EVENT_ONCLICK(_T("ShowWhenStartCheck"), ShowWhenStartCheck)
	DHTML_EVENT_ONCLICK(_T("ProHistroyButton0"), OnProHistroyButton)
	DHTML_EVENT_ONCLICK(_T("ProHistroyButton1"), OnProHistroyButton)
	DHTML_EVENT_ONCLICK(_T("ProHistroyButton2"), OnProHistroyButton)
	DHTML_EVENT_ONCLICK(_T("ProHistroyButton3"), OnProHistroyButton)
	DHTML_EVENT_ONCLICK(_T("UnNamed0"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed1"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed2"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed3"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed4"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed5"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed6"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed7"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed8"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed9"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed10"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("UnNamed11"), OnProUnNamed)
	DHTML_EVENT_ONCLICK(_T("DllCommand0"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand1"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand2"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand3"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand4"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand5"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand6"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand7"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand8"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand9"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand10"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("DllCommand11"), OnDllCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand0"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand1"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand2"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand3"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand4"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand5"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand6"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand7"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand8"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand9"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand10"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("BasicCommand11"), OnBasicCommand)
	DHTML_EVENT_ONCLICK(_T("ScripSkill0"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill1"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill2"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill3"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill4"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill5"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill6"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill7"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill8"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill9"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill10"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("ScripSkill11"), OnScripSkill)
	DHTML_EVENT_ONCLICK(_T("WebGame0"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame1"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame2"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame3"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame4"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame5"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame6"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame7"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame8"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame9"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame10"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebGame11"), OnWebGame)
	DHTML_EVENT_ONCLICK(_T("WebControl0"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl1"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl2"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl3"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl4"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl5"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl6"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl7"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl8"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl9"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl10"), OnWebControl)
	DHTML_EVENT_ONCLICK(_T("WebControl11"), OnWebControl)
END_DHTML_EVENT_MAP()



/***********************************************************************/
/*  \函数名称：TravelDir
/*	\函数功能：复制文件夹及返回其中找到的字串
/*	\参数说明：
	@param     CString strSrcPath 源文件夹
	@param      CStringArray &strFoundArray 找到的串
/***********************************************************************/
bool TraveExampleDir(CString strSrcPath, CStringArray &strFoundArray)
{
	strFoundArray.RemoveAll();

	TCHAR szFind[MAX_PATH]={0};
	TCHAR szFile[MAX_PATH]={0};
	WIN32_FIND_DATA FindFileData;
	wcscpy_s(szFind,strSrcPath);
	wcscat_s(szFind,_T("*.*"));
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);

	if(INVALID_HANDLE_VALUE == hFind)
		return false;

	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0] != _T('.'))
			{
				wcscpy_s(szFile,strSrcPath);
				wcscat_s(szFile,FindFileData.cFileName);
				CString csDelFolder=szFile;
				wcscat_s(szFile,_T("\\"));
				wcscat_s(szFile,_T("*.*"));
				WIN32_FIND_DATA FindProFileData;
				HANDLE hProFind=::FindFirstFile(szFile,&FindProFileData);

				if(INVALID_HANDLE_VALUE == hProFind)
					return false;
				while(TRUE)
				{
					if(FindProFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if(FindProFileData.cFileName[0] != _T('.'))
						{

						}

					}
					else
					{
						CString filePath = FindProFileData.cFileName;
						filePath.Trim();
						filePath.MakeLower();
						if(filePath.Right(7) == _T(".tcproj"))
						{
							CString AddStr; 
							AddStr.Format(_T("%s%s%s"),FindFileData.cFileName , _T("|") , FindProFileData.cFileName);
							strFoundArray.Add(AddStr);
						}		
					}
					if(!FindNextFile(hProFind,&FindProFileData))
						break;
				}
				FindClose(hProFind);
			}
		}
		/*else
		{
			continue;
		}*/
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);

	return true;
}

// CStartHtmlDia 消息处理程序

//HRESULT CStartHtmlDia::OnButtonOK(IHTMLElement* /*pElement*/)
//{
//	OnOK();
//	return S_OK;
//}
//
//HRESULT CStartHtmlDia::OnButtonCancel(IHTMLElement* /*pElement*/)
//{
//	OnCancel();
//	return S_OK;
//}

HRESULT CStartHtmlDia::ClosedAfterLoadCheck(IHTMLElement* pElement)
{
	//将控件中的值保存到关联的变量中
	UpdateData(TRUE);
	CTCRunTimeManager::g_StartPageClose = m_colsedAfterLoad == TRUE ? true : false;
	return S_OK;
}

HRESULT CStartHtmlDia::ShowWhenStartCheck(IHTMLElement *pElement)
{
	//将控件中的值保存到关联的变量中
	UpdateData(TRUE);
	CTCRunTimeManager::g_StartPageShow = m_showWhenStart == TRUE ? true : false;
	return S_OK;
}

HRESULT CStartHtmlDia::OnProHistroyButton(IHTMLElement *pElement)
{
	//将控件中的值保存到关联的变量中
	UpdateData(TRUE);
	HistroyElement(pElement);  
	return S_OK;
}

HRESULT CStartHtmlDia::OnProUnNamed(IHTMLElement *pElement)
{
	UpdateData(TRUE);
	ExampleElement(pElement);	
	return S_OK;
}

HRESULT CStartHtmlDia::OnDllCommand(IHTMLElement *pElement)
{
	UpdateData(TRUE);
	ExampleElement(pElement);	
	return S_OK;
}


HRESULT CStartHtmlDia::OnBasicCommand(IHTMLElement *pElement)
{
	UpdateData(TRUE);
	ExampleElement(pElement);	
	return S_OK;
}
HRESULT CStartHtmlDia::OnScripSkill(IHTMLElement *pElement)
{
	UpdateData(TRUE);
	ExampleElement(pElement);	
	return S_OK;
}

HRESULT CStartHtmlDia::OnWebGame(IHTMLElement *pElement)
{
	UpdateData(TRUE);
	ExampleElement(pElement);	
	return S_OK;
}

HRESULT CStartHtmlDia::OnWebControl(IHTMLElement *pElement)
{
	UpdateData(TRUE);
	ExampleElement(pElement);	
	return S_OK;
}

void  CStartHtmlDia::HistroyElement(IHTMLElement *pElement)
{
	CComQIPtr <IHTMLInputElement> buttonElem;
	buttonElem = pElement;
	CComBSTR valueStr;
	if(buttonElem)
	{
		buttonElem->get_name(&valueStr);
		buttonElem.Release();
		CMainFrame *pFrame = (CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd);
		CString projPath = valueStr;
		::PostMessage(pFrame->GetSafeHwnd(),WM_MYCLOSESTARTVIEW_MESSAGE,0,(LPARAM)projPath.AllocSysString());
	}
}

void  CStartHtmlDia::ExampleElement(IHTMLElement *pElement)
{
	if(pElement)
	{
		CComBSTR valueStr;
		pElement->get_className(&valueStr);
		CMainFrame *pFrame = (CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd);
		CString projPath = valueStr;
		::PostMessage(pFrame->GetSafeHwnd(),WM_MYCLOSESTARTVIEW_MESSAGE,0,(LPARAM)projPath.AllocSysString());
	}
}

BOOL CStartHtmlDia::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//拦截F5，不让刷新
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == 116) || pMsg->message == WM_RBUTTONDOWN  || pMsg->message == WM_RBUTTONDBLCLK )
		return TRUE;

	return CDHtmlDialog::PreTranslateMessage(pMsg);
}



void CStartHtmlDia::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	if( m_isLoaded )
		return;

	IHTMLElement *pEle = NULL;
	HRESULT ret = GetElement(_T("Version"),&pEle);
	if( SUCCEEDED(ret) )
	{
		CString version;
		version.Format(_T("TC%s版"),CTCRunTimeManager::g_TCVersion);
		version = _T("<p id=\"Version\">") + version + _T("</p>");
		pEle->put_outerHTML(_bstr_t(version));
	}

	ret = GetElement(_T("histroyList"),&pEle);
	if( SUCCEEDED(ret) )
	{
		CStringArray recentList;
		((CTCApp*)AfxGetApp())->TCGetRecentFileList(recentList);
		CString html;
		for( int i = 0;i < recentList.GetCount(); i++)
		{			
			CString listStr,projName;
			projName = wcsrchr(recentList[i],'\\');
			int pos = projName.Find(_T("."));
			projName = projName.Mid(1,pos - 1);
			listStr.Format(_T("<input class=\"file\" name=\"%s\" type=\"button\" id=\"ProHistroyButton%d\" value=\"%s\">"),recentList[i],i,projName);
			html += listStr;
		}
		
		html = _T("<div id=\"histroyList\">") + html + _T("</div>");		
		pEle->put_outerHTML(_bstr_t(html));
	}
	//基本语法
	ret = GetElement(_T("con_idname_1"),&pEle);
	if( SUCCEEDED(ret) )
	{
		CString unNameDirPath = CTCRunTimeManager::g_runtimePath + _T("\\Examples\\基本语法\\");
		CStringArray unNameArray;
		if (TraveExampleDir(unNameDirPath,unNameArray))
		{
			CString html;
			int count = unNameArray.GetCount();
			for( int i = 0;i < unNameArray.GetCount(); i++)
			{
				CString unNamePro,ProName,ProPath;
				int pos = unNameArray[i].Find(_T("|"));
				ProName = unNameArray[i].Mid(0,pos);
				ProPath = unNameDirPath + ProName + _T("\\") + unNameArray[i].Mid(pos+1,unNameArray[i].GetLength()-pos);
				unNamePro.Format(_T("<li><a href=\"#\" class=\"%s\" id=\"BasicCommand%d\">%s</a></li>"),ProPath,i,ProName);
				html += unNamePro;	
			}
			html = _T("<ul id=\"con_idname_1\">") + html + _T("</ul>");
			pEle->put_outerHTML(_bstr_t(html));
		}
		
	}
	//脚本技巧
	ret = GetElement(_T("con_idname_2"),&pEle);
	if( SUCCEEDED(ret) )
	{
		CString unNameDirPath = CTCRunTimeManager::g_runtimePath + _T("\\Examples\\脚本技巧\\");
		CStringArray unNameArray;
		if (TraveExampleDir(unNameDirPath,unNameArray))
		{
			CString html;
			for( int i = 0;i < unNameArray.GetCount(); i++)
			{
				CString unNamePro,ProName,ProPath;
				int pos = unNameArray[i].Find(_T("|"));
				ProName = unNameArray[i].Mid(0,pos);
				ProPath = unNameDirPath + ProName + _T("\\") + unNameArray[i].Mid(pos+1,unNameArray[i].GetLength()-pos);
				unNamePro.Format(_T("<li><a href=\"#\" class=\"%s\" id=\"ScripSkill%d\">%s</a></li>"),ProPath,i,ProName);
				html += unNamePro;	
			}
			html = _T("<ul id=\"con_idname_2\" style=\"display:none;\">") + html + _T("</ul>");
			pEle->put_outerHTML(_bstr_t(html));
		}
	}
	//插件应用
	ret = GetElement(_T("con_idname_3"),&pEle);
	if( SUCCEEDED(ret) )
	{
		CString unNameDirPath = CTCRunTimeManager::g_runtimePath + _T("\\Examples\\插件应用\\");
		CStringArray unNameArray;
		if (TraveExampleDir(unNameDirPath,unNameArray))
		{
			CString html;
			for( int i = 0;i < unNameArray.GetCount(); i++)
			{
				CString unNamePro,ProName,ProPath;
				int pos = unNameArray[i].Find(_T("|"));
				ProName = unNameArray[i].Mid(0,pos);
				ProPath = unNameDirPath + ProName + _T("\\") + unNameArray[i].Mid(pos+1,unNameArray[i].GetLength()-pos);
				unNamePro.Format(_T("<li><a href=\"#\" class=\"%s\" id=\"DllCommand%d\">%s</a></li>"),ProPath,i,ProName);
				html += unNamePro;	
			}
			html = _T("<ul id=\"con_idname_3\" style=\"display:none;\">") + html + _T("</ul>");
			pEle->put_outerHTML(_bstr_t(html));
		}
	}
	//游戏应用
	ret = GetElement(_T("con_idname_4"),&pEle);
	if( SUCCEEDED(ret) )
	{
		CString unNameDirPath = CTCRunTimeManager::g_runtimePath + _T("\\Examples\\游戏应用\\");
		CStringArray unNameArray;
		if (TraveExampleDir(unNameDirPath,unNameArray))
		{
			CString html;
			for( int i = 0;i < unNameArray.GetCount(); i++)
			{
				CString unNamePro,ProName,ProPath;
				int pos = unNameArray[i].Find(_T("|"));
				ProName = unNameArray[i].Mid(0,pos);
				ProPath = unNameDirPath + ProName + _T("\\") + unNameArray[i].Mid(pos+1,unNameArray[i].GetLength()-pos);
				unNamePro.Format(_T("<li><a href=\"#\" class=\"%s\" id=\"WebGame%d\">%s</a></li>"),ProPath,i,ProName);
				html += unNamePro;	
			}
			html = _T("<ul id=\"con_idname_4\" style=\"display:none;\">") + html + _T("</ul>");
			pEle->put_outerHTML(_bstr_t(html));
		}
	}
	//网页控件
	ret = GetElement(_T("con_idname_5"),&pEle); 
	if( SUCCEEDED(ret) )
	{
		CString unNameDirPath = CTCRunTimeManager::g_runtimePath + _T("\\Examples\\网页控件\\");
		CStringArray unNameArray;
		if (TraveExampleDir(unNameDirPath,unNameArray))
		{
			CString html;
			for( int i = 0;i < unNameArray.GetCount(); i++)
			{
				CString unNamePro,ProName,ProPath;
				int pos = unNameArray[i].Find(_T("|"));
				ProName = unNameArray[i].Mid(0,pos);
				ProPath = unNameDirPath + ProName + _T("\\") + unNameArray[i].Mid(pos+1,unNameArray[i].GetLength()-pos);
				unNamePro.Format(_T("<li><a href=\"#\" class=\"%s\" id=\"WebControl%d\">%s</a></li>"),ProPath,i,ProName);
				html += unNamePro;	
			}
			html = _T("<ul id=\"con_idname_5\" style=\"display:none;\">") + html + _T("</ul>");
			pEle->put_outerHTML(_bstr_t(html));
		}
	}
	//未分类
	ret = GetElement(_T("con_idname_6"),&pEle);
	if( SUCCEEDED(ret) )
	{
		CString unNameDirPath = CTCRunTimeManager::g_runtimePath + _T("\\Examples\\未分类\\");
		CStringArray unNameArray;
		if (TraveExampleDir(unNameDirPath,unNameArray))
		{
			CString html;
			for( int i = 0;i < unNameArray.GetCount(); i++)
			{
				CString unNamePro,ProName,ProPath;
				int pos = unNameArray[i].Find(_T("|"));
				ProName = unNameArray[i].Mid(0,pos);
				ProPath = unNameDirPath + ProName + _T("\\") + unNameArray[i].Mid(pos+1,unNameArray[i].GetLength()-pos);
				unNamePro.Format(_T("<li><a href=\"#\" class=\"%s\" id=\"UnNamed%d\">%s</a></li>"),ProPath,i,ProName);
				html += unNamePro;	
			}
			html = _T("<ul id=\"con_idname_6\" style=\"display:none;\">") + html + _T("</ul>");
			pEle->put_outerHTML(_bstr_t(html));
		}
	}
	m_isLoaded = TRUE;
	CDHtmlDialog::OnDocumentComplete(pDisp,szUrl);
}

void CStartHtmlDia::OnDestroy()
{
	if( m_hWnd != NULL )
	{
		CTCRunTimeManager::g_StartPageShow = m_showWhenStart == TRUE ? true : false;
		CTCRunTimeManager::g_StartPageClose = m_colsedAfterLoad == TRUE ? true : false;

		if( CTCRunTimeManager::g_StartPageShow )
			CTCRunTimeManager::setMainSettingIni(_T("StartPage"),_T("Show"),_T("True"));
		else
			CTCRunTimeManager::setMainSettingIni(_T("StartPage"),_T("Show"),_T("False"));

		if( CTCRunTimeManager::g_StartPageClose )
			CTCRunTimeManager::setMainSettingIni(_T("StartPage"),_T("Close"),_T("True"));
		else
			CTCRunTimeManager::setMainSettingIni(_T("StartPage"),_T("Close"),_T("False"));
	}
	CDHtmlDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}


void CStartHtmlDia::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDHtmlDialog::OnOK();
}
