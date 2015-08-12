#pragma once
#include "afxvslistbox.h"

#include "tcCtl\TCTab.h"
#include "tcCtl\TCTabPage.h"

#include "TCTabListBox.h"
#include "TCTabProperty.h"

// CTCTableProperty 对话框

class CTCTableProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CTCTableProperty)
	friend class CTCTabListBox;
	friend class CTCTabProperty;

public:
	CTCTableProperty(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCTableProperty();

// 对话框数据
	enum { IDD = IDD_DIALOG_TABLE };

	////tab控件的指针
	CTCTab *m_pTabCtrl;
	////tab的ID
	CString m_ctrlID;
	/////
	////标签的个数
	//int m_numOfPage;
	////创建过的最大标签数,方便以后删除销毁
	//int m_maxPage;
	////记录所有标签标题的数组
	//CStringArray tabName;
	////列表框对象
	//CListBox m_tabList;

	//int m_cursel ;

		
	//CVSListBox m_List;
	CTCTabListBox* m_List; 
	CTCTabProperty* m_ProPerty;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
