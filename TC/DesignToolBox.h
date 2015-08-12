#pragma once

#include "FormToolBox.h"

class CDesignToolBox : public CDockablePane
{
// 构造
public:
	CDesignToolBox();
    void AdjustLayout();
// 特性
protected:


protected:

//外部接口
public:
    TCTOOLTYPE GetSelectToolType(); 
    void ResetSelectToolType();

private:
    CFormToolBox m_ToolBox;

// 实现
public:
	virtual ~CDesignToolBox();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};
