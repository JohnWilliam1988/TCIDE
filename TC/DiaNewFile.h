#pragma once


// CDiaNewFile 对话框

class CDiaNewFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaNewFile)

public:
	CDiaNewFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaNewFile();

// 对话框数据
	enum { IDD = IDD_DIANEWFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    void SetFilePath(CString path);

    CString m_FileName;
    CString m_FilePath;
    afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
