#pragma once


// CDiaNewProj �Ի���

class CDiaNewProj : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaNewProj)

public:
	CDiaNewProj(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaNewProj();

// �Ի�������
	enum { IDD = IDD_DIANEWPROJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CString m_ProjName;
    CString m_ProjPath;
    int m_LangType;
    afx_msg void OnEnChangeEditprojpath();
    virtual BOOL OnInitDialog();
};
