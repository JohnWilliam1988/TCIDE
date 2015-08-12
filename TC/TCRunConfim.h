#pragma once
#include "afxwin.h"


// CTCRunConfim 对话框

class CTCRunConfim : public CDialogEx
{
	DECLARE_DYNAMIC(CTCRunConfim)

public:
	CTCRunConfim(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCRunConfim();

// 对话框数据
	enum { IDD = IDD_DIARUNCONFIM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CButton m_popup;
public:
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
