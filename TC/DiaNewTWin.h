#pragma once


// CDiaNewTWin 对话框

class CDiaNewTWin : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaNewTWin)

public:
	CDiaNewTWin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaNewTWin();

	CString TCGetNewTwinName()	{	return m_newTwinName;	}
	bool	TCGetNeedNewTFile()	{	return m_needNewTFile;	}

// 对话框数据
	enum { IDD = IDD_DIANEWTWIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
private:
	CString m_newTwinName;
	bool	m_needNewTFile;			//是否需要创建新的T文件
};
