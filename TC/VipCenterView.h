#pragma once
#include "VipCenterHtmlDia.h"

// CVipCenterView ��ͼ

class CVipCenterView : public CView
{
	DECLARE_DYNCREATE(CVipCenterView)

protected:
	CVipCenterView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CVipCenterView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CVipCenterHtmlDia m_vipCenterDia;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void AdjustLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


