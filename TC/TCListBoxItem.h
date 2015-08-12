#pragma once
#include "afxwin.h"
#include "tcCtl\TCListBox.h"


// CTCListBoxItem 对话框

class CTCListBoxItem : public CDialogEx
{
	DECLARE_DYNAMIC(CTCListBoxItem)

public:
	CTCListBoxItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCListBoxItem();

// 对话框数据
	enum { IDD = IDD_DIALISTITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    CEdit m_editItem;
    CString m_ctrlID;
    CTCListBox * m_pList;
    afx_msg void OnBnClickedBtnok();
    virtual BOOL OnInitDialog();
};
