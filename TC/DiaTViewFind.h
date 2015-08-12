#pragma once
#include "afxwin.h"


// CDiaTViewFind 对话框

class CDiaTViewFind : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaTViewFind)

public:
	CDiaTViewFind(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaTViewFind();

// 对话框数据
	enum { IDD = IDD_TCTVIEWFIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public:
	CString m_textToFind;
	CString m_strToReplace;
	
	BOOL m_isMatchCase;
	BOOL m_isWholeWord;
	CComboBox m_FindContentType;
	CView* m_pView;
	CString m_csText;					//上次查找的内容
	int     m_clickCount;				//点击次数
	CComboBox m_foundTextCombox;		//查找框控件对象
	CComboBox m_replaceTextCombox;		//替换框控件对象
	unsigned int m_maxFoundList;		//最大查找记录个数
	unsigned int m_maxRepalceList;		//最大替换记录个数

	void TravelAllTFile(CString sourcePath,vector <wstring> &allTfile);
	void WriteRecentFoundReplaceList();
	void ReadRecentFoundReplaceList();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnreplace();
	afx_msg void OnBnClickedBtnallreplace();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);	
};
