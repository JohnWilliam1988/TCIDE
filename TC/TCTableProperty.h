#pragma once
#include "afxvslistbox.h"

#include "tcCtl\TCTab.h"
#include "tcCtl\TCTabPage.h"

#include "TCTabListBox.h"
#include "TCTabProperty.h"

// CTCTableProperty �Ի���

class CTCTableProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CTCTableProperty)
	friend class CTCTabListBox;
	friend class CTCTabProperty;

public:
	CTCTableProperty(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTCTableProperty();

// �Ի�������
	enum { IDD = IDD_DIALOG_TABLE };

	////tab�ؼ���ָ��
	CTCTab *m_pTabCtrl;
	////tab��ID
	CString m_ctrlID;
	/////
	////��ǩ�ĸ���
	//int m_numOfPage;
	////������������ǩ��,�����Ժ�ɾ������
	//int m_maxPage;
	////��¼���б�ǩ���������
	//CStringArray tabName;
	////�б�����
	//CListBox m_tabList;

	//int m_cursel ;

		
	//CVSListBox m_List;
	CTCTabListBox* m_List; 
	CTCTabProperty* m_ProPerty;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnLbnSelchangeListTable();
	//afx_msg void OnBnClickedBtntabFoward();
	//afx_msg void OnBnClickedBtntabBack();
	//afx_msg void OnBnClickedBtntabAdd();
	//afx_msg void OnBnClickedBtntabDelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtntabOk();
};
