#pragma once
#include "afxwin.h"

#include "TCComboBox.h"


// CTCComBoxItem �Ի���

class CTCComBoxItem : public CDialogEx
{
	DECLARE_DYNAMIC(CTCComBoxItem)

public:
	CTCComBoxItem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTCComBoxItem();

// �Ի�������
	enum { IDD = IDD_DIACOMBOITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_editItem;
    CString m_ctrlID;
    CTCComboBox * m_pCombox;
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedBtnok();
    virtual BOOL OnInitDialog();
};
