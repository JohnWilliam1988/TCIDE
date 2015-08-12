// ProjTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "TC.h"
#include "mainfrm.h"
#include "ProjTreeView.h"
#include "FileTreeView.h"
#include "DiaNewFile.h"
#include "DiaNewProj.h"
#include "DiaNewTWin.h"

// CProjTreeView

IMPLEMENT_DYNCREATE(CProjTreeView, CTreeView)

CProjTreeView::CProjTreeView()
{
	m_isNewFolder = false;
	fFunTreePath = _T("");
	fFunTreeName = _T("");
}

CProjTreeView::~CProjTreeView()
{
}

BEGIN_MESSAGE_MAP(CProjTreeView, CTreeView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEACTIVATE()
	///////////////////////////文件夹节点功能函数/////////////////////
	ON_COMMAND(ID_PROJ_FOLDERRENAME, &CProjTreeView::OnProjFolderrename)
	ON_COMMAND(ID_PROJ_DELFOLDER, &CProjTreeView::OnProjDelfolder)
	///////////////////////////项目管理节点功能函数///////////////////
	ON_COMMAND(ID_TC_BUILD, &CProjTreeView::OnTcBuild)
	ON_COMMAND(ID_TC_COMPILE, &CProjTreeView::OnTcCompile)
	ON_COMMAND(ID_TC_CLEAR, &CProjTreeView::OnTcClear)
	ON_COMMAND(ID_TC_DEBUGRUN, &CProjTreeView::OnTcDebug)
	ON_COMMAND(ID_PROJ_NEWFILE, &CProjTreeView::OnProjNewfile)
	ON_COMMAND(ID_PROJ_EXISTFILE, &CProjTreeView::OnProjExistfile)
	ON_COMMAND(ID_PROJ_NEWFOLDER, &CProjTreeView::OnProjNewfolder)
	///////////////////////////文件节点菜单功能函数///////////////////
	ON_COMMAND(ID_PROJ_IMPORT, &CProjTreeView::OnProjImport)
	ON_COMMAND(ID_PROJ_OPENFILE, &CProjTreeView::OnProjOpenfile)
	ON_COMMAND(ID_PROJ_DELETEFILE, &CProjTreeView::OnProjDeletefile)
	ON_COMMAND(ID_PROJ_FILERENAME, &CProjTreeView::OnProjFilerename)
	///////////////////////////资源节点菜单功能///////////////////////
	ON_COMMAND(ID_PROJ_INSERTRC, &CProjTreeView::OnProjInsertrc)
	ON_COMMAND(ID_PROJ_OPENRCFOLDER, &CProjTreeView::OnProjOpenrcfolder)
	ON_COMMAND(ID_PROJ_RCRELOAD, &CProjTreeView::OnProjRcreload)
	ON_COMMAND(ID_PROJ_NEWDIALOG, &CProjTreeView::OnProjNewdialog)
	ON_COMMAND(ID_FILE_NEW, &CProjTreeView::OnFileNew)
	//////////////////////////资源文件节点菜单功能////////////////////
	ON_COMMAND(ID_PROJ_RCFILERENAME, &CProjTreeView::OnProjRcfilerename)
	ON_COMMAND(ID_PROJ_DELETERCFILE, &CProjTreeView::OnProjDeletercfile)
	ON_COMMAND(ID_FILE_OPEN, &CProjTreeView::OnFileOpen)
	ON_COMMAND(ID_EDIT_CUT, &CProjTreeView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CProjTreeView::OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, &CProjTreeView::OnEditClear)
	ON_COMMAND(ID_PROPERTIES, &CProjTreeView::OnProperties)
	//////////////////////////编译时禁用相关菜单功能////////////////////////
	ON_UPDATE_COMMAND_UI(ID_TC_BUILD, &CProjTreeView::OnUpdateTcBuild)
	ON_UPDATE_COMMAND_UI(ID_TC_COMPILE, &CProjTreeView::OnUpdateTcCompile)
	ON_UPDATE_COMMAND_UI(ID_TC_CLEAR, &CProjTreeView::OnUpdateTcClear)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CProjTreeView::OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_TC_DEBUGRUN, &CProjTreeView::OnUpdateTcDebugrun)
END_MESSAGE_MAP()


// CProjTreeView 诊断

#ifdef _DEBUG
void CProjTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CProjTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProjTreeView 消息处理程序


BOOL CProjTreeView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if(pNMHDR->hwndFrom == m_wndProjView.m_hWnd)
	{
		switch (pNMHDR->code)
		{
		case TVN_BEGINLABELEDIT:
			OnTvnBeginlabeleditProjView(pNMHDR, pResult);
			break;
		case TVN_ENDLABELEDIT:
			OnTvnEndlabeleditProjView(pNMHDR, pResult);
			break;
		case NM_DBLCLK:
			OnNMDblclkProjView(pNMHDR, pResult);
			break;
		case NM_CLICK:
			OnClickedProjView(pNMHDR, pResult);
			break;
		default:
			break;
		}
	}
	return CTreeView::OnNotify(wParam, lParam, pResult);
}


void CProjTreeView::OnSize(UINT nType, int cx, int cy)
{
	CTreeView::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
}


int CProjTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	// 创建视图:
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS| TVS_EDITLABELS |TVS_EX_AUTOHSCROLL;

	if (!m_wndProjView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	//加载视图图像:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndProjView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	OnChangeVisualStyle();

	AdjustLayout();
	FillFileView();

	m_hCurTreeItem = NULL;
	return 0;
}

void CProjTreeView::SetImageList(CImageList imgageList)
{
	m_wndProjView.SetImageList(&imgageList, TVSIL_NORMAL);
}

void CProjTreeView::FillFileView()
{
	//项目树
	HTREEITEM hRoot = m_wndProjView.InsertItem(_T("未加载任何项目"), 4, 4);
	m_wndProjView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndProjView.SetItemData(hRoot,FCTREE_PROJMANAGER);
	//文件树
	//HTREEITEM hRoot1 = m_wndFileView.InsertItem(_T("未选中任何源文件"), 0, 0);
	//m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
}

BOOL CProjTreeView::SortTreeList(HTREEITEM treeItem,CTreeCtrl *pmyTreeCtrl)
{
	TVSORTCB tvs; 
	tvs.hParent = treeItem;
	tvs.lpfnCompare = SortTreeCompareProc;
	tvs.lParam = (LPARAM)&m_wndProjView;
	m_wndProjView.SortChildrenCB(&tvs);
	return TRUE;
}

int CALLBACK CProjTreeView::SortTreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    /*int nItem1=0;
    int nItem2=0;*/
    FCTREENODETYPE ht1 = (FCTREENODETYPE)lParam1;
    FCTREENODETYPE ht2 = (FCTREENODETYPE)lParam2;

    return ht2-ht1;
}

//加载项目
void CProjTreeView::ReloadProj()
{
	CFileTreeView* pFileView = (CFileTreeView*)(((CxSplitterWnd*)GetParent())->GetPane(1,0));
	if( pFileView == NULL )
		return;
	m_wndProjView.DeleteAllItems();
	pFileView->m_wndFileView.DeleteAllItems();
	if(CTCRunTimeManager::g_ProjPath!=_T(""))
	{
		CString csProjName;
		csProjName.Format(_T("项目管理-%s"),CTCRunTimeManager::g_ProjName);
		HTREEITEM root=m_wndProjView.InsertItem(csProjName,4,4,TVI_ROOT);
		m_wndProjView.SetItemData(root,FCTREE_PROJMANAGER);
		CString strFile;
		strFile.Format(_T("%s\\"),CTCRunTimeManager::g_ProjSourcePath);
		filterSourcePath(strFile,root);
		m_wndProjView.Expand(root,TVE_EXPAND);
		SortTreeList(root,&m_wndProjView);
	}
	else
	{
		HTREEITEM root=m_wndProjView.InsertItem(_T("未加载任何项目"),4,4,TVI_ROOT);
		m_wndProjView.SetItemData(root,FCTREE_PROJMANAGER);
	}
}

//这个方法是得到一个树形控件节点所代码的文件的路径,是相对路径.
void CProjTreeView::GetItemRelativePatchFromFileClassTree(HTREEITEM item,CString &path)
{
	CString csTempPath=m_wndProjView.GetItemText(item);
	FCTREENODETYPE type=(FCTREENODETYPE)m_wndProjView.GetItemData(item);

	if(FCTREENODETYPE::FCTREE_PROJMANAGER==type)
	{
		path=_T("");
		return;
	}

	path=_T("");

	CString csPath,csOldPath;
	csPath=csTempPath;

	HTREEITEM helpitem=m_wndProjView.GetParentItem(item);
	csTempPath=m_wndProjView.GetItemText(helpitem);
	type=(FCTREENODETYPE)m_wndProjView.GetItemData(helpitem);
	while(FCTREENODETYPE::FCTREE_PROJMANAGER!=type)
	{
		csOldPath=csPath;
		csPath.Format(_T("%s\\%s"),csTempPath,csOldPath);
		helpitem=m_wndProjView.GetParentItem(helpitem);
		type=(FCTREENODETYPE)m_wndProjView.GetItemData(helpitem);
		csTempPath=m_wndProjView.GetItemText(helpitem);
	}
	path.Format(_T("%s"),csPath);
}

//这个方法是得到一个树形控件节点所代表文件的路径,是绝对路径.
void CProjTreeView::GetItemPatchFromFileClassTree(HTREEITEM item,CString &path)
{
	CString csHelpPath;
	GetItemRelativePatchFromFileClassTree(item,csHelpPath);
	csHelpPath.Trim();
	if(!csHelpPath.IsEmpty())
		path.Format(_T("%s\\%s"),CTCRunTimeManager::g_ProjSourcePath,csHelpPath);
	else
		path=CTCRunTimeManager::g_ProjSourcePath;
}

void CProjTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndProjView.SetWindowPos(NULL,rectClient.left + 1,rectClient.top + 1,rectClient.Width() - 1,rectClient.Height() - 1, SWP_NOACTIVATE | SWP_NOZORDER);

}

bool CProjTreeView::DeleteFolder(CString lpPath)
{
	TCHAR szFind[MAX_PATH]={0};
	TCHAR szFile[MAX_PATH]={0};
	WIN32_FIND_DATA FindFileData;
	wcscpy_s(szFind,lpPath);
	wcscat_s(szFind,_T("*.*"));
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);

	if(INVALID_HANDLE_VALUE == hFind)
		return false;

	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!=_T('.'))
			{
				//9-6修改Bug，老是提示无法读源或磁盘
				wcscpy_s(szFile,lpPath);
				wcscat_s(szFile,FindFileData.cFileName);
				CString csDelFolder=szFile;
				wcscat_s(szFile,_T("\\"));
				if(DeleteFolder(szFile))
				{
					wchar_t wpath[MAX_PATH];
					::memset(wpath,0,sizeof(wchar_t)*MAX_PATH);
					LPCTSTR pPath=csDelFolder;
					wcscpy(wpath,pPath);

					SHFILEOPSTRUCT op;
					op.fFlags = FOF_NOCONFIRMATION;
					op.hNameMappings = NULL;
					op.hwnd = NULL;
					op.lpszProgressTitle = NULL;
					op.pFrom = wpath;
					op.pTo = wpath;
					op.wFunc = FO_DELETE;    
					int nRet=::SHFileOperation(&op); 
					if(nRet!=0)
						return false;
				}
				else
					return false;
			}
		}
		else
		{
			CString csFileName=FindFileData.cFileName;
			csFileName.Trim();
			CString csFileEx=csFileName.Right(2);
			CString csTFile;
			csTFile.Format(_T("%s%s"),lpPath,csFileName);
			if(0==::wcscmp(csFileEx,_T(".t")))
			{
				::DeleteFile(csTFile);
				((CTCApp*)AfxGetApp())->TCDeleteTFileView(csTFile);
			}
			::DeleteFile(csTFile);
		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);

	return true;
}

void CProjTreeView::filterSourcePath(CString lpPath,HTREEITEM item)
{
	TCHAR szFind[MAX_PATH]={0};
	TCHAR szFile[MAX_PATH]={0};
	WIN32_FIND_DATA FindFileData;
	wcscpy_s(szFind,lpPath);
	wcscat_s(szFind,_T("*.*"));
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);

	if(INVALID_HANDLE_VALUE == hFind)
		return;

	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!=_T('.'))
			{
				HTREEITEM childItem=m_wndProjView.InsertItem(FindFileData.cFileName,0,0,item);
				m_wndProjView.SetItemData(childItem,FCTREE_FOLDER);
				wcscpy_s(szFile,lpPath);
				wcscat_s(szFile,FindFileData.cFileName);
				wcscat_s(szFile,_T("\\"));
				if(CTCRunTimeManager::isSameFile(FindFileData.cFileName,_T("资源")))
				{
					filterRCPath(szFile,childItem);
				}
				else
				{
					filter(szFile,childItem);
				}
				SortTreeList(childItem,&m_wndProjView);
				//m_wndProjView.Expand(childItem,TVE_EXPAND);
			}
		}
		else
		{
			CString csFileName=FindFileData.cFileName;
			csFileName.Trim();
			CString csFileEx=csFileName.Right(2);
			if(0==::wcscmp(csFileEx,_T(".t")))
			{
				HTREEITEM funcItem=m_wndProjView.InsertItem(FindFileData.cFileName,1,1,item);
				m_wndProjView.SetItemData(funcItem,FCTREE_FILE);
			}
		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);
}


//过滤资源目录,加载到树形控件
void CProjTreeView::filterRCPath(CString lpPath,HTREEITEM item)
{
	TCHAR szFind[MAX_PATH]={0};
	TCHAR szFile[MAX_PATH]={0};
	WIN32_FIND_DATA FindFileData;
	wcscpy_s(szFind,lpPath);
	wcscat_s(szFind,_T("*.*"));
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);

	if(INVALID_HANDLE_VALUE == hFind)
		return;

	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!=_T('.'))
			{
				//HTREEITEM childItem=m_wndProjView.InsertItem(FindFileData.cFileName,0,0,item);
				//m_wndProjView.SetItemData(childItem,FCTREE_FOLDER);
				//wcscpy_s(szFile,lpPath);
				//wcscat_s(szFile,FindFileData.cFileName);
				//wcscat_s(szFile,_T("\\"));
				//filter(szFile,childItem);
				//SortTreeList(childItem,&m_wndProjView);
			}
		}
		else
		{
			CString csFileName=FindFileData.cFileName;
			csFileName.Trim();
			csFileName.MakeLower();
			CString csFileEx=csFileName.Right(5);
			CString csPictEx=csFileName.Right(4);
			csPictEx.MakeLower();
			if(0==::wcscmp(csFileEx,_T(".twin")))
			{				
				//////////////////////////////////遍历面板管理类添加界面节点/////////////////////////////////
				int count = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getCount();
				for(int i = 0;i < count;i++)
				{
					wndNode *temp = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(i);		
					HTREEITEM funcItem=m_wndProjView.InsertItem(temp->id,3,3,item);
					m_wndProjView.SetItemData(funcItem,FCTREE_RCWIN);
				}
				//////////////////////////////////遍历面板管理类添加界面节点/////////////////////////////////
			}
			else if(0==::wcscmp(csPictEx,_T(".jpg"))||
				0==::wcscmp(csPictEx,_T(".bmp"))||
				0==::wcscmp(csPictEx,_T(".png"))||
				0==::wcscmp(csPictEx,_T(".ico")))
			{
				HTREEITEM funcItem=m_wndProjView.InsertItem(FindFileData.cFileName,5,5,item);
				m_wndProjView.SetItemData(funcItem,FCTREE_RCFILE);
			}
			else
			{
				HTREEITEM funcItem=m_wndProjView.InsertItem(FindFileData.cFileName,6,6,item);
				m_wndProjView.SetItemData(funcItem,FCTREE_RCFILE);
			}

		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);
}

void CProjTreeView::filter(CString lpPath,HTREEITEM item)
{
	TCHAR szFind[MAX_PATH]={0};
	TCHAR szFile[MAX_PATH]={0};
	WIN32_FIND_DATA FindFileData;
	wcscpy_s(szFind,lpPath);
	wcscat_s(szFind,_T("*.*"));
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);

	if(INVALID_HANDLE_VALUE == hFind)
		return;

	while(TRUE)
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!=_T('.'))
			{
				HTREEITEM childItem=m_wndProjView.InsertItem(FindFileData.cFileName,0,0,item);
				m_wndProjView.SetItemData(childItem,FCTREE_FOLDER);
				wcscpy_s(szFile,lpPath);
				wcscat_s(szFile,FindFileData.cFileName);
				wcscat_s(szFile,_T("\\"));
				filter(szFile,childItem);
				SortTreeList(childItem,&m_wndProjView);
			}
		}
		else
		{
			CString csFileName=FindFileData.cFileName;
			csFileName.Trim();
			csFileName.MakeLower();
			CString csFileEx=csFileName.Right(2);
			if(0==::wcscmp(csFileEx,_T(".t")))
			{
				HTREEITEM funcItem=m_wndProjView.InsertItem(FindFileData.cFileName,1,1,item);
				m_wndProjView.SetItemData(funcItem,FCTREE_FILE);
			}
		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);
}

bool CProjTreeView::FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd)
{
	HTREEITEM m_RootItem = m_wndProjView.GetRootItem();
	if( m_wndProjView.GetChildItem(m_RootItem) != NULL)
	{
		if(TravelFileTree(m_RootItem,oldPatn,newName,FileOrWnd))
		{
			return true;
		}
	}
	else 
		return false;	

	return false;
}

bool CProjTreeView::TravelFileTree(HTREEITEM hItem,CString oldPatn,CString newName,BOOL FileOrWnd)
{
	if( m_wndProjView.ItemHasChildren(hItem) )
	{
		if(FileOrWnd)
		{
			HTREEITEM ItemChild = m_wndProjView.GetChildItem(hItem);
			CString ItemPath=_T("");// 节点的路径
			while( ItemChild != NULL )
			{
				//得到子节点的路径
				GetItemPatchFromFileClassTree(ItemChild,ItemPath);
				CString csFileFloder = _T("");
				GetItemPatchFromFileClassTree(m_wndProjView.GetParentItem(ItemChild),csFileFloder);
				if( 0==ItemPath.CompareNoCase(oldPatn))
				{
					//新的重命名的文件路径
					CString csFileNewPath = _T("");
					csFileNewPath.Format(_T("%s\\%s"),csFileFloder,newName);
					//重命名操作
					TCHAR oldpath[MAX_PATH];
					TCHAR newPath[MAX_PATH];

					int oldLen=ItemPath.GetLength();
					int newLen=csFileNewPath.GetLength();

					::wcscpy_s(oldpath,ItemPath);
					oldpath[oldLen]=0;
					oldpath[oldLen+1]=0;
					::wcscpy_s(newPath,csFileNewPath);
					newPath[newLen]=0;
					newPath[newLen+1]=0;

					SHFILEOPSTRUCT lpsh;
					ZeroMemory(&lpsh,sizeof(lpsh));
					lpsh.hwnd= NULL;
					lpsh.fFlags=FOF_NOCONFIRMATION;
					lpsh.wFunc=FO_RENAME;
					lpsh.pFrom=oldpath;
					lpsh.pTo =newPath;
					if(0!=SHFileOperation(&lpsh))
					{
						CString csError;
						csError.Format(_T("重命名 %s 失败!"),ItemPath);
						::AfxMessageBox(csError);
						return false;
					}
					else
					{
						((CTCApp*)AfxGetApp())->TCRenameTFile(oldpath,newPath);
						m_wndProjView.SetItemText(ItemChild,newName);
						return true;
					}
				}

				//这儿递归进去
				TravelFileTree(ItemChild,oldPatn,newName,TRUE);
				ItemChild = m_wndProjView.GetNextItem(ItemChild,TVGN_NEXT);
			}
		}
		else
		{
			HTREEITEM ItemChild = m_wndProjView.GetChildItem(hItem);
			while( ItemChild != NULL )
			{	
				if(m_wndProjView.GetItemText(ItemChild) == _T("资源") && m_wndProjView.GetItemData(ItemChild) == FCTREE_FOLDER)
				{
					ItemChild = m_wndProjView.GetChildItem(ItemChild);
					while( ItemChild!= NULL )
					{
						CString oldStr = m_wndProjView.GetItemText(ItemChild);
						if( 0 == oldPatn.CompareNoCase(oldStr))
						{
							m_wndProjView.SetItemText(ItemChild,newName);
							return true;
						}
						ItemChild = m_wndProjView.GetNextItem(ItemChild,TVGN_NEXT);
					}				
				}
				ItemChild = m_wndProjView.GetNextItem(ItemChild,TVGN_NEXT);
			}

		}
	}
}

//点击项目管理树
void CProjTreeView::OnClickedProjView(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_wndProjView.ScreenToClient(&pt);

	//不知道为什么必须要unFlags,要不然得到一直是0
	UINT unFlags = 0 ;
	m_hCurTreeItem = m_wndProjView.HitTest(pt, &unFlags) ;

	if(m_hCurTreeItem!=NULL)
	{
		FCTREENODETYPE itemData=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);

		//得到当前文件的路径
		CString csPath;
		GetItemPatchFromFileClassTree(m_hCurTreeItem,csPath);

		CString csName=m_wndProjView.GetItemText(m_hCurTreeItem);
		if(itemData==FCTREENODETYPE::FCTREE_PROJMANAGER)
		{    
			OnProperties();
			m_hCurTreeItem=0;
		}
		else if( itemData==FCTREENODETYPE::FCTREE_FILE )
		{
			ReloadSPandFunList();
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(csName,csPath);
			m_hCurTreeItem=0;

		}
		else if(itemData==FCTREENODETYPE::FCTREE_RCWIN)
		{
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetRcNodeDiaProperty(csName);
			//POSITION pos=pDocTmp->GetFirstDocPosition();
			//CTCDesignView *pView=NULL;
			//CTCDesignDoc* pDoc = NULL;
			//while (NULL != pos)
			//{
			//	pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
			//	if(pDoc==NULL)
			//		continue;
			//	POSITION posView = pDoc->GetFirstViewPosition();
			//	while (NULL != posView)
			//	{
			//		pView = (CTCDesignView *)pDoc->GetNextView(posView);
			//		if (NULL == pView )
			//			continue;
			//		//如果当前视图存在则显示窗口属性,不用做打开操作
			//		if(csName == pView->m_TWinID)
			//		{			
			//			CMDIChildWnd *pChild=(CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
			//			CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
			//			//如果当前视图不是活动视图，则激活
			//			if(pChild->GetActiveView()->m_hWnd != pView->m_hWnd)
			//				pFWnd->MDIActivate();
			//			//否则更新窗口属性
			//			else
			//				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetRcNodeDiaProperty(csName);
			//				//((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(csName,&pView->m_DiaDesign/*,TRUE*/);		
			//		}
			//	}
			//}		
			m_hCurTreeItem=0;
		}
		else 
		{
			m_hCurTreeItem=0;
			return;
		}
	}
	*pResult = 0;
}

void CProjTreeView::ReloadSPandFunList()
{
	CFileTreeView* pFileView = (CFileTreeView*)(((CxSplitterWnd*)GetParent())->GetPane(1,0));
	if( pFileView == NULL )
		return;

	pFileView->m_wndFileView.DeleteAllItems();
	CString filePath;
	HTREEITEM mySelected = m_hCurTreeItem;
	CString fileName = m_wndProjView.GetItemText(mySelected);
	GetItemPatchFromFileClassTree(mySelected,filePath);

	fFunTreePath = filePath;
	fFunTreeName = fileName;

	if(!fFunTreePath.IsEmpty())
	{
		::memset(&m_FunTreeInfor,0,sizeof(MySourceInfor));
		MyEventScanner fScer;
		fScer.scanDocument(filePath);
		::memcpy(&m_FunTreeInfor,&(fScer.fScanInfor),sizeof(MySourceInfor));

		CString csCaption;
		if(fScer.fScanInfor.getLength()==0)
		{
			csCaption.Format(_T("无法获得空间函数-%s"),fileName);
			HTREEITEM root = pFileView->m_wndFileView.InsertItem(csCaption,1,1,TVI_ROOT);
			return;
		}

		csCaption.Format(_T("空间函数列表-%s"),fileName);
		HTREEITEM root = pFileView->m_wndFileView.InsertItem(csCaption,1,1,TVI_ROOT);

		for(int i=0;i<fScer.fScanInfor.getLength();i++)
		{         
			const MyNsInfor *ns = fScer.fScanInfor.getNsInfor(i);
			HTREEITEM nameSP = pFileView->m_wndFileView.InsertItem(ns->getNSName(),7,7,root);
			pFileView->m_wndFileView.SetItemData(nameSP,FCTREE_NAMESPACE);
			for(int j=0;j<ns->getLength();j++)
			{
				const MyFunInfor *fun=ns->getFunPrototype(j);
				CString csInput;
				csInput.Format(L"%s%s",fun->fFunName,fun->fFunParam);
				HTREEITEM funItem = pFileView->m_wndFileView.InsertItem(csInput,8,8,nameSP);
				pFileView->m_wndFileView.SetItemData(funItem,FCTREE_FUNCTION);
			}
			pFileView->m_wndFileView.Expand(nameSP,TVE_EXPAND); 
		}  
		pFileView->m_wndFileView.Expand(root,TVE_EXPAND);
	}
}

void CProjTreeView::OnNMDblclkProjView(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM helpitem=m_wndProjView.GetSelectedItem();
	CString csFileName=m_wndProjView.GetItemText(helpitem);

	FCTREENODETYPE type=(FCTREENODETYPE)m_wndProjView.GetItemData(helpitem);

	if(FCTREENODETYPE::FCTREE_FILE==type)
	{
		CString csPath;
		GetItemPatchFromFileClassTree(helpitem,csPath);
		//这儿打开有问题???
		((CTCApp*)AfxGetApp())->TCOpenTFile(csPath);
		//9-13这儿打开同步属性
		//((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(csFileName,csPath);
		m_hCurTreeItem=0;
	}
	else if(FCTREENODETYPE::FCTREE_RCWIN==type)
	{
		((CTCApp*)AfxGetApp())->TCOpenTWin(csFileName);
		m_hCurTreeItem=0;
	}
	else if(FCTREENODETYPE::FCTREE_RCFILE==type)
	{
		CString csPath;
		GetItemPatchFromFileClassTree(helpitem,csPath);
		CString csPictEx=csFileName.Right(4);
		csPictEx.MakeLower();
		if(0==::wcscmp(csPictEx,_T(".jpg"))||
			0==::wcscmp(csPictEx,_T(".bmp"))||
			0==::wcscmp(csPictEx,_T(".png"))||
			0==::wcscmp(csPictEx,_T(".ico")))
			ShellExecute(NULL, _T("open"), csPath, NULL, NULL, SW_SHOWNORMAL); 
	}
}

void CProjTreeView::OnTvnBeginlabeleditProjView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_wndProjView.GetEditControl()->LimitText(MAX_PATH);

	m_hCurTreeItem=pTVDispInfo->item.hItem;

	*pResult = 0;
}

void CProjTreeView::OnTvnEndlabeleditProjView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if((m_hCurTreeItem==pTVDispInfo->item.hItem)&&(m_hCurTreeItem!=NULL))
	{
		CString csOldName=m_wndProjView.GetItemText(m_hCurTreeItem);
		CString csNewName=pTVDispInfo->item.pszText;

		csOldName.Trim();
		csNewName.Trim();

		if(CTCRunTimeManager::isSameFile(csOldName,csNewName))
		{
			m_hCurTreeItem=0;
			m_isNewFolder=false;
			return;
		}

		//如果是想重命名为 项目管理 文件夹,直接退出
		FCTREENODETYPE itemData=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
		if(itemData==FCTREENODETYPE::FCTREE_PROJMANAGER)
		{           
			m_hCurTreeItem=0;
			m_isNewFolder=false;
			return;
		}

		//如果是想重命名 资源 文件夹,直接退出
		if(itemData==FCTREENODETYPE::FCTREE_FOLDER&&csOldName==_T("资源")&&(FCTREENODETYPE::FCTREE_PROJMANAGER==(FCTREENODETYPE)m_wndProjView.GetItemData(m_wndProjView.GetParentItem(m_hCurTreeItem))))
		{
			m_hCurTreeItem=0;
			m_isNewFolder=false;
			return;
		}

		//判断不能修改启动文件
		//得到以前的路径名
		CString csOldPath;
		GetItemPatchFromFileClassTree(m_hCurTreeItem,csOldPath);

		if(CTCRunTimeManager::isSameFile(csOldPath,CTCRunTimeManager::g_projEnterFile))
		{
			//::AfxMessageBox(_T("不能重命名启动文件!"));
			m_isNewFolder=false;
			return;
		}

		if(pTVDispInfo->item.pszText)
		{
			m_wndProjView.SetItemText(m_hCurTreeItem,pTVDispInfo->item.pszText);
		}

		//得到当前的路径名
		CString csFileName;    
		GetItemPatchFromFileClassTree(m_hCurTreeItem,csFileName);

		//这里是处理新建文件夹的情况,因为这个时候"新建文件夹"是不存在的
		if(m_isNewFolder)
		{
			if(itemData==FCTREENODETYPE::FCTREE_FOLDER)
			{
				//if(csNewName.IsEmpty())
				//{
				//    m_wndProjView.DeleteItem(m_hCurTreeItem);
				//    m_isNewFolder=false;
				//    m_hCurTreeItem=0;
				//    return;
				//}

				if(::PathFileExists(csFileName))
				{
					CString csError;
					m_wndProjView.DeleteItem(m_hCurTreeItem);
					m_hCurTreeItem=0;
					m_isNewFolder=false;
					csError.Format(_T("路径 %s 已经存在,操作失败!"),csFileName);
					::AfxMessageBox(csError);
					return;
				}

				//判断文件夹创建是否成功
				if(0==CreateDirectory(csFileName,NULL))   
				{
					m_wndProjView.DeleteItem(m_hCurTreeItem);
					AfxMessageBox(_T("创建项目文件夹失败,请重试!")); 
				}
				m_hCurTreeItem=0;
				m_isNewFolder=false;
				return;
			}

			AfxMessageBox(_T("执行到这里表示有问题!")); 
			m_hCurTreeItem=0;
			m_isNewFolder=false;
			return;
		}

		//为空不做任何操作,直接返回
		if(csNewName.IsEmpty())
		{
			m_wndProjView.SetItemText(m_hCurTreeItem,csOldName);
			m_isNewFolder=false;
			m_hCurTreeItem=0;
			return;
		}

		if( itemData==FCTREENODETYPE::FCTREE_FILE||itemData==FCTREENODETYPE::FCTREE_FOLDER||itemData==FCTREENODETYPE::FCTREE_RCFILE)
		{
			CString csTFile =  csNewName.Right(2);
			if( itemData==FCTREENODETYPE::FCTREE_FILE && csTFile != _T(".t"))
			{
				m_wndProjView.SetItemText(m_hCurTreeItem,csOldName);
				m_hCurTreeItem=0;
				m_isNewFolder=false;
				return;
			}

			//这里判断新建的文件和文件夹操作,是不是已经存在,存在的话就直接退出
			if(::PathFileExists(csFileName))
			{
				CString csError;
				m_wndProjView.SetItemText(m_hCurTreeItem,csOldName);
				csError.Format(_T("%s 已经存在,修改失败!"),csFileName);
				::AfxMessageBox(csError);
				m_hCurTreeItem=0;
				m_isNewFolder=false;
				return;
			}

			//重命名操作
			TCHAR oldpath[MAX_PATH];
			TCHAR newPath[MAX_PATH];

			int oldLen=csOldPath.GetLength();
			int newLen=csFileName.GetLength();

			::wcscpy_s(oldpath,csOldPath);
			oldpath[oldLen]=0;
			oldpath[oldLen+1]=0;
			::wcscpy_s(newPath,csFileName);
			newPath[newLen]=0;
			newPath[newLen+1]=0;

			SHFILEOPSTRUCT lpsh;
			ZeroMemory(&lpsh,sizeof(lpsh));
			lpsh.hwnd= NULL;
			lpsh.fFlags=FOF_NOCONFIRMATION;
			lpsh.wFunc=FO_RENAME;
			lpsh.pFrom=oldpath;
			lpsh.pTo =newPath;
			if(0!=SHFileOperation(&lpsh))
			{
				CString csError;
				csError.Format(_T("重命名 %s 失败!"),csOldName);
				::AfxMessageBox(csError);
				m_wndProjView.SetItemText(m_hCurTreeItem,csOldName);
				m_hCurTreeItem=0;
				m_isNewFolder=false;
				return;
			}
			else
			{
				if( itemData==FCTREENODETYPE::FCTREE_FILE)
					((CTCApp*)AfxGetApp())->TCRenameTFile(csOldPath,csFileName);
				//这儿和属性框同步
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(csNewName,newPath);
			}
		}
		else if(itemData==FCTREENODETYPE::FCTREE_RCWIN)
		{
			//这里要处理 twin 文件的重命名操作
			//如果重命名的是项目主窗口则返回
			if(csOldName == CTCRunTimeManager::g_ProjName)
			{
				m_wndProjView.SetItemText(m_hCurTreeItem,csOldName);
				return;
			}

			if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.isGoodID(csNewName))
			{
				int index = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getIndexByID(csOldName);
				if(-1 == index)
					return;
				wndNode *temp = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(index);
				if(temp == NULL)
					return;
				wcscpy_s(temp->id,csNewName);
				//////////重名名DesignView///////////////////
				((CTCApp*)AfxGetApp())->TCRenameTWin(csOldName,csNewName);
			}			
		}
	}

	m_hCurTreeItem=0;
	m_isNewFolder=false; 
	*pResult = 0;
}


void CProjTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CTreeView::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_wndProjView.SetFocus();
}


//void CProjTreeView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: 在此处添加消息处理程序代码
//	// 不为绘图消息调用 
//	CRect rectTree;
//	m_wndProjView.GetWindowRect(rectTree);
//	ScreenToClient(rectTree);
//	rectTree.InflateRect(1, 1);
//
//	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNFACE));
//	//CTreeView::OnPaint();
//}


BOOL CProjTreeView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)  
	{  
		m_wndProjView.EndEditLabelNow(FALSE);
		return  TRUE;  
	}
	return CTreeView::PreTranslateMessage(pMsg);
}


void CProjTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndProjView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
	    CTreeView::OnContextMenu(pWnd, point);
	    return;
	}
	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		m_hCurTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (m_hCurTreeItem != NULL)
		{
		    pWndTree->SelectItem(m_hCurTreeItem);
		    //这儿得到点击的项
		    FCTREENODETYPE itemData=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
		    CString csItemStr = m_wndProjView.GetItemText(m_hCurTreeItem);	
		    //AddhItem = m_wndProjView.GetParentItem(hTreeItem);
		    if( itemData == FCTREENODETYPE::FCTREE_PROJMANAGER )
		    {
		        if( csItemStr == _T("未加载任何项目") )
		        {
		            pWndTree->SetFocus();
		            theApp.GetContextMenuManager()->ShowPopupMenu(IDR_PROJ_LOADPROJ,point.x,point.y,this,TRUE);
		        }
		        else
		        {
		            pWndTree->SetFocus();
		            theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
		        }
		    }
		    else if( itemData == FCTREENODETYPE::FCTREE_FILE )
		    {
		        pWndTree->SetFocus();
		        theApp.GetContextMenuManager()->ShowPopupMenu(IDR_PROJ_FILE, point.x, point.y, this, TRUE);
		    }
		    else if( itemData == FCTREENODETYPE::FCTREE_FOLDER )
		    {
		        if( csItemStr == _T("资源") &&FCTREENODETYPE::FCTREE_PROJMANAGER == ((FCTREENODETYPE)m_wndProjView.GetItemData(m_wndProjView.GetParentItem(m_hCurTreeItem))))
		        {
		            pWndTree->SetFocus();
		            theApp.GetContextMenuManager()->ShowPopupMenu(IDR_PROJ_RC, point.x, point.y, this, TRUE);
		        }
		        else
		        {
		            pWndTree->SetFocus();
		            theApp.GetContextMenuManager()->ShowPopupMenu(IDR_PROJ_FOLDER,point.x,point.y,this,TRUE);
		        }
		    }
		    else if(itemData==FCTREENODETYPE::FCTREE_RCFILE || itemData==FCTREENODETYPE::FCTREE_RCWIN)
		    {
		        pWndTree->SetFocus();
		        theApp.GetContextMenuManager()->ShowPopupMenu(IDR_PROJ_RCFILE, point.x, point.y, this, TRUE);
		    }
		}
	}

	/*pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);*/
}

void CProjTreeView::OnChangeVisualStyle()
{
	m_FileViewImages.DeleteImageList();
	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndProjView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
	//m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

int CProjTreeView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CTreeView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CProjTreeView::OnTcBuild()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;	
	//这儿先激活OutPut窗口
	pFrame->OnTcBuild();
}

void CProjTreeView::OnTcCompile()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->OnTcCompile();
}

void CProjTreeView::OnTcClear()
{
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->TCClearProj();
	// TODO: 在此添加命令处理程序代码
}

void CProjTreeView::OnTcDebug()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->OnTcDebugrun();
	// pFrame->TCDebug();
}

void CProjTreeView::OnProjNewfile()
{
    // TODO: 在此添加命令处理程序代码
	if(m_hCurTreeItem)
	{
		CString csFoduleName=m_wndProjView.GetItemText(m_hCurTreeItem);
		CString csPath;
		GetItemPatchFromFileClassTree(m_hCurTreeItem,csPath);
		CDiaNewFile dlg;
		dlg.SetFilePath(csPath);
		if(IDOK==dlg.DoModal())
		{
			CString InsertName = _T("");
			InsertName.Format(_T("%s.t"),dlg.m_FileName);
			HTREEITEM funcItem=m_wndProjView.InsertItem(InsertName,1,1,m_hCurTreeItem);
			m_wndProjView.Expand(funcItem,TVE_EXPAND);
			m_wndProjView.SetItemData(funcItem,FCTREE_FILE);
			SortTreeList(m_hCurTreeItem,&m_wndProjView);
			m_wndProjView.Invalidate(TRUE);
		}
		else
			return;
	}
}

void CProjTreeView::OnProjFolderrename()
{
	// TODO: 在此添加命令处理程序代码
	if(m_hCurTreeItem)
	{
	    //m_hCurTreeItem=0;
	    m_wndProjView.EditLabel(m_hCurTreeItem);
	}
}


void CProjTreeView::OnProjDelfolder()
{
	// TODO: 在此添加命令处理程序代码
	if(m_hCurTreeItem)
	{
		CString csPath;
		//得到文件夹的路径
		GetItemPatchFromFileClassTree(m_hCurTreeItem,csPath);
		CString strFile;
		strFile.Format(_T("%s\\"),csPath);

		if(DeleteFolder(strFile))
		{

			//9-6修改Bug，老是提示无法读源或磁盘
			wchar_t wpath[MAX_PATH];
			::memset(wpath,0,sizeof(wchar_t)*MAX_PATH);
			LPCTSTR pPath=csPath;
			wcscpy(wpath,pPath);

			SHFILEOPSTRUCT op;
			op.fFlags = FOF_NOCONFIRMATION;
			op.hNameMappings = NULL;
			op.hwnd = NULL;
			op.lpszProgressTitle = NULL;
			op.pFrom = wpath;
			op.pTo = wpath;
			op.wFunc = FO_DELETE;   

			int nRet=::SHFileOperationW(&op); 
			if(nRet!=0)
				return;
			m_wndProjView.DeleteItem(m_hCurTreeItem);
			m_hCurTreeItem=NULL;
		}
	}
}

void CProjTreeView::OnProjExistfile()
{
    // TODO: 在此添加命令处理程序代码
    if(m_hCurTreeItem)
    {
        CFileDialog filedlg(TRUE, 
            _T(".t"), 
            _T("未命名.t"),
            OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
            // 注意过滤器的写法
            _T("TC Files (*.t)|*.t||"),this);

        // 他将filedlg.m_ofn.lpstrTitle 指定的文本换为标题
        filedlg.m_ofn.lpstrTitle = _T("导入源文件(.t)");

        if(IDOK==filedlg.DoModal())
        {
            CString csFilePath=filedlg.GetPathName();
            CString csFileName=filedlg.GetFileName();

            CString csInputHelp;
            csInputHelp.Format(_T("是否导入文件:%s"),csFilePath);
            int nresponse=MessageBox(csInputHelp, L"TC", MB_YESNO|MB_ICONQUESTION);
            //如果是IDYES,则把焦点放到文档最前端,从头查找第一个位置
            if( nresponse == IDNO )
            {
                return;
            }

            HTREEITEM childItem=m_wndProjView.InsertItem(csFileName,1,1,m_hCurTreeItem);
            m_wndProjView.SetItemData(childItem,FCTREENODETYPE::FCTREE_FILE);
            CString csPath;
            GetItemPatchFromFileClassTree(childItem,csPath);

            if(-1!=_waccess(csPath,0))
            {
                CString csError;
                csError.Format(_T("路径%s已经存在,导入失败!"),csPath);
                m_wndProjView.DeleteItem(childItem);
                ::AfxMessageBox(csError);
                m_hCurTreeItem=0;
                return;
            }

            if(!CopyFile(csFilePath,csPath,TRUE))
            {
                CString csError;
                csError.Format(_T("路径%s导入失败!"),csPath);
                m_wndProjView.DeleteItem(childItem);
                ::AfxMessageBox(csError);
                m_hCurTreeItem=0;
                return;
            }
            ////这儿再排一次序
            SortTreeList(m_hCurTreeItem,&m_wndProjView);
        }
    }

    m_hCurTreeItem=0;
}


void CProjTreeView::OnProjNewfolder()
{
    // TODO: 在此添加命令处理程序代码
    if(m_hCurTreeItem)
    {
        FCTREENODETYPE itemData = (FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
        if(itemData==FCTREE_FOLDER||itemData==FCTREE_PROJMANAGER)
        {
            HTREEITEM childItem=m_wndProjView.InsertItem(_T("新建文件夹"),0,0,m_hCurTreeItem);
            m_wndProjView.SetItemData(childItem,FCTREE_FOLDER);
            m_wndProjView.Expand(m_hCurTreeItem,TVE_EXPAND);
            m_hCurTreeItem=0;
            m_wndProjView.EditLabel(childItem);
            m_isNewFolder=true;
        }
    }
}

void CProjTreeView::OnProjImport()
{
    // TODO: 在此添加命令处理程序代码
    //这里是导入头的代码
	 if(m_hCurTreeItem)
    {
        FCTREENODETYPE itemData=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
        if(itemData!=FCTREENODETYPE::FCTREE_FILE)
        {
            return;
        }
        CString csFilePatch;
        GetItemRelativePatchFromFileClassTree(m_hCurTreeItem,csFilePatch);
        CString csInclude;
        csInclude.Format(_T("#include\"%s\"\n"),csFilePatch);
        CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;
		pFrame->TCInsertCode(csInclude,0);
        m_hCurTreeItem=0;
    }
}


void CProjTreeView::OnProjOpenfile()
{
    // TODO: 在此添加命令处理程序代码
    //这里是打开一个t文件的代码
    if(m_hCurTreeItem)
    {
        CString csFileName=m_wndProjView.GetItemText(m_hCurTreeItem);

        FCTREENODETYPE type=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);

        if(FCTREENODETYPE::FCTREE_FILE==type)
        {

            CString csPath;
            GetItemPatchFromFileClassTree(m_hCurTreeItem,csPath);
            ((CTCApp*)AfxGetApp())->TCOpenTFile(csPath);
            m_hCurTreeItem=0;
        }
    }
}


void CProjTreeView::OnProjDeletefile()
{
    // TODO: 在此添加命令处理程序代码
    if(m_hCurTreeItem!=0)
    {
        FCTREENODETYPE type=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
        CString csFileName=m_wndProjView.GetItemText(m_hCurTreeItem);
        if(type==FCTREENODETYPE::FCTREE_FILE)
        {
            CString csFilePatch;
            GetItemPatchFromFileClassTree(m_hCurTreeItem,csFilePatch);

            if(CTCRunTimeManager::isSameFile(csFilePatch,CTCRunTimeManager::g_projEnterFile))
            {
                ::AfxMessageBox(_T("不能删除启动文件!"));
                m_hCurTreeItem=0;
                return;
            }

            CString csDelete;
            csDelete.Format(_T("是否删除 %s "),csFilePatch);

            //确定是否删除
            if(IDNO == AfxMessageBox(csDelete,MB_YESNO | MB_ICONINFORMATION ))
            {
                return;
            }
            m_wndProjView.DeleteItem(m_hCurTreeItem);
            ::DeleteFile(csFilePatch);
            ((CTCApp*)AfxGetApp())->TCDeleteTFileView(csFilePatch);
            m_hCurTreeItem=0;
        }
    }
}


void CProjTreeView::OnProjFilerename()
{
    // TODO: 在此添加命令处理程序代码
    if(m_hCurTreeItem)
    {
        //m_hCurTreeItem=0;
        m_wndProjView.EditLabel(m_hCurTreeItem);
    }
}

void CProjTreeView::OnProjOpenrcfolder()
{
	// TODO: 在此添加命令处理程序代码
	CString csResourse;
	csResourse.Format(_T("%s\\资源"),CTCRunTimeManager::g_ProjSourcePath);
	ShellExecute(NULL, _T("open"),csResourse,NULL,NULL,SW_SHOWNORMAL);
}



void CProjTreeView::OnProjInsertrc()
{
    // TODO: 在此添加命令处理程序代码
    CFileDialog dlg(TRUE);
    if(IDOK == dlg.DoModal())
    {	
        CString sourceFilePath = dlg.GetPathName();

        int iLength = lstrlen(sourceFilePath) ;
        int iIndex = sourceFilePath.ReverseFind(L'\\') ;
        CString strFileName = sourceFilePath.Right(iLength - iIndex - 1);

        CString filePath = _T("");
        filePath.Format(_T("%s\\资源\\%s"),CTCRunTimeManager::g_ProjSourcePath,strFileName);
        if(!CopyFileW(sourceFilePath,filePath,TRUE))
        {	
            AfxMessageBox(_T("资源导入失败或该资源已经存在!"));
            return;
        }
        CString csPictEx=sourceFilePath.Right(4);
        if(0==::wcscmp(csPictEx,_T(".jpg"))||
            0==::wcscmp(csPictEx,_T(".bmp"))||
            0==::wcscmp(csPictEx,_T(".png"))||
            0==::wcscmp(csPictEx,_T(".ico")))
        {
            HTREEITEM funcItem=m_wndProjView.InsertItem(strFileName,5,5,m_hCurTreeItem);
            m_wndProjView.SetItemData(funcItem,FCTREE_RCFILE);
        }
        else
        {
            HTREEITEM funcItem=m_wndProjView.InsertItem(strFileName,6,6,m_hCurTreeItem);
            m_wndProjView.SetItemData(funcItem,FCTREE_RCFILE);
        }
    }
}

void CProjTreeView::OnProjDeletercfile()
{
	// TODO: 在此添加命令处理程序代码
	if(m_hCurTreeItem!=0)
	{
		FCTREENODETYPE type=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
		CString csFileName=m_wndProjView.GetItemText(m_hCurTreeItem);
		if(type==FCTREENODETYPE::FCTREE_RCFILE)
		{
			CString csFilePatch;
			GetItemPatchFromFileClassTree(m_hCurTreeItem,csFilePatch);

			CString csDelete;
			csDelete.Format(_T("是否删除资源文件 %s "),csFilePatch);
			//确定是否删除
			if(IDNO == AfxMessageBox(csDelete,MB_YESNO | MB_ICONINFORMATION ))
			{
				return;
			}
			m_wndProjView.DeleteItem(m_hCurTreeItem);
			::DeleteFile(csFilePatch);
			m_hCurTreeItem=0;
		}
		else if(type==FCTREENODETYPE::FCTREE_RCWIN)
		{
			//项目主面板不能删除
			if(CTCRunTimeManager::g_ProjName == csFileName)
				return;

			CString csDelete;
			csDelete.Format(_T("是否删除窗口: %s"),csFileName);

			//确定是否删除
			if(IDNO == AfxMessageBox(csDelete,MB_YESNO | MB_ICONINFORMATION ))
			{
				return;
			}

			//在全局管理类中删除当前窗口节点
			if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.removeWndInfo(csFileName))
			{
				AfxMessageBox(_T("全局管理类中删除窗口节点失败!"));
			}	
			else
			{
				((CTCApp*)AfxGetApp())->TCDeleteDesignView(csFileName);
				m_wndProjView.DeleteItem(m_hCurTreeItem);
				m_hCurTreeItem=0;
			}
		}
	}
}

void CProjTreeView::OnProjRcfilerename()
{
	// TODO: 在此添加命令处理程序代码
	if(m_hCurTreeItem)
	{
	    //m_hCurTreeItem=0;
	    m_wndProjView.EditLabel(m_hCurTreeItem);
	}
}

void CProjTreeView::OnProjRcreload()
{
	// TODO: 在此添加命令处理程序代码
	ReloadProj();
}

void CProjTreeView::OnProjNewdialog()
{
	// TODO: 在此添加命令处理程序代码
	CDiaNewTWin dlg;
	if(IDOK == dlg.DoModal())
	{
		//如果新创建了T文件则增加节点否则不用
		if(dlg.TCGetNeedNewTFile())
		{
			HTREEITEM childItem=m_wndProjView.InsertItem(dlg.TCGetNewTwinName()+_T(".t"),1,1,m_wndProjView.GetRootItem()/*m_hCurTreeItem*/);
			m_wndProjView.SetItemData(childItem,FCTREENODETYPE::FCTREE_FILE);
		}

		HTREEITEM funcItem=m_wndProjView.InsertItem(dlg.TCGetNewTwinName(),3,3,m_hCurTreeItem);
		m_wndProjView.SetItemData(funcItem,FCTREE_RCWIN);

		//添加后排一次序
		SortTreeList(m_wndProjView.GetRootItem(),&m_wndProjView);
	}
	else
		return;
}

void CProjTreeView::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	CDiaNewProj dia;
	int nResponse=dia.DoModal();
	if(nResponse == IDOK)  // OK
	{
		CTCExeVersionManager::TCResetVersionInfo();
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.reset();

		//打开项目默认文件,是否只需要这样打开就可以了? -match
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.load(CTCRunTimeManager::g_projRcFile);
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->ProjFileViewReload();
		((CTCApp*)AfxGetApp())->TCOpenTWin();

		//设置主标题
		CString csMainTitle;
		csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
		((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
		//创建备份项目
		//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
	}
}


void CProjTreeView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCOpenExistProject())
	{
		//设置主标题
		CString csMainTitle;
		csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
		((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);

		//备份项目
		//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
	}
}


void CProjTreeView::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
}


void CProjTreeView::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
}


void CProjTreeView::OnEditClear()
{
	// TODO: 在此添加命令处理程序代码
}


void CProjTreeView::OnProperties()
{
	// TODO: 在此添加命令处理程序代码
	CMDIFrameWnd   *pMain  = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;      
	CMDIChildWnd   *pChild  = (CMDIChildWnd *)pMain->GetActiveFrame(); 
	CDocument* pDoc = NULL;
	pDoc = pChild->GetActiveDocument();
	//9-8添加判断，如果不判断，都关闭后再点击项目管理项，程序会崩溃
	if( pDoc!= NULL)
	{
		CString filePath=pDoc->GetPathName();
		filePath.Trim();
		filePath.MakeLower();
		CString fileEx=filePath.Right(2);
		if(fileEx==_T(".t"))
		{
			int iLength = lstrlen(filePath) ;
			int iIndex = filePath.ReverseFind(L'\\') ;
			CString fileName = filePath.Right(iLength - iIndex - 1); 
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(fileName,filePath);
		}
		else
		{
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(_T(""),_T(""));
		}
	}
}


void CProjTreeView::OnUpdateTcBuild(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(CTCRunTimeManager::g_IsLoadProj)
	{
		if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_IsInCompile)
		{
			pCmdUI->Enable(FALSE);
		}
		else
		{
			pCmdUI->Enable();
		}
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void CProjTreeView::OnUpdateTcCompile(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(CTCRunTimeManager::g_IsLoadProj)
	{
		if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_IsInCompile)
		{
			pCmdUI->Enable(FALSE);
		}
		else
		{
			pCmdUI->Enable();
		}
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void CProjTreeView::OnUpdateTcClear(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(CTCRunTimeManager::g_IsLoadProj)
	{
		if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_IsInCompile)
		{
			pCmdUI->Enable(FALSE);
		}
		else
		{
			pCmdUI->Enable();
		}
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void CProjTreeView::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_IsInCompile)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable();
	}
}


void CProjTreeView::OnUpdateTcDebugrun(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(CTCRunTimeManager::g_IsLoadProj)
	{
		if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_IsInCompile)
		{
			pCmdUI->Enable(FALSE);
		}
		else
		{
			pCmdUI->Enable();
		}
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}



