#pragma once


// CUpdateSettingPage �Ի���

class CUpdateSettingPage : public CDialogEx
{
	friend class CTCSettingDia;
	DECLARE_DYNAMIC(CUpdateSettingPage)

public:
	CUpdateSettingPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUpdateSettingPage();

// �Ի�������
	enum { IDD = IDD_UPDATESETTING_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CButton m_updateTrue;
	CButton m_updateFalse;
public:
	virtual BOOL OnInitDialog();
};
