#pragma once
#include "afxvslistbox.h"
#include "afxpropertygridctrl.h"
#include "TCRunTimeManager.h"
#include "GridCtrl_src/GridCtrl.h"

#include "TCGridListBox.h"
#include "TCGridProperty.h"



// CTCGridItems �Ի���

struct TCGRIDINFO
{
	int colwidth;	//�п�
	CString title;	//����
	bool readonly;	//�Ƿ�ֻ��
	TCGRIDCTRLTYPE type;	//�еĿؼ�����
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
	//��Ԫ��
	 friend class CTCGridListBox;
	 friend class CTCGridProperty;
public:
	CTCGridItems(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTCGridItems();
	//�ؼ���ID
	CString m_ctrlID;
	CTCGridListBox* m_columList;
	CTCGridProperty* m_gridProperty;

	int				m_curSelectItem;		//����ѡ����
// �Ի�������
	enum { IDD = IDD_DIAGRIDITEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

private:
	CGridCtrl* m_pGridCtrl;
	TCGRIDINFO m_TCGridStruct[256];
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
};
