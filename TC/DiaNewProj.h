#pragma once


// CDiaNewProj 对话框

class CDiaNewProj : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaNewProj)

public:
	CDiaNewProj(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaNewProj();

// 对话框数据
	enum { IDD = IDD_DIANEWPROJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CString m_ProjName;
    CString m_ProjPath;
    int m_LangType;
    afx_msg void OnEnChangeEditprojpath();
    virtual BOOL OnInitDialog();
};
