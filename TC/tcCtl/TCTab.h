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
	bool	m_isAbled;			//��¼�Ƿ���Ч
//����
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
	//��ʾ
	void showIndex(int index);
	//tabҳ�ĸ���
	int  getTabPageCount();
	//������ʾ����
	void TCAdjustRect();
	//���һ��tabҳ��
	void addPage(CString tabName);

	void addItem(CString tabName);
    //tabҳ��Ӧ���Ӵ���ָ������
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



