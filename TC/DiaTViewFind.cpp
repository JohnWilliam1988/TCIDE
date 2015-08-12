// DiaTViewFind.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "MainFrm.h"
#include "DiaTViewFind.h"
#include "afxdialogex.h"

#include "DiaTViewFind.h"

#include "TCDoc.h"
#include "TCView.h"
//////////////////////////�༭�����ͷ/////////////////////////
#include "..\scintilla\include\scintilla.h"
#include "..\scintilla\include\scilexer.h"

#include "matchLib.h"

// CDiaTViewFind �Ի���

IMPLEMENT_DYNAMIC(CDiaTViewFind, CDialogEx)

CDiaTViewFind::CDiaTViewFind(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaTViewFind::IDD, pParent)
	, m_textToFind(_T(""))
	, m_strToReplace(_T(""))
	, m_isMatchCase(FALSE)
	, m_isWholeWord(FALSE)
{
	//m_ViewParent=NULL;
	m_pView = NULL;
	m_csText.Empty();
	m_maxFoundList = 5;
	m_maxRepalceList = 5;
	m_clickCount = 0;
}

CDiaTViewFind::~CDiaTViewFind()
{
}

void CDiaTViewFind::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_TEXTTOFIND, m_textToFind);
	DDX_CBString(pDX, IDC_TEXTTOREPLACE, m_strToReplace);
	DDX_Check(pDX, IDC_MATCHCASE, m_isMatchCase);
	DDX_Check(pDX, IDC_WHOLEWORD, m_isWholeWord);
	DDX_Control(pDX, IDC_COMBO_FIND, m_FindContentType);
	DDX_Control(pDX, IDC_TEXTTOFIND, m_foundTextCombox);
	DDX_Control(pDX, IDC_TEXTTOREPLACE, m_replaceTextCombox);
}


BEGIN_MESSAGE_MAP(CDiaTViewFind, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDiaTViewFind::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTNREPLACE, &CDiaTViewFind::OnBnClickedBtnreplace)
	ON_BN_CLICKED(IDCANCEL, &CDiaTViewFind::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTNALLREPLACE, &CDiaTViewFind::OnBnClickedBtnallreplace)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDiaTViewFind ��Ϣ�������


void CDiaTViewFind::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	UpdateData(TRUE);	
	if( m_textToFind.IsEmpty() )
		return;
	// Get the active MDI child window.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	// Get the active view attached to the active MDI child window.
	m_pView = (CView *)pChild->GetActiveView();
	int curSel = m_FindContentType.GetCurSel();
	if( 0 == curSel )
	{
		if( m_pView != NULL && m_pView->IsKindOf(RUNTIME_CLASS(CTCView)) )
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CTCView * pTView=(CTCView *)m_pView;				
			pFrame->m_findInfo.clear();
			m_csText = m_textToFind;
			//int length=pTView->TCGetLength();
			//int start=pTView->TCGetCurrentPos();
			int flag=0;
			TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
			_tcscpy(line,m_csText);

			bool hasChinese = false;
			for(int i=0;i<m_csText.GetLength();i++)
			{
				if(XMLString::isChinese(line[i]))
					hasChinese = true;
			}

			if(m_isMatchCase || hasChinese)
				flag|=SCFIND_MATCHCASE;
			if(m_isWholeWord)
				flag|=SCFIND_WHOLEWORD;

			//pTView->TCFindText(csText,flag,start,length);
			if ( pTView->FindAllContent(line,1,flag) )
			{
				if( pFrame->m_findInfo.size() == 0 )
					return;

				if( m_clickCount >= pFrame->m_findInfo.size() )
					m_clickCount = 0;

				CTCView *pView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[m_clickCount].path);
				pView->TCGotoLine(pFrame->m_findInfo[m_clickCount].lineNumber);
				pView->TCContentText(pFrame->m_findInfo[m_clickCount].strBeginPos,pFrame->m_findInfo[m_clickCount].strEndPos);

				WriteRecentFoundReplaceList();

				m_clickCount++;
			}		
			return ;
		}
	}
	else if( 1 == curSel )
	{
		if( m_pView != NULL && m_pView->IsKindOf(RUNTIME_CLASS(CTCView)) )
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CTCView * pTView=(CTCView *)m_pView;			
			pFrame->m_findInfo.clear();
			m_csText = m_textToFind;

			int flag=0;
			TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
			_tcscpy(line,m_csText);

			bool hasChinese = false;
			for(int i=0;i<m_csText.GetLength();i++)
			{
				if(XMLString::isChinese(line[i]))
					hasChinese = true;
			}

			if(m_isMatchCase || hasChinese)
				flag|=SCFIND_MATCHCASE;
			if(m_isWholeWord)
				flag|=SCFIND_WHOLEWORD;

			POSITION pos = ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetFirstDocPosition();

			CTCView *pView=NULL;
			CTCDoc* pDoc = NULL;
			while (NULL != pos)
			{
				pDoc = (CTCDoc*)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
				if(pDoc==NULL)
					continue;
				POSITION posView = pDoc->GetFirstViewPosition();
				while (NULL != posView)
				{
					pView = (CTCView *)(pDoc->GetNextView(posView));
					if (NULL == pView )
						continue;
					pView->FindAllContent(line,1,flag);
				}
			}
			
			if( pFrame->m_findInfo.size() == 0 )
				return;

			if( m_clickCount >= pFrame->m_findInfo.size() )
				m_clickCount = 0;

			CTCView *pCurView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[m_clickCount].path);
			pCurView->TCGotoLine(pFrame->m_findInfo[m_clickCount].lineNumber);
			pCurView->TCContentText(pFrame->m_findInfo[m_clickCount].strBeginPos,pFrame->m_findInfo[m_clickCount].strEndPos);

			WriteRecentFoundReplaceList();
			m_clickCount++;
			return ;

		}
		return;
	}
	else if( 2 == curSel )
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CTCView * pTView=(CTCView *)m_pView;			
		pFrame->m_findInfo.clear();
		m_csText = m_textToFind;

		int flag=0;
		TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
		_tcscpy(line,m_csText);

		bool hasChinese = false;
		for(int i=0;i<m_csText.GetLength();i++)
		{
			if(XMLString::isChinese(line[i]))
				hasChinese = true;
		}

		if(m_isMatchCase || hasChinese)
			flag|=SCFIND_MATCHCASE;
		if(m_isWholeWord)
			flag|=SCFIND_WHOLEWORD;

		vector< wstring > allTfile;
		TravelAllTFile(CTCRunTimeManager::g_ProjSourcePath + _T("\\"),allTfile);


		POSITION pos = ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetFirstDocPosition();
		CTCView *pView=NULL;
		CTCDoc* pDoc = NULL;
		while (NULL != pos)
		{
			pDoc = (CTCDoc*)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
			if(pDoc==NULL)
				continue;
			POSITION posView = pDoc->GetFirstViewPosition();
			while (NULL != posView)
			{
				pView = (CTCView *)(pDoc->GetNextView(posView));
				if (NULL == pView )
					continue;
				pView->FindAllContent(line,1,flag);

				//�����б�������T�ļ���ȥ���Ѿ��򿪵�
				for( vector< wstring >::iterator iter = allTfile.begin(); iter != allTfile.end(); )
				{
					if( iter->compare(pView->GetDocument()->GetPathName().GetString()) == 0 )
						iter = allTfile.erase(iter);
					else
						iter++ ;
				}		
			}
		}

		pTView->FindAllFileContent(allTfile,line);

		if( pFrame->m_findInfo.size() == 0 )
			return;

		if( m_clickCount >= pFrame->m_findInfo.size() )
			m_clickCount = 0;

		CTCView *pCurView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[m_clickCount].path);
		pCurView->TCGotoLine(pFrame->m_findInfo[m_clickCount].lineNumber);
		pCurView->TCContentText(pFrame->m_findInfo[m_clickCount].strBeginPos,pFrame->m_findInfo[m_clickCount].strEndPos);

		WriteRecentFoundReplaceList();
		m_clickCount++;
		return;
	}
	return ;

}


void CDiaTViewFind::OnBnClickedBtnreplace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(TRUE); 
	//// Get the active MDI child window.
	//CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	//// Get the active MDI child window.
	//CMDIChildWnd *pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	//// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	//// Get the active view attached to the active MDI child window.
	//CView *pView = (CView *) pChild->GetActiveView();
	//if(pView->IsKindOf(RUNTIME_CLASS(CTCView)))
	//{

	//	TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
	//
	//	_tcscpy(line,m_textToFind);

	//	bool hasChinese = false;
	//	for(int i=0;i<m_textToFind.GetLength();i++)
	//	{
	//		if(XMLString::isChinese(line[i]))
	//			hasChinese = true;
	//	}

	//	int flag=0;
	//	if(m_isMatchCase || hasChinese)
	//		flag|=SCFIND_MATCHCASE;
	//	if(m_isWholeWord)
	//		flag|=SCFIND_WHOLEWORD;

	//	CTCView * pTView=(CTCView *)pView;
	//	pTView->TCReplaceSelText(m_textToFind,m_strToReplace,flag);
	//	return ;
	//}
	//return ;
	UpdateData(TRUE);	
	// Get the active MDI child window.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	// Get the active view attached to the active MDI child window.
	m_pView = (CView *)pChild->GetActiveView();
	int curSel = m_FindContentType.GetCurSel();
	if( 0 == curSel )
	{
		if( m_pView != NULL && m_pView->IsKindOf(RUNTIME_CLASS(CTCView)) )
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CTCView * pTView=(CTCView *)m_pView;				
			pFrame->m_findInfo.clear();
			m_csText = m_textToFind;
			//int length=pTView->TCGetLength();
			//int start=pTView->TCGetCurrentPos();
			int flag=0;
			TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
			_tcscpy(line,m_csText);

			bool hasChinese = false;
			for(int i=0;i<m_csText.GetLength();i++)
			{
				if(XMLString::isChinese(line[i]))
					hasChinese = true;
			}

			if(m_isMatchCase || hasChinese)
				flag|=SCFIND_MATCHCASE;
			if(m_isWholeWord)
				flag|=SCFIND_WHOLEWORD;

			//pTView->TCFindText(m_csText,flag,start,length);
			pTView->TCReplaceSelText(m_textToFind,m_strToReplace,flag);	

			WriteRecentFoundReplaceList();

			return ;
		}
	}
	else if( 1 == curSel )
	{
		if( m_pView != NULL && m_pView->IsKindOf(RUNTIME_CLASS(CTCView)) )
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CTCView * pTView=(CTCView *)m_pView;			
			pFrame->m_findInfo.clear();
			m_csText = m_textToFind;

			TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
			_tcscpy(line,m_csText);

			bool hasChinese = false;
			for(int i=0;i<m_csText.GetLength();i++)
			{
				if(XMLString::isChinese(line[i]))
					hasChinese = true;
			}

			int flag=0;
			if(m_isMatchCase || hasChinese)
				flag|=SCFIND_MATCHCASE;
			if(m_isWholeWord)
				flag|=SCFIND_WHOLEWORD;

			POSITION pos = ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetFirstDocPosition();
			CTCView *pView=NULL;
			CTCDoc* pDoc = NULL;
			while (NULL != pos)
			{
				pDoc = (CTCDoc*)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
				if(pDoc==NULL)
					continue;
				POSITION posView = pDoc->GetFirstViewPosition();
				while (NULL != posView)
				{
					pView = (CTCView *)(pDoc->GetNextView(posView));
					if (NULL == pView )
						continue;
					pView->FindAllContent(line,1,flag);
				}
			}
			
			if( pFrame->m_findInfo.size() == 0 )
				return;

			if( m_clickCount >= pFrame->m_findInfo.size() )
				m_clickCount = 0;

			CTCView *pCurView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[m_clickCount].path);
			pCurView->TCGotoLine(pFrame->m_findInfo[m_clickCount].lineNumber);
			pCurView->TCContentText(pFrame->m_findInfo[m_clickCount].strBeginPos,pFrame->m_findInfo[m_clickCount].strEndPos);
			pCurView->TCReplaceSelText(m_textToFind,m_strToReplace,flag);
			WriteRecentFoundReplaceList();
			m_clickCount++;
			return ;
		}
		return;
	}
	else if( 2 == curSel )
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CTCView * pTView=(CTCView *)m_pView;			
		pFrame->m_findInfo.clear();
		m_csText = m_textToFind;

		TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
		_tcscpy(line,m_csText);

		int flag=0;
		bool hasChinese = false;
		for(int i=0;i<m_csText.GetLength();i++)
		{
			if(XMLString::isChinese(line[i]))
				hasChinese = true;
		}

		if(m_isMatchCase || hasChinese)
			flag|=SCFIND_MATCHCASE;
		if(m_isWholeWord)
			flag|=SCFIND_WHOLEWORD;

		//�������������ļ�����
		vector< wstring > allTfile;
		TravelAllTFile(CTCRunTimeManager::g_ProjSourcePath + _T("\\"),allTfile);

		POSITION pos = ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetFirstDocPosition();
		CTCView *pView=NULL;
		CTCDoc* pDoc = NULL;
		while (NULL != pos)
		{
			pDoc = (CTCDoc*)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
			if(pDoc==NULL)
				continue;
			POSITION posView = pDoc->GetFirstViewPosition();
			while (NULL != posView)
			{
				pView = (CTCView *)(pDoc->GetNextView(posView));
				if (NULL == pView )
					continue;
				pView->FindAllContent(line,1,flag);

				//�����б�������T�ļ���ȥ���Ѿ��򿪵�
				for( vector< wstring >::iterator iter = allTfile.begin(); iter != allTfile.end(); )
				{
					if( iter->compare(pView->GetDocument()->GetPathName().GetString()) == 0 )
						iter = allTfile.erase(iter);
					else
						iter++ ;
				}		
			}
		}

		if ( pTView->FindAllFileContent(allTfile,line) )
		{
			if( pFrame->m_findInfo.size() == 0 )
				return;

			if( m_clickCount >= pFrame->m_findInfo.size() )
				m_clickCount = 0;

			CTCView *pCurView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[m_clickCount].path);
			pCurView->TCGotoLine(pFrame->m_findInfo[m_clickCount].lineNumber);
			pCurView->TCContentText(pFrame->m_findInfo[m_clickCount].strBeginPos,pFrame->m_findInfo[m_clickCount].strEndPos);
			pCurView->TCReplaceSelText(m_textToFind,m_strToReplace,flag);

			WriteRecentFoundReplaceList();

			m_clickCount++;
		}	
		return;
	}
	return ;
}


void CDiaTViewFind::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->ShowWindow(SW_HIDE);
	//CDialogEx::OnCancel();
}


BOOL CDiaTViewFind::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ʼ������/�滻������Ϣ
	ReadRecentFoundReplaceList();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_FindContentType.AddString(_T("��ǰ�ĵ�"));
	m_FindContentType.AddString(_T("���д��ĵ�"));
	m_FindContentType.AddString(_T("��ǰ��Ŀ"));

	m_FindContentType.SetCurSel(0);



	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


/***********************************************************************/
/*  \�������ƣ�TravelAllTFile
/*	\�������ܣ�������Ŀ�ļ���������T�ļ������ڲ���
/*	\�������أ��� 
/*	\����˵����@sourcePath Դ·�� @&allTfile ���ص�·������
/***********************************************************************/
void CDiaTViewFind::TravelAllTFile(CString sourcePath,vector <wstring> &allTfile)
{
	CString lpPath = sourcePath;
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
				wcscpy_s(szFile,lpPath);
				wcscat_s(szFile,FindFileData.cFileName);	
				wcscat_s(szFile,_T("\\"));
				TravelAllTFile(szFile,allTfile);
			}
		}
		else
		{
			CString csFileName=FindFileData.cFileName;
			csFileName.Trim();
			CString csFileEx=csFileName.Right(2);
			if(0==::wcscmp(csFileEx,_T(".t")))
			{
				CString tfilePath = lpPath + csFileName;
				allTfile.push_back(tfilePath.GetString());			
			}
		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);
}

/***********************************************************************/
/*  \�������ƣ�WriteRecentFoundReplaceList
/*	\�������ܣ�������/�滻��Ϣд�������ļ�
/*	\�������أ��� 
/*	\����˵������
/***********************************************************************/
void CDiaTViewFind::WriteRecentFoundReplaceList()
{

	bool newFoundStr = true;
	bool newReplaceStr = true;

	for( int count = 0; count <  m_foundTextCombox.GetCount(); count++ )
	{
		CString contentStr;
		m_foundTextCombox.GetLBText(count,contentStr);
		if (contentStr == m_textToFind)
			newFoundStr = false;
	}
	if( newFoundStr && !m_textToFind.IsEmpty())
		m_foundTextCombox.AddString(m_textToFind);

	for( int count = 0; count < m_replaceTextCombox.GetCount(); count++ )
	{
		CString replaceStr;
		m_replaceTextCombox.GetLBText(count,replaceStr);
		if (replaceStr == m_strToReplace)
			newReplaceStr = false;
	}
	if( newReplaceStr && !m_strToReplace.IsEmpty())
		m_replaceTextCombox.AddString(m_strToReplace);

	int foundCount = m_foundTextCombox.GetCount() < m_maxFoundList ? m_foundTextCombox.GetCount() : m_maxFoundList;

	CString foudNumStr;
	foudNumStr.Format(_T("%d"),foundCount);
	CTCRunTimeManager::setTCProjFile(_T("Found/Replace"),_T("FoundNum"),foudNumStr);
	
	for( int count = 0; count < foundCount; count++ )
	{
		CString contentStr;
		m_foundTextCombox.GetLBText(count,contentStr);
		CString keyStr;
		keyStr.Format(_T("Found[%d]"),count);
		
		//��������Ϣ���浽��Ŀ�����ļ�����
		CTCRunTimeManager::setTCProjFile(_T("Found/Replace"),keyStr,contentStr);
	}

	int replaceCount =  m_replaceTextCombox.GetCount() < m_maxRepalceList ? m_replaceTextCombox.GetCount() : m_maxRepalceList;
	CString replaceNumStr;
	replaceNumStr.Format(_T("%d"),replaceCount);
	CTCRunTimeManager::setTCProjFile(_T("Found/Replace"),_T("ReplaceNum"),replaceNumStr);
	
	for ( int count = 0; count < replaceCount; count++)
	{
		CString replaceStr;
		m_replaceTextCombox.GetLBText(count,replaceStr);
		CString keyStr;
		keyStr.Format(_T("Replace[%d]"),count);
		//��������Ϣ���浽��Ŀ�����ļ�����
		CTCRunTimeManager::setTCProjFile(_T("Found/Replace"),keyStr,replaceStr);
	}
}

/***********************************************************************/
/*  \�������ƣ�ReadRecentFoundReplaceList
/*	\�������ܣ�������/�滻��Ϣ�������ļ�����
/*	\�������أ��� 
/*	\����˵������
/***********************************************************************/
void CDiaTViewFind::ReadRecentFoundReplaceList()
{
	m_foundTextCombox.ResetContent();
	m_replaceTextCombox.ResetContent();
	CString foundNumStr,replaceNumStr;
	::GetPrivateProfileStringW(_T("Found/Replace"),_T("FoundNum"),_T(""),foundNumStr.GetBuffer(MAX_PATH),MAX_PATH,CTCRunTimeManager::g_ProjFilePath);
	foundNumStr.ReleaseBuffer();
	::GetPrivateProfileStringW(_T("Found/Replace"),_T("ReplaceNum"),_T(""),replaceNumStr.GetBuffer(MAX_PATH),MAX_PATH,CTCRunTimeManager::g_ProjFilePath);
	replaceNumStr.ReleaseBuffer();
	int count = _ttoi(foundNumStr);
	for( int i = 0; i < count; i++ )
	{
		CString keyStr,foundStr;
		keyStr.Format(_T("Found[%d]"),i);
		::GetPrivateProfileStringW(_T("Found/Replace"),keyStr,_T(""),foundStr.GetBuffer(MAX_PATH),MAX_PATH,CTCRunTimeManager::g_ProjFilePath);
		foundStr.ReleaseBuffer();
		if( !foundStr.IsEmpty() )
			m_foundTextCombox.AddString(foundStr);
	}
	count = _ttoi(replaceNumStr);
	for( int i = 0; i < count; i++ )
	{
		CString keyStr,replaceStr;
		keyStr.Format(_T("Replace[%d]"),i);
		::GetPrivateProfileStringW(_T("Found/Replace"),keyStr,_T(""),replaceStr.GetBuffer(MAX_PATH),MAX_PATH,CTCRunTimeManager::g_ProjFilePath);
		replaceStr.ReleaseBuffer();
		if( !replaceStr.IsEmpty() )
			m_replaceTextCombox.AddString(replaceStr);
	}
}



void CDiaTViewFind::OnBnClickedBtnallreplace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	// Get the active MDI child window.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	// Get the active view attached to the active MDI child window.
	m_pView = (CView *)pChild->GetActiveView();
	int curSel = m_FindContentType.GetCurSel();
	if( 0 == curSel )
	{
		if( m_pView != NULL && m_pView->IsKindOf(RUNTIME_CLASS(CTCView)) )
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CTCView * pTView=(CTCView *)m_pView;				
			pFrame->m_findInfo.clear();
			m_csText = m_textToFind;
			//int length=pTView->TCGetLength();
			//int start=pTView->TCGetCurrentPos();
			
			int flag=0;
			TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
			_tcscpy(line,m_csText);

			bool hasChinese = false;
			for(int i=0;i<m_csText.GetLength();i++)
			{
				if(XMLString::isChinese(line[i]))
					hasChinese = true;
			}

			if(m_isMatchCase || hasChinese)
				flag|=SCFIND_MATCHCASE;
			if(m_isWholeWord)
				flag|=SCFIND_WHOLEWORD;

			//pTView->TCFindText(m_csText,flag,start,length);
			pTView->FindAllContent(line,1,flag);

			for( int count = 0; count < pFrame->m_findInfo.size(); count++ )
			{
				pTView->TCReplaceSelText(m_textToFind,m_strToReplace,flag,true);	
			}	
			WriteRecentFoundReplaceList();
			return ;
		}
	}
	else if( 1 == curSel )
	{
		if( m_pView != NULL && m_pView->IsKindOf(RUNTIME_CLASS(CTCView)) )
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CTCView * pTView=(CTCView *)m_pView;			
			pFrame->m_findInfo.clear();
			m_csText = m_textToFind;

			TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
			_tcscpy(line,m_csText);

			int flag=0;	
			bool hasChinese = false;
			for(int i=0;i<m_csText.GetLength();i++)
			{
				if(XMLString::isChinese(line[i]))
					hasChinese = true;
			}

			if(m_isMatchCase || hasChinese)
				flag |= SCFIND_MATCHCASE;
			if(m_isWholeWord)
				flag |= SCFIND_WHOLEWORD;

			POSITION pos = ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetFirstDocPosition();

			CTCView *pView=NULL;
			CTCDoc* pDoc = NULL;
			while (NULL != pos)
			{
				pDoc = (CTCDoc*)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
				if(pDoc==NULL)
					continue;
				POSITION posView = pDoc->GetFirstViewPosition();
				while (NULL != posView)
				{
					pView = (CTCView *)(pDoc->GetNextView(posView));
					if (NULL == pView )
						continue;
					pView->FindAllContent(line,1,flag);
				}
			}

			if( pFrame->m_findInfo.size() == 0 )
				return;

			for( int count = 0; count < pFrame->m_findInfo.size(); count++ )
			{
				CTCView* pCurView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[count].path);
				pCurView->TCReplaceSelText(m_textToFind,m_strToReplace,flag,true);
			}

			WriteRecentFoundReplaceList();
			
			return ;

		}
		return;
	}
	else if( 2 == curSel )
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CTCView * pTView=(CTCView *)m_pView;			
		pFrame->m_findInfo.clear();
		m_csText = m_textToFind;

		TCHAR line[1024]={0}; //���浱ǰ����µĵ��� 
		_tcscpy(line,m_csText);

		int flag=0;
		bool hasChinese = false;
		for(int i=0;i<m_csText.GetLength();i++)
		{
			if(XMLString::isChinese(line[i]))
				hasChinese = true;
		}

		if(m_isMatchCase || hasChinese)
			flag|=SCFIND_MATCHCASE;
		if(m_isWholeWord)
			flag|=SCFIND_WHOLEWORD;

		//�������������ļ��ļ���
		vector< wstring > allTfile;
		TravelAllTFile(CTCRunTimeManager::g_ProjSourcePath + _T("\\"),allTfile);

		POSITION pos = ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetFirstDocPosition();
		CTCView *pView=NULL;
		CTCDoc* pDoc = NULL;
		while (NULL != pos)
		{
			pDoc = (CTCDoc*)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
			if(pDoc==NULL)
				continue;
			POSITION posView = pDoc->GetFirstViewPosition();
			while (NULL != posView)
			{
				pView = (CTCView *)(pDoc->GetNextView(posView));
				if (NULL == pView )
					continue;
				pView->FindAllContent(line,1,flag);

				//�����б�������T�ļ���ȥ���Ѿ��򿪵�
				for( vector< wstring >::iterator iter = allTfile.begin(); iter != allTfile.end(); )
				{
					if( iter->compare(pView->GetDocument()->GetPathName().GetString()) == 0 )
						iter = allTfile.erase(iter);
					else
						iter++ ;
				}		
			}
		}

		pTView->FindAllFileContent(allTfile,line);

		if( pFrame->m_findInfo.size() == 0 )
			return;

		for( int count = 0; count < pFrame->m_findInfo.size(); count++ )
		{
			CTCView *pCurView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[count].path);
			pCurView->TCReplaceSelText(m_textToFind,m_strToReplace,flag,true);
		}
		
		WriteRecentFoundReplaceList();
		return;
	}
	return ;
}


void CDiaTViewFind::OnShowWindow(BOOL bShow, UINT nStatus)
{

	ReadRecentFoundReplaceList();

	// Get the active MDI child window.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
	// Get the active view attached to the active MDI child window.
	CView *pView = (CView *)pChild->GetActiveView();
	if(pView->IsKindOf(RUNTIME_CLASS(CTCView)))
	{
		CTCView* pTView = (CTCView*)pView;
		TCHAR selString[1024]={0};
		pTView->SendEditor(SCI_GETSELTEXT,NULL,(LPARAM)selString);
		m_textToFind = selString;
		UpdateData(FALSE);
	}

	CDialogEx::OnShowWindow(bShow, nStatus);
	// TODO: �ڴ˴������Ϣ����������
}
