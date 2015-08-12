// ProjTreeView.cpp : ʵ���ļ�
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
	///////////////////////////�ļ��нڵ㹦�ܺ���/////////////////////
	ON_COMMAND(ID_PROJ_FOLDERRENAME, &CProjTreeView::OnProjFolderrename)
	ON_COMMAND(ID_PROJ_DELFOLDER, &CProjTreeView::OnProjDelfolder)
	///////////////////////////��Ŀ����ڵ㹦�ܺ���///////////////////
	ON_COMMAND(ID_TC_BUILD, &CProjTreeView::OnTcBuild)
	ON_COMMAND(ID_TC_COMPILE, &CProjTreeView::OnTcCompile)
	ON_COMMAND(ID_TC_CLEAR, &CProjTreeView::OnTcClear)
	ON_COMMAND(ID_TC_DEBUGRUN, &CProjTreeView::OnTcDebug)
	ON_COMMAND(ID_PROJ_NEWFILE, &CProjTreeView::OnProjNewfile)
	ON_COMMAND(ID_PROJ_EXISTFILE, &CProjTreeView::OnProjExistfile)
	ON_COMMAND(ID_PROJ_NEWFOLDER, &CProjTreeView::OnProjNewfolder)
	///////////////////////////�ļ��ڵ�˵����ܺ���///////////////////
	ON_COMMAND(ID_PROJ_IMPORT, &CProjTreeView::OnProjImport)
	ON_COMMAND(ID_PROJ_OPENFILE, &CProjTreeView::OnProjOpenfile)
	ON_COMMAND(ID_PROJ_DELETEFILE, &CProjTreeView::OnProjDeletefile)
	ON_COMMAND(ID_PROJ_FILERENAME, &CProjTreeView::OnProjFilerename)
	///////////////////////////��Դ�ڵ�˵�����///////////////////////
	ON_COMMAND(ID_PROJ_INSERTRC, &CProjTreeView::OnProjInsertrc)
	ON_COMMAND(ID_PROJ_OPENRCFOLDER, &CProjTreeView::OnProjOpenrcfolder)
	ON_COMMAND(ID_PROJ_RCRELOAD, &CProjTreeView::OnProjRcreload)
	ON_COMMAND(ID_PROJ_NEWDIALOG, &CProjTreeView::OnProjNewdialog)
	ON_COMMAND(ID_FILE_NEW, &CProjTreeView::OnFileNew)
	//////////////////////////��Դ�ļ��ڵ�˵�����////////////////////
	ON_COMMAND(ID_PROJ_RCFILERENAME, &CProjTreeView::OnProjRcfilerename)
	ON_COMMAND(ID_PROJ_DELETERCFILE, &CProjTreeView::OnProjDeletercfile)
	ON_COMMAND(ID_FILE_OPEN, &CProjTreeView::OnFileOpen)
	ON_COMMAND(ID_EDIT_CUT, &CProjTreeView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CProjTreeView::OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, &CProjTreeView::OnEditClear)
	ON_COMMAND(ID_PROPERTIES, &CProjTreeView::OnProperties)
	//////////////////////////����ʱ������ز˵�����////////////////////////
	ON_UPDATE_COMMAND_UI(ID_TC_BUILD, &CProjTreeView::OnUpdateTcBuild)
	ON_UPDATE_COMMAND_UI(ID_TC_COMPILE, &CProjTreeView::OnUpdateTcCompile)
	ON_UPDATE_COMMAND_UI(ID_TC_CLEAR, &CProjTreeView::OnUpdateTcClear)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CProjTreeView::OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_TC_DEBUGRUN, &CProjTreeView::OnUpdateTcDebugrun)
END_MESSAGE_MAP()


// CProjTreeView ���

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


// CProjTreeView ��Ϣ�������


BOOL CProjTreeView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ˴������Ϣ����������
	AdjustLayout();
}


int CProjTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	// ������ͼ:
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS| TVS_EDITLABELS |TVS_EX_AUTOHSCROLL;

	if (!m_wndProjView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	//������ͼͼ��:
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
	//��Ŀ��
	HTREEITEM hRoot = m_wndProjView.InsertItem(_T("δ�����κ���Ŀ"), 4, 4);
	m_wndProjView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndProjView.SetItemData(hRoot,FCTREE_PROJMANAGER);
	//�ļ���
	//HTREEITEM hRoot1 = m_wndFileView.InsertItem(_T("δѡ���κ�Դ�ļ�"), 0, 0);
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

//������Ŀ
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
		csProjName.Format(_T("��Ŀ����-%s"),CTCRunTimeManager::g_ProjName);
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
		HTREEITEM root=m_wndProjView.InsertItem(_T("δ�����κ���Ŀ"),4,4,TVI_ROOT);
		m_wndProjView.SetItemData(root,FCTREE_PROJMANAGER);
	}
}

//��������ǵõ�һ�����οؼ��ڵ���������ļ���·��,�����·��.
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

//��������ǵõ�һ�����οؼ��ڵ��������ļ���·��,�Ǿ���·��.
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
				//9-6�޸�Bug��������ʾ�޷���Դ�����
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
				if(CTCRunTimeManager::isSameFile(FindFileData.cFileName,_T("��Դ")))
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


//������ԴĿ¼,���ص����οؼ�
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
				//////////////////////////////////��������������ӽ���ڵ�/////////////////////////////////
				int count = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getCount();
				for(int i = 0;i < count;i++)
				{
					wndNode *temp = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(i);		
					HTREEITEM funcItem=m_wndProjView.InsertItem(temp->id,3,3,item);
					m_wndProjView.SetItemData(funcItem,FCTREE_RCWIN);
				}
				//////////////////////////////////��������������ӽ���ڵ�/////////////////////////////////
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
			CString ItemPath=_T("");// �ڵ��·��
			while( ItemChild != NULL )
			{
				//�õ��ӽڵ��·��
				GetItemPatchFromFileClassTree(ItemChild,ItemPath);
				CString csFileFloder = _T("");
				GetItemPatchFromFileClassTree(m_wndProjView.GetParentItem(ItemChild),csFileFloder);
				if( 0==ItemPath.CompareNoCase(oldPatn))
				{
					//�µ����������ļ�·��
					CString csFileNewPath = _T("");
					csFileNewPath.Format(_T("%s\\%s"),csFileFloder,newName);
					//����������
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
						csError.Format(_T("������ %s ʧ��!"),ItemPath);
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

				//����ݹ��ȥ
				TravelFileTree(ItemChild,oldPatn,newName,TRUE);
				ItemChild = m_wndProjView.GetNextItem(ItemChild,TVGN_NEXT);
			}
		}
		else
		{
			HTREEITEM ItemChild = m_wndProjView.GetChildItem(hItem);
			while( ItemChild != NULL )
			{	
				if(m_wndProjView.GetItemText(ItemChild) == _T("��Դ") && m_wndProjView.GetItemData(ItemChild) == FCTREE_FOLDER)
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

//�����Ŀ������
void CProjTreeView::OnClickedProjView(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_wndProjView.ScreenToClient(&pt);

	//��֪��Ϊʲô����ҪunFlags,Ҫ��Ȼ�õ�һֱ��0
	UINT unFlags = 0 ;
	m_hCurTreeItem = m_wndProjView.HitTest(pt, &unFlags) ;

	if(m_hCurTreeItem!=NULL)
	{
		FCTREENODETYPE itemData=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);

		//�õ���ǰ�ļ���·��
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
			//		//�����ǰ��ͼ��������ʾ��������,�������򿪲���
			//		if(csName == pView->m_TWinID)
			//		{			
			//			CMDIChildWnd *pChild=(CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
			//			CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
			//			//�����ǰ��ͼ���ǻ��ͼ���򼤻�
			//			if(pChild->GetActiveView()->m_hWnd != pView->m_hWnd)
			//				pFWnd->MDIActivate();
			//			//������´�������
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
			csCaption.Format(_T("�޷���ÿռ亯��-%s"),fileName);
			HTREEITEM root = pFileView->m_wndFileView.InsertItem(csCaption,1,1,TVI_ROOT);
			return;
		}

		csCaption.Format(_T("�ռ亯���б�-%s"),fileName);
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
		//�����������???
		((CTCApp*)AfxGetApp())->TCOpenTFile(csPath);
		//9-13�����ͬ������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_wndProjView.GetEditControl()->LimitText(MAX_PATH);

	m_hCurTreeItem=pTVDispInfo->item.hItem;

	*pResult = 0;
}

void CProjTreeView::OnTvnEndlabeleditProjView(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

		//�������������Ϊ ��Ŀ���� �ļ���,ֱ���˳�
		FCTREENODETYPE itemData=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
		if(itemData==FCTREENODETYPE::FCTREE_PROJMANAGER)
		{           
			m_hCurTreeItem=0;
			m_isNewFolder=false;
			return;
		}

		//������������� ��Դ �ļ���,ֱ���˳�
		if(itemData==FCTREENODETYPE::FCTREE_FOLDER&&csOldName==_T("��Դ")&&(FCTREENODETYPE::FCTREE_PROJMANAGER==(FCTREENODETYPE)m_wndProjView.GetItemData(m_wndProjView.GetParentItem(m_hCurTreeItem))))
		{
			m_hCurTreeItem=0;
			m_isNewFolder=false;
			return;
		}

		//�жϲ����޸������ļ�
		//�õ���ǰ��·����
		CString csOldPath;
		GetItemPatchFromFileClassTree(m_hCurTreeItem,csOldPath);

		if(CTCRunTimeManager::isSameFile(csOldPath,CTCRunTimeManager::g_projEnterFile))
		{
			//::AfxMessageBox(_T("���������������ļ�!"));
			m_isNewFolder=false;
			return;
		}

		if(pTVDispInfo->item.pszText)
		{
			m_wndProjView.SetItemText(m_hCurTreeItem,pTVDispInfo->item.pszText);
		}

		//�õ���ǰ��·����
		CString csFileName;    
		GetItemPatchFromFileClassTree(m_hCurTreeItem,csFileName);

		//�����Ǵ����½��ļ��е����,��Ϊ���ʱ��"�½��ļ���"�ǲ����ڵ�
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
					csError.Format(_T("·�� %s �Ѿ�����,����ʧ��!"),csFileName);
					::AfxMessageBox(csError);
					return;
				}

				//�ж��ļ��д����Ƿ�ɹ�
				if(0==CreateDirectory(csFileName,NULL))   
				{
					m_wndProjView.DeleteItem(m_hCurTreeItem);
					AfxMessageBox(_T("������Ŀ�ļ���ʧ��,������!")); 
				}
				m_hCurTreeItem=0;
				m_isNewFolder=false;
				return;
			}

			AfxMessageBox(_T("ִ�е������ʾ������!")); 
			m_hCurTreeItem=0;
			m_isNewFolder=false;
			return;
		}

		//Ϊ�ղ����κβ���,ֱ�ӷ���
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

			//�����ж��½����ļ����ļ��в���,�ǲ����Ѿ�����,���ڵĻ���ֱ���˳�
			if(::PathFileExists(csFileName))
			{
				CString csError;
				m_wndProjView.SetItemText(m_hCurTreeItem,csOldName);
				csError.Format(_T("%s �Ѿ�����,�޸�ʧ��!"),csFileName);
				::AfxMessageBox(csError);
				m_hCurTreeItem=0;
				m_isNewFolder=false;
				return;
			}

			//����������
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
				csError.Format(_T("������ %s ʧ��!"),csOldName);
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
				//��������Կ�ͬ��
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(csNewName,newPath);
			}
		}
		else if(itemData==FCTREENODETYPE::FCTREE_RCWIN)
		{
			//����Ҫ���� twin �ļ�������������
			//���������������Ŀ�������򷵻�
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
				//////////������DesignView///////////////////
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

	// TODO: �ڴ˴������Ϣ����������
	m_wndProjView.SetFocus();
}


//void CProjTreeView::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: �ڴ˴������Ϣ����������
//	// ��Ϊ��ͼ��Ϣ���� 
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
	// TODO: �ڴ����ר�ô����/����û���
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
		// ѡ���ѵ�������:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		m_hCurTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (m_hCurTreeItem != NULL)
		{
		    pWndTree->SelectItem(m_hCurTreeItem);
		    //����õ��������
		    FCTREENODETYPE itemData=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
		    CString csItemStr = m_wndProjView.GetItemText(m_hCurTreeItem);	
		    //AddhItem = m_wndProjView.GetParentItem(hTreeItem);
		    if( itemData == FCTREENODETYPE::FCTREE_PROJMANAGER )
		    {
		        if( csItemStr == _T("δ�����κ���Ŀ") )
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
		        if( csItemStr == _T("��Դ") &&FCTREENODETYPE::FCTREE_PROJMANAGER == ((FCTREENODETYPE)m_wndProjView.GetItemData(m_wndProjView.GetParentItem(m_hCurTreeItem))))
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
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CTreeView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CProjTreeView::OnTcBuild()
{
	// TODO: �ڴ���������������
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;	
	//����ȼ���OutPut����
	pFrame->OnTcBuild();
}

void CProjTreeView::OnTcCompile()
{
	// TODO: �ڴ���������������
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->OnTcCompile();
}

void CProjTreeView::OnTcClear()
{
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->TCClearProj();
	// TODO: �ڴ���������������
}

void CProjTreeView::OnTcDebug()
{
	// TODO: �ڴ���������������
	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->OnTcDebugrun();
	// pFrame->TCDebug();
}

void CProjTreeView::OnProjNewfile()
{
    // TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	if(m_hCurTreeItem)
	{
	    //m_hCurTreeItem=0;
	    m_wndProjView.EditLabel(m_hCurTreeItem);
	}
}


void CProjTreeView::OnProjDelfolder()
{
	// TODO: �ڴ���������������
	if(m_hCurTreeItem)
	{
		CString csPath;
		//�õ��ļ��е�·��
		GetItemPatchFromFileClassTree(m_hCurTreeItem,csPath);
		CString strFile;
		strFile.Format(_T("%s\\"),csPath);

		if(DeleteFolder(strFile))
		{

			//9-6�޸�Bug��������ʾ�޷���Դ�����
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
    // TODO: �ڴ���������������
    if(m_hCurTreeItem)
    {
        CFileDialog filedlg(TRUE, 
            _T(".t"), 
            _T("δ����.t"),
            OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
            // ע���������д��
            _T("TC Files (*.t)|*.t||"),this);

        // ����filedlg.m_ofn.lpstrTitle ָ�����ı���Ϊ����
        filedlg.m_ofn.lpstrTitle = _T("����Դ�ļ�(.t)");

        if(IDOK==filedlg.DoModal())
        {
            CString csFilePath=filedlg.GetPathName();
            CString csFileName=filedlg.GetFileName();

            CString csInputHelp;
            csInputHelp.Format(_T("�Ƿ����ļ�:%s"),csFilePath);
            int nresponse=MessageBox(csInputHelp, L"TC", MB_YESNO|MB_ICONQUESTION);
            //�����IDYES,��ѽ���ŵ��ĵ���ǰ��,��ͷ���ҵ�һ��λ��
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
                csError.Format(_T("·��%s�Ѿ�����,����ʧ��!"),csPath);
                m_wndProjView.DeleteItem(childItem);
                ::AfxMessageBox(csError);
                m_hCurTreeItem=0;
                return;
            }

            if(!CopyFile(csFilePath,csPath,TRUE))
            {
                CString csError;
                csError.Format(_T("·��%s����ʧ��!"),csPath);
                m_wndProjView.DeleteItem(childItem);
                ::AfxMessageBox(csError);
                m_hCurTreeItem=0;
                return;
            }
            ////�������һ����
            SortTreeList(m_hCurTreeItem,&m_wndProjView);
        }
    }

    m_hCurTreeItem=0;
}


void CProjTreeView::OnProjNewfolder()
{
    // TODO: �ڴ���������������
    if(m_hCurTreeItem)
    {
        FCTREENODETYPE itemData = (FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
        if(itemData==FCTREE_FOLDER||itemData==FCTREE_PROJMANAGER)
        {
            HTREEITEM childItem=m_wndProjView.InsertItem(_T("�½��ļ���"),0,0,m_hCurTreeItem);
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
    // TODO: �ڴ���������������
    //�����ǵ���ͷ�Ĵ���
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
    // TODO: �ڴ���������������
    //�����Ǵ�һ��t�ļ��Ĵ���
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
    // TODO: �ڴ���������������
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
                ::AfxMessageBox(_T("����ɾ�������ļ�!"));
                m_hCurTreeItem=0;
                return;
            }

            CString csDelete;
            csDelete.Format(_T("�Ƿ�ɾ�� %s "),csFilePatch);

            //ȷ���Ƿ�ɾ��
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
    // TODO: �ڴ���������������
    if(m_hCurTreeItem)
    {
        //m_hCurTreeItem=0;
        m_wndProjView.EditLabel(m_hCurTreeItem);
    }
}

void CProjTreeView::OnProjOpenrcfolder()
{
	// TODO: �ڴ���������������
	CString csResourse;
	csResourse.Format(_T("%s\\��Դ"),CTCRunTimeManager::g_ProjSourcePath);
	ShellExecute(NULL, _T("open"),csResourse,NULL,NULL,SW_SHOWNORMAL);
}



void CProjTreeView::OnProjInsertrc()
{
    // TODO: �ڴ���������������
    CFileDialog dlg(TRUE);
    if(IDOK == dlg.DoModal())
    {	
        CString sourceFilePath = dlg.GetPathName();

        int iLength = lstrlen(sourceFilePath) ;
        int iIndex = sourceFilePath.ReverseFind(L'\\') ;
        CString strFileName = sourceFilePath.Right(iLength - iIndex - 1);

        CString filePath = _T("");
        filePath.Format(_T("%s\\��Դ\\%s"),CTCRunTimeManager::g_ProjSourcePath,strFileName);
        if(!CopyFileW(sourceFilePath,filePath,TRUE))
        {	
            AfxMessageBox(_T("��Դ����ʧ�ܻ����Դ�Ѿ�����!"));
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
	// TODO: �ڴ���������������
	if(m_hCurTreeItem!=0)
	{
		FCTREENODETYPE type=(FCTREENODETYPE)m_wndProjView.GetItemData(m_hCurTreeItem);
		CString csFileName=m_wndProjView.GetItemText(m_hCurTreeItem);
		if(type==FCTREENODETYPE::FCTREE_RCFILE)
		{
			CString csFilePatch;
			GetItemPatchFromFileClassTree(m_hCurTreeItem,csFilePatch);

			CString csDelete;
			csDelete.Format(_T("�Ƿ�ɾ����Դ�ļ� %s "),csFilePatch);
			//ȷ���Ƿ�ɾ��
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
			//��Ŀ����岻��ɾ��
			if(CTCRunTimeManager::g_ProjName == csFileName)
				return;

			CString csDelete;
			csDelete.Format(_T("�Ƿ�ɾ������: %s"),csFileName);

			//ȷ���Ƿ�ɾ��
			if(IDNO == AfxMessageBox(csDelete,MB_YESNO | MB_ICONINFORMATION ))
			{
				return;
			}

			//��ȫ�ֹ�������ɾ����ǰ���ڽڵ�
			if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.removeWndInfo(csFileName))
			{
				AfxMessageBox(_T("ȫ�ֹ�������ɾ�����ڽڵ�ʧ��!"));
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
	// TODO: �ڴ���������������
	if(m_hCurTreeItem)
	{
	    //m_hCurTreeItem=0;
	    m_wndProjView.EditLabel(m_hCurTreeItem);
	}
}

void CProjTreeView::OnProjRcreload()
{
	// TODO: �ڴ���������������
	ReloadProj();
}

void CProjTreeView::OnProjNewdialog()
{
	// TODO: �ڴ���������������
	CDiaNewTWin dlg;
	if(IDOK == dlg.DoModal())
	{
		//����´�����T�ļ������ӽڵ������
		if(dlg.TCGetNeedNewTFile())
		{
			HTREEITEM childItem=m_wndProjView.InsertItem(dlg.TCGetNewTwinName()+_T(".t"),1,1,m_wndProjView.GetRootItem()/*m_hCurTreeItem*/);
			m_wndProjView.SetItemData(childItem,FCTREENODETYPE::FCTREE_FILE);
		}

		HTREEITEM funcItem=m_wndProjView.InsertItem(dlg.TCGetNewTwinName(),3,3,m_hCurTreeItem);
		m_wndProjView.SetItemData(funcItem,FCTREE_RCWIN);

		//��Ӻ���һ����
		SortTreeList(m_wndProjView.GetRootItem(),&m_wndProjView);
	}
	else
		return;
}

void CProjTreeView::OnFileNew()
{
	// TODO: �ڴ���������������
	CDiaNewProj dia;
	int nResponse=dia.DoModal();
	if(nResponse == IDOK)  // OK
	{
		CTCExeVersionManager::TCResetVersionInfo();
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.reset();

		//����ĿĬ���ļ�,�Ƿ�ֻ��Ҫ�����򿪾Ϳ�����? -match
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.load(CTCRunTimeManager::g_projRcFile);
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->ProjFileViewReload();
		((CTCApp*)AfxGetApp())->TCOpenTWin();

		//����������
		CString csMainTitle;
		csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
		((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
		//����������Ŀ
		//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
	}
}


void CProjTreeView::OnFileOpen()
{
	// TODO: �ڴ���������������
	if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCOpenExistProject())
	{
		//����������
		CString csMainTitle;
		csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
		((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);

		//������Ŀ
		//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
	}
}


void CProjTreeView::OnEditCut()
{
	// TODO: �ڴ���������������
}


void CProjTreeView::OnEditCopy()
{
	// TODO: �ڴ���������������
}


void CProjTreeView::OnEditClear()
{
	// TODO: �ڴ���������������
}


void CProjTreeView::OnProperties()
{
	// TODO: �ڴ���������������
	CMDIFrameWnd   *pMain  = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;      
	CMDIChildWnd   *pChild  = (CMDIChildWnd *)pMain->GetActiveFrame(); 
	CDocument* pDoc = NULL;
	pDoc = pChild->GetActiveDocument();
	//9-8����жϣ�������жϣ����رպ��ٵ����Ŀ�������������
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
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ������������û����洦��������
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
	// TODO: �ڴ������������û����洦��������
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



