#pragma once
#include "afxwin.h"

#include "TCComboBox.h"


// CTCComBoxItem 对话框

class CTCComBoxItem : public CDialogEx
{
	DECLARE_DYNAMIC(CTCComBoxItem)

public:
	CTCComBoxItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCComBoxItem();

// 对话框数据
	enum { IDD = IDD_DIACOMBOITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_editItem;
    CString m_ctrlID;
    CTCComboBox * m_pCombox;
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedBtnok();
    virtual BOOL OnInitDialog();
};
