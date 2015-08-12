#pragma once
#include "afxvslistbox.h"
#include "afxpropertygridctrl.h"
#include "TCRunTimeManager.h"
#include "GridCtrl_src/GridCtrl.h"

#include "TCGridListBox.h"
#include "TCGridProperty.h"



// CTCGridItems 对话框

struct TCGRIDINFO
{
	int colwidth;	//列宽
	CString title;	//列名
	bool readonly;	//是否只读
	TCGRIDCTRLTYPE type;	//列的控件类型
	TCGRIDINFO()
	{
		colwidth = 0;
		title = _T("");
		readonly = false;
		type = TCGRID_CTRL_MAX;
	}
};

class CTCGridItems : public CDialogEx
{
	DECLARE_DYNAMIC(CTCGridItems)
	//友元类
	 friend class CTCGridListBox;
	 friend class CTCGridProperty;
public:
	CTCGridItems(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCGridItems();
	//控件的ID
	CString m_ctrlID;
	CTCGridListBox* m_columList;
	CTCGridProperty* m_gridProperty;

	int				m_curSelectItem;		//当期选中项
// 对话框数据
	enum { IDD = IDD_DIAGRIDITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

private:
	CGridCtrl* m_pGridCtrl;
	TCGRIDINFO m_TCGridStruct[256];
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
};
