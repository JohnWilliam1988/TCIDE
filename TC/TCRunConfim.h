#pragma once
#include "afxwin.h"


// CTCRunConfim �Ի���

class CTCRunConfim : public CDialogEx
{
	DECLARE_DYNAMIC(CTCRunConfim)

public:
	CTCRunConfim(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTCRunConfim();

// �Ի�������
	enum { IDD = IDD_DIARUNCONFIM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CButton m_popup;
public:
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
