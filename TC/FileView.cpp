// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "TC.h"
#include "TCDoc.h"
#include "TCView.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"
#include "TCRunTimeManager.h"
#include "DiaNewFile.h"
#include "DiaNewTWin.h"
#include "TCLoadWinRCFile.h"

#include "ProjTreeView.h"
#include "FileTreeView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
    m_isNewFolder=false;
    fFunTreePath=_T("");
    fFunTreeName=_T("");
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_PROPERTIES, OnProperties)
    //ON_COMMAND(ID_OPEN, OnFileOpen)
    //ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
    //ON_COMMAND(ID_TC_COMPILE, &CFileView::OnTcCompile)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

	m_wndSplitter.CreateStatic(this,2,1);
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CProjTreeView),CSize(0,220),NULL);
	m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CFileTreeView),CSize(220,600),NULL);

    //������ͼͼ��:
    m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));

    //m_wndProjView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
    //m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
    m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

    OnChangeVisualStyle();
	 
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

    m_wndToolBar.SetOwner(this);

    // �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//�����������������λ��
    AdjustLayout();
    m_hCurTreeItem=NULL;

    return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CFileView::AdjustLayout()
{
    if (GetSafeHwnd() == NULL)
    {
        return;
    }
	
    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
    m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	//�ָ�ڵ�λ��
	m_wndSplitter.SetWindowPos(NULL,rectClient.left, rectClient.top + cyTlb ,rectClient.Width() ,rectClient.Height() - cyTlb , SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
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

//void CFileView::OnFileOpen()
//{
//    // TODO: �ڴ˴���������������
//}
//
//void CFileView::OnFileOpenWith()
//{
//    // TODO: �ڴ˴���������������
//}

void CFileView::OnEditCut()
{
    // TODO: �ڴ˴���������������
}

void CFileView::OnEditCopy()
{
    // TODO: �ڴ˴���������������
}

void CFileView::OnEditClear()
{
    // TODO: �ڴ˴���������������
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);
    //m_wndProjView.SetFocus();
}



void CFileView::OnChangeVisualStyle()
{
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

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

    //m_wndProjView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
    //m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

//������Ŀ
void CFileView::ReloadProj()
{
	CProjTreeView* pProjTreeView = (CProjTreeView*)m_wndSplitter.GetPane(0,0);
	if( pProjTreeView != NULL )
	{
		pProjTreeView->ReloadProj();
	}
}

bool CFileView::DeleteFolder(CString lpPath)
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


bool CFileView::FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd)
{
	CProjTreeView* pProjTreeView = (CProjTreeView*)m_wndSplitter.GetPane(0,0);
	if( pProjTreeView != NULL )
	{
		return pProjTreeView->FileReNameByPropWnd(oldPatn,newName,FileOrWnd);
	}
    return false;
}



