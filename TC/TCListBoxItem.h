#pragma once
#include "afxwin.h"
#include "tcCtl\TCListBox.h"


// CTCListBoxItem �Ի���

class CTCListBoxItem : public CDialogEx
{
	DECLARE_DYNAMIC(CTCListBoxItem)

public:
	CTCListBoxItem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTCListBoxItem();

// �Ի�������
	enum { IDD = IDD_DIALISTITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    CEdit m_editItem;
    CString m_ctrlID;
    CTCListBox * m_pList;
    afx_msg void OnBnClickedBtnok();
    virtual BOOL OnInitDialog();
};
