#pragma once


// CDiaNewFile �Ի���

class CDiaNewFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaNewFile)

public:
	CDiaNewFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaNewFile();

// �Ի�������
	enum { IDD = IDD_DIANEWFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    void SetFilePath(CString path);

    CString m_FileName;
    CString m_FilePath;
    afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
