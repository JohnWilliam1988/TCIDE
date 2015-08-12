#pragma once
#include "afxwin.h"


// CDiaTViewFind �Ի���

class CDiaTViewFind : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaTViewFind)

public:
	CDiaTViewFind(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaTViewFind();

// �Ի�������
	enum { IDD = IDD_TCTVIEWFIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	
public:
	CString m_textToFind;
	CString m_strToReplace;
	
	BOOL m_isMatchCase;
	BOOL m_isWholeWord;
	CComboBox m_FindContentType;
	CView* m_pView;
	CString m_csText;					//�ϴβ��ҵ�����
	int     m_clickCount;				//�������
	CComboBox m_foundTextCombox;		//���ҿ�ؼ�����
	CComboBox m_replaceTextCombox;		//�滻��ؼ�����
	unsigned int m_maxFoundList;		//�����Ҽ�¼����
	unsigned int m_maxRepalceList;		//����滻��¼����

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
