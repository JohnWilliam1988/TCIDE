// ScintillaWnd.cpp: implementation of the CScintillaWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TC.h"
#include "ScintillaWnd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define STR_SCINTILLAWND _T("Scintilla")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScintillaWnd::CScintillaWnd()
{
    m_hDll = LoadLibrary(_T("SciLexer.dll"));    
	if (m_hDll==NULL)    
	{    
		MessageBox(_T("不能读取动态链接库：SciLexer.dll",_T("TC")),NULL);   
	}  
	m_strFileName = _T("");
}

BOOL CScintillaWnd::Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::CreateEx(WS_EX_CLIENTEDGE,STR_SCINTILLAWND,lpszWindowName,dwStyle,rect,pParentWnd,(UINT)nID) ;
}


CScintillaWnd::~CScintillaWnd()
{

}


BOOL CScintillaWnd::SaveFile(CString strFileName)
{
	UINT nFileOpenMode=0;
	CString FileNameForSave=strFileName;
	
	//若没有指定路径，则用原来的文件名保存文件。
	if (FileNameForSave.IsEmpty())
	{
		FileNameForSave=m_strFileName;
		nFileOpenMode=CFile::modeWrite;
	}
	else
	{
		nFileOpenMode=CFile::modeWrite|CFile::modeCreate;
	}
	
	if (!PathFileExists(FileNameForSave))
	{
		return FALSE;
	}

	if (NULL == m_hDll)
    {
        byte bytes[2];
        bytes[0] = 0xFF;
        bytes[1] = 0xFE;
        int nLength = SendMessage(SCI_GETLENGTH,0,0);
        CString csText;
		GetWindowText(csText);
        int bRet;
        HANDLE handle=0;
        DWORD dwWritenSize = 0;
        handle=CreateFile(FileNameForSave,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
        ::WriteFile(handle,bytes,2,&dwWritenSize,0);
        bRet = ::WriteFile(handle,csText,csText.GetLength()*sizeof(TCHAR),&dwWritenSize,0);
        CloseHandle(handle);
	} 
	else
	{		
        byte bytes[2];
        bytes[0] = 0xFF;
        bytes[1] = 0xFE;

        int nLength = SendMessage(SCI_GETLENGTH,0,0);
        TCHAR *szBuffer=new TCHAR[nLength + 1];
        SendMessage(SCI_GETTEXT,nLength + 1,(LPARAM)szBuffer);
        int bRet;
        HANDLE handle=0;
        DWORD dwWritenSize = 0;
        handle=CreateFile(FileNameForSave,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
        ::WriteFile(handle,bytes,2,&dwWritenSize,0);
        bRet = ::WriteFile(handle,szBuffer,nLength*sizeof(TCHAR),&dwWritenSize,0);
        delete [] szBuffer;
        CloseHandle(handle);
    }
    SendMessage(SCI_SETSAVEPOINT,0,0);
	return TRUE;
}

//DEL CString CScintillaWnd::m_strFileName()
//DEL {
//DEL 
//DEL }

BOOL CScintillaWnd::OpenFile(LPCTSTR lpFileName)
{
	if (!::PathFileExists(lpFileName))
	{
        CString csError;
        csError.Format(_T("%s 不存在"),lpFileName);
        MessageBox(csError,_T("TC"),NULL); 
		return FALSE;
	}

    HANDLE hReadFile = CreateFileW(lpFileName,      // name of the readfile
        GENERIC_READ,           // open for writing
        FILE_SHARE_READ,        // do not share
        NULL,                   // default security
        OPEN_EXISTING,          // overwrite existing
        FILE_ATTRIBUTE_NORMAL,  // normal file
        NULL);                  // no attr. template
    if (hReadFile == INVALID_HANDLE_VALUE) 
    { 
        CString csError;
        csError.Format(_T("不能打开文件 %s"),lpFileName);
        MessageBox(csError,_T("TC"),NULL); 
        return FALSE;
    }

    m_strFileName=lpFileName;

    DWORD dwFileSize = GetFileSize(hReadFile,  NULL);
    DWORD dwReadSize = dwFileSize/sizeof(TCHAR)+1;
    //吃掉uncode的标识符
    DWORD dword;
    byte helpHead[2];
    memset(helpHead,0,sizeof(byte)*2);
    ReadFile(hReadFile,helpHead,2,&dword,NULL);
	
	TCHAR *pBuffer;
	pBuffer=new TCHAR[dwReadSize];
    ::memset(pBuffer,NULL,(dwReadSize)*sizeof(TCHAR));

    ReadFile(hReadFile,pBuffer,dwFileSize,&dword,NULL);
    
    int textLength=::lstrlenW(pBuffer);
    CloseHandle(hReadFile);
    if (NULL == m_hDll)
	{
		//pBuffer[nFileLength] = L'\0';
		SetWindowText(pBuffer);
		//::AfxMessageBox(_T("OPENFILE1234"));
	}
	else
	{
		if (SendMessage(SCI_GETREADONLY,0,0))
		{	
			SendMessage(SCI_SETREADONLY,FALSE,0);
		}

		SendMessage(SCI_CANCEL,0,0);
		SendMessage(SCI_SETUNDOCOLLECTION,0,0);
		SendMessage(SCI_EMPTYUNDOBUFFER,0,0);

		//如果文本没有只读属性，则清除所有文字。
		SendMessage(SCI_CLEARALL,0,0);

		//从所有行中删除标记，若markerNumber=-1，则删除所有标记。
		SendMessage(SCI_MARKERDELETEALL,(WPARAM)-1,0);
		SendMessage(SCI_ADDTEXT,textLength,(LPARAM)pBuffer);
		//::AfxMessageBox(_T("OPENFILE"));
		SendMessage(SCI_SETUNDOCOLLECTION,1,0);
		SendMessage(EM_EMPTYUNDOBUFFER,0,0);
		SendMessage(SCI_SETSAVEPOINT,0,0);
		SendMessage(SCI_GOTOPOS,0,0);
		SendMessage(SCI_CHOOSECARETX,0,0);

//		UpdateLineNumberWidth();
	}

	delete [] pBuffer;

	return TRUE;
}

CString CScintillaWnd::GetFileName()
{
    return m_strFileName;
}