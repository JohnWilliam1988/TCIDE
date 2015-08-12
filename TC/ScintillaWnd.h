// ScintillaWnd.h: interface for the CScintillaWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCINTILLAWND_H__80B9E14B_7F3F_4490_A393_9E516118B3D2__INCLUDED_)
#define AFX_SCINTILLAWND_H__80B9E14B_7F3F_4490_A393_9E516118B3D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\scintilla\include\scintilla.h"
#include "..\scintilla\include\scilexer.h"

class CScintillaWnd : public CWnd  
{
public:
	CScintillaWnd();
	virtual ~CScintillaWnd();
	BOOL Create (LPCTSTR lpszWindowName, DWORD dwStyle, \
		const RECT& rect, CWnd* pParentWnd, UINT nID);
public:
	BOOL OpenFile(LPCTSTR lpFileName);
	BOOL SaveFile(CString strFileName);
    CString GetFileName();

    CString m_strFileName;
private:
    HMODULE m_hSciLexerDll;
public:
    HMODULE m_hDll;
};

#endif // !defined(AFX_SCINTILLAWND_H__80B9E14B_7F3F_4490_A393_9E516118B3D2__INCLUDED_)
