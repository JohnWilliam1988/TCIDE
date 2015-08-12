#pragma once
#include "resource.h"
#include "afxwin.h"
// CEditorSettingPage �Ի���

//�༭�����������Ϣ�ṹ��
typedef struct EditorSetInfo
{
	CString fontFace;		//��������
	int fontSize;			//�����С
	bool fontBold;			//����Ӵ�
	int tabWidth;			//Tab���ַ����
	bool replaceSign;		//�滻�����ַ�

	EditorSetInfo()
	{
		fontFace = _T("");
		fontSize = 0;
		fontBold = false;
		tabWidth = 8;
		replaceSign = false;
	}
};

class CEditorSettingPage : public CDialogEx
{
	friend class CTCSettingDia;
	DECLARE_DYNAMIC(CEditorSettingPage)

public:
	CEditorSettingPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditorSettingPage();

// �Ի�������
	enum { IDD = IDD_EDITORSETTING_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	//�༭�����ý���ؼ�����
	//CMFCFontComboBox m_fontNameCtrl;	
	CComboBox m_fontCombox;
	CComboBox m_fontSizeCtrl;
	CButton m_editorBoldTrue;
	CButton m_editorBoldFalse;
	//CMFCColorButton m_eidtorBack;
	CEdit m_tabWidthCtrl;
	CSpinButtonCtrl m_tabWidthSplinCtrl;
	CButton m_editorSignTrue;
	CButton m_editorSignFalse;

	//����༭��������Ϣ�ṹ�����
	EditorSetInfo m_EditInfo;
public:
	afx_msg void OnBnClickedEditorReset();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
