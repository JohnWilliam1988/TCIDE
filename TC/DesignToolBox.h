#pragma once

#include "FormToolBox.h"

class CDesignToolBox : public CDockablePane
{
// ����
public:
	CDesignToolBox();
    void AdjustLayout();
// ����
protected:


protected:

//�ⲿ�ӿ�
public:
    TCTOOLTYPE GetSelectToolType(); 
    void ResetSelectToolType();

private:
    CFormToolBox m_ToolBox;

// ʵ��
public:
	virtual ~CDesignToolBox();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};
