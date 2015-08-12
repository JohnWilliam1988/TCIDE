// TCTab.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../TC.h"
#include "TCTab.h"
#include "TCTabPage.h"

// CTCTab
IMPLEMENT_DYNAMIC(CTCTab, CTabCtrl)

CTCTab::CTCTab()
{
	m_isAbled = true;
}

CTCTab::~CTCTab()
{
}


BEGIN_MESSAGE_MAP(CTCTab, CTabCtrl)
		ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTCTab::OnTcnSelchange)
		ON_WM_SIZE()
END_MESSAGE_MAP()


// CTCTab ��Ϣ�������

/////////////////////////////////////////////////////////////////
//��tab�ؼ��Ĵ�С�ı�ʱ,�����Ӵ��ڵĴ�СҲ���Ÿı�
void CTCTab::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
    TCAdjustRect();
}

/////////////////////////////////////////////////////////////////
//��Ӧtab�ؼ��ı�ǩ�л���Ϣ
void CTCTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < getTabPageCount(); i++)
		m_Pages[i]->ShowWindow(SW_HIDE);
	m_Pages[GetCurSel()]->ShowWindow(SW_SHOW);

	*pResult = 0;
}

int CTCTab::getTabPageCount()
{
	return this->GetItemCount();
}

void CTCTab::addPage(CString tabName)
{
	int index=this->GetItemCount();
	this->InsertItem(index,tabName);
	CTCTabPage *page=new CTCTabPage();
	page->Create(IDD_TCTABPAGE, this);
	this->m_Pages[index] = page;

	TCAdjustRect();
}


void CTCTab::addItem(CString tabName)
{
	int index=this->GetItemCount();
	this->InsertItem(index,tabName);
}

void CTCTab::TCAdjustRect()
{
	CRect rc(0, 0, 0, 0);
	this->GetClientRect(&rc);
	this->AdjustRect(FALSE, &rc);//
	for (int i = 0; i < this->GetItemCount(); i++)
	{
		m_Pages[i]->MoveWindow(&rc);
	}
}

void CTCTab::showIndex(int index)
{
	CRect rc(0, 0, 0, 0);
	this->GetClientRect(&rc);
	this->AdjustRect(FALSE, &rc);//

	for (int i = 0; i < this->GetItemCount(); i++)
	{
		//���Bug
		if(i==index)
		{
			::MoveWindow(m_Pages[i]->m_hWnd,rc.left,rc.top,rc.right,rc.bottom,TRUE);
			m_Pages[i]->ShowWindow(SW_SHOW);
		}
		else
			m_Pages[i]->ShowWindow(SW_HIDE);
	}
}

void CTCTab::TCResetAndClear()
{
	for (int i = 0; i < this->GetItemCount(); i++)
	{
		//���Bug
		delete m_Pages[i];
	}
	this->DeleteAllItems();
}


void CTCTab::TCSetAbled(bool set)
{
	if(set)
	{
		ModifyStyle(WS_DISABLED,NULL,NULL);
		m_isAbled = true;
	}
	else
	{
		ModifyStyle(NULL,WS_DISABLED,NULL);
		m_isAbled = false;
	}
	Invalidate(TRUE);
}