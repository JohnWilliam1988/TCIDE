#pragma once


// CDiaNewTWin �Ի���

class CDiaNewTWin : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaNewTWin)

public:
	CDiaNewTWin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaNewTWin();

	CString TCGetNewTwinName()	{	return m_newTwinName;	}
	bool	TCGetNeedNewTFile()	{	return m_needNewTFile;	}

// �Ի�������
	enum { IDD = IDD_DIANEWTWIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
private:
	CString m_newTwinName;
	bool	m_needNewTFile;			//�Ƿ���Ҫ�����µ�T�ļ�
};
