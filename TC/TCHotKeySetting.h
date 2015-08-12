#pragma once
#include "afxcmn.h"
#include "TCHotKeyCtrl.h"

// CTCHotKeySetting 对话框

class CTCHotKeySetting : public CDialogEx
{
	DECLARE_DYNAMIC(CTCHotKeySetting)

public:
	CTCHotKeySetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCHotKeySetting();

// 对话框数据
	enum { IDD = IDD_DIA_HOTKEYSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//控件的ID
	CString m_ctrlID;
	CHotKeyCtrl m_hotKey;
	WORD m_wVirtualKeyCode;
	WORD m_wModifiers;
	CTCHotKeyCtrl* m_pHotKey; 
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedBtnhotkeyOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedHotkeyBtn();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnHotkeySys(UINT id);
	afx_msg void OnUpdateHotkeySys(CCmdUI *pCmdUI);
	afx_msg void OnHotkyeShift();
	afx_msg void OnHotkeyCtrl();
	afx_msg void OnHotkeyAlt();
	afx_msg void OnUpdateHotkyeShift(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHotkeyCtrl(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHotkeyAlt(CCmdUI *pCmdUI);
};
