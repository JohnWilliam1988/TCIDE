#pragma once

#include "TCTabPage.h"

const int MAXTABPAGELENGTH=32;

// CTCTab

class CTCTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CTCTab)

public:
	CTCTab();
	virtual ~CTCTab();

	CString m_strSelChange;
	bool	m_isAbled;			//记录是否有效
//功能
public:
	void TCSetSelChange(CString selChange)
	{
		m_strSelChange = selChange;
	}
	CString TCGetSelChange()
	{
		return m_strSelChange;
	}

	void TCResetAndClear();
	//显示
	void showIndex(int index);
	//tab页的个数
	int  getTabPageCount();
	//调整显示区域
	void TCAdjustRect();
	//添加一个tab页面
	void addPage(CString tabName);

	void addItem(CString tabName);
    //tab页对应的子窗口指针数组
    CDialogEx *m_Pages[MAXTABPAGELENGTH];

    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);

	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
protected:
	DECLARE_MESSAGE_MAP()
};



