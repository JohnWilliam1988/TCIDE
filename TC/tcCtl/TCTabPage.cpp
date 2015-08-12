// TCTabPage.cpp : 实现文件
//

#include "stdafx.h"

#include "../mainfrm.h"
#include "../TC.h"

//TC运行时管理
#include "..\TCRunTimeManager.h"

#include "..\DiaTCDesign.h"
#include "..\TCDesignDoc.h"   
#include "..\TCDesignView.h"
#include "TCTabPage.h"
#include "TCTab.h"

#include "..\TCCtlHeads.h"


#include "afxdialogex.h"

#include "..\Docommand.h"


// CTCTabPage 对话框

IMPLEMENT_DYNAMIC(CTCTabPage, CDialogEx)

CTCTabPage::CTCTabPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCTabPage::IDD, pParent)
{
	m_TcDlgConfig.Reset();
}

CTCTabPage::~CTCTabPage()
{
}

void CTCTabPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTCTabPage, CDialogEx)
    ON_WM_CREATE()
    ON_WM_SETCURSOR()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CTCTabPage 消息处理程序
//////////////////////////////////////////////////////////////////////
//标签子窗口的创建函数
int CTCTabPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ControlTracker.Create( this );
	m_ControlTracker.SetParentWndType(TCPARENT_TABPAGE);
	//在子窗口上创建一些控件
	return 0;
}

BOOL CTCTabPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_ControlTracker.SetCursor(nHitTest, message ))
        return TRUE;

    if(nHitTest == HTCLIENT) 
    {
        CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

        TCTOOLTYPE type=pFrame->GetSelectToolType();

        if(type==TCTOOLTYPE::TCTT_STATIC)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSTATIC))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_EDIT)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CUREDIT))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_CHECK)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURCHECK))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_COMBOBOX)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURCOMBOBOX))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_RADIO)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURRADIO))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_PROGRESS)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURPROGRESS))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_GROUP)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURGROUPBOX))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_LISTBOX)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURLISTBOX))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_DATE)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURDATE))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_PICTURE)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURPICTURE))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_BUTTON)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURBUTTON))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_SLIDER)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSLIDER))))
                return true;
        }
        else if(type==TCTOOLTYPE::TCTT_PATH)
        {
            if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURPATH))))
                return true;
		}
		else if(type==TCTOOLTYPE::TCTT_HOTKEY)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURHOTKEY))))
				return true;
		}
		/*else if(type==TCTOOLTYPE::TCTT_EXPLORER)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURWEB))))
				return true;
		}*/
		else if(type==TCTOOLTYPE::TCTT_GRIDCTRL)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURGRID))))
				return true;
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CTCTabPage::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialogEx::OnPaint()
    m_ControlTracker.Draw(&dc);
}

void CTCTabPage::TCSetBkgrColor(COLORREF color)
{
	m_TcDlgConfig.m_BkgrColor=color;
	this->SetBackgroundColor(color);
}

COLORREF CTCTabPage::TCGetBkgrColor()
{
	return m_TcDlgConfig.m_BkgrColor;
}


void CTCTabPage::TCSetBackgroundImage(CString bmp,BackgroundLocation location)
{
	CString bmpPath;
	bmpPath.Format(_T("%s\\资源\\%s"),CTCRunTimeManager::g_ProjSourcePath,bmp);

	m_ImageLoader.Load(bmpPath);
	if(!m_ImageLoader.IsNull())
	{
		m_TcDlgConfig.m_BkgrBitmap=bmp;
		HBITMAP bitmap = m_ImageLoader.Detach();
		this->SetBackgroundImage(bitmap,location);
	}
	else
	{
		m_TcDlgConfig.m_BkgrBitmap=_T("");
		this->SetBackgroundImage((HBITMAP)NULL);
	}
}

CString CTCTabPage::TCGetBackgroundImage()
{
	return m_TcDlgConfig.m_BkgrBitmap;
}


BOOL CTCTabPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//获取消息类型
	UINT msgType = pMsg->message;

	CPoint point = pMsg->pt;
	ScreenToClient(&point);
	UINT nFlags = pMsg->wParam;

	//获取主框架的指针
	CMainFrame *pMainFrame = ((CMainFrame*)AfxGetApp()->m_pMainWnd);
	TCTOOLTYPE type=pMainFrame->GetSelectToolType();
	switch (msgType)
	{		
	case WM_LBUTTONDOWN:
		if(type==TCTOOLTYPE::TCTT_MAX)
        {
            CWnd * hwnd;
            POINT pointGetCtl;
            GetCursorPos(&pointGetCtl);
            hwnd= (CWnd *)WindowFromPoint(pointGetCtl);

            /////////当选中TAB中的控件时,把面板设计中的其他控件置位不选中状态///////
           
            /////////得到面板设计视图//////
			CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
			CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

			if(pCurrent != NULL)
			{
				pCurrent->m_DiaDesign.m_ControlTracker.DeSelectAll();

				for(int i=0;i<pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
				{
					if( pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd() != this->GetParent() )
					{
						if( pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
						{
							CTCTab *pTab = (CTCTab *)pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
							for(int j=0;j<pTab->getTabPageCount();j++)
							{
								((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelectAll();
							}									
						}
					}
				}
				pCurrent->m_DiaDesign.Invalidate();
			}
               
            while (hwnd && hwnd->GetParent()!=this)
                hwnd = hwnd->GetParent();
            int index=m_ControlTracker.IsIncludeControl(hwnd);
            if(-1!=index)
            {
                //这里是得到面板设计客户区域的范围
                CRect DesignRect;
                this->GetClientRect(&DesignRect);
                this->ClientToScreen(&DesignRect);

                //这里是得到选中空间的范围
                CRect CtlRect;
                POINT pointCur;

                //判断是不是选中了多个控件,并且鼠标所在的控件正好是选中的控件
                if(m_ControlTracker.m_arrRectanglesSelected.GetSize()&&(-1!=m_ControlTracker.IsInSelectControl(hwnd)))
                {
                    CtlRect=m_ControlTracker.GetBoundsRect();
                    POINT pointHelp;
                    GetCursorPos(&pointHelp);
                    this->ScreenToClient(&pointHelp);
                    m_ControlTracker.GetBoundsPointCur(pointHelp,&pointCur);
                }
                else
                {
                    hwnd->GetWindowRect(&CtlRect);
                    ScreenToClient(&CtlRect);
                    //这里是得到光标在控件上的位置
                    GetCursorPos(&pointCur);
                    hwnd->ScreenToClient(&pointCur);
                }

                //计算鼠标可以拖动的范围
                CRect ClipRect;
                ClipRect.left=DesignRect.left+pointCur.x;
                ClipRect.top=DesignRect.top+pointCur.y;
                ClipRect.right=DesignRect.right-(CtlRect.Width()-pointCur.x);
                ClipRect.bottom=DesignRect.bottom-(CtlRect.Height()-pointCur.y);

                //这里是锁光标的,拖动大小的时候,会计算覆盖窗口的大小
				bool hitSide = false;
				if (m_ControlTracker.m_arrRectanglesSelected.GetCount() > 0)
				{
					int hitPos = m_ControlTracker.m_arrRectanglesSelected[0]->HitTest(point);
					if (hitPos != CRectTracker::hitNothing && hitPos != CRectTracker::hitMiddle)
						hitSide = true;
				}

				//如果没用点中边缘点
                //设置鼠标区域
				if (hitSide == false)
                ClipCursor(&ClipRect);

                ////////////////////显示选中控件的属性//////////////////////
                int showIndex=m_ControlTracker.IsIncludeControl(hwnd);
                if(showIndex!=-1)
                {
                    TCTOOLTYPE selectType;
                    CWnd *     selectHwnd;
                    CString    selectID;
                    selectType=m_ControlTracker.m_arrRectanglesAll[showIndex]->GetCtlType();
                    selectHwnd=m_ControlTracker.m_arrRectanglesAll[showIndex]->GetCWnd();
                    selectID=m_ControlTracker.m_arrRectanglesAll[showIndex]->GetCtlID();
                    TCTOOLTYPE SelectType;
					
                    if(selectType==TCTT_STATIC)
                    {
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(selectID,(CTCStatic *)selectHwnd);
                    }
                    else if( selectType == TCTT_BUTTON )
                    {
                        CTCButton *ctrlButton = (CTCButton*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(selectID,ctrlButton);
                    }
                    else if( selectType == TCTT_GROUP )
                    {
                        CTCGroupBox *ctrlGroup = (CTCGroupBox*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(selectID,ctrlGroup);
                    }
                    else if( selectType == TCTT_EDIT )
                    {
                        CTCEdit *ctrlEdit = (CTCEdit*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(selectID,ctrlEdit);
                    }
                    else if( selectType == TCTT_RADIO )
                    {
                        CTCRadio *ctrlRadio = (CTCRadio*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(selectID,ctrlRadio);
                    }
                    else if( selectType == TCTT_CHECK )
                    {
                        CTCCheck *ctrlCheck = (CTCCheck*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(selectID,ctrlCheck);
                    }
                    else if( selectType == TCTT_COMBOBOX )
                    {
                        CTCComboBox *ctrlCombox = (CTCComboBox*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(selectID,ctrlCombox);
                    }
                    else if( selectType == TCTT_PICTURE )
                    {
                        CTCPicture *ctrlPic = (CTCPicture*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(selectID,ctrlPic);
                    }
                    else if( selectType ==TCTT_PROGRESS )
                    {
                        CTCProgress *ctrlProgress = (CTCProgress*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(selectID,ctrlProgress);
                    }
                    else if( selectType == TCTT_PATH )
                    {
                        CTCBrowse *ctrlPath = (CTCBrowse*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(selectID,ctrlPath);
                    }
                    else if( selectType == TCTT_DATE )
                    {
                        CTCDate *ctrlDate = (CTCDate*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(selectID,ctrlDate);
                    }
                    else if( selectType == TCTT_SLIDER )
                    {
                        CTCSlider *ctrlSlider = (CTCSlider*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(selectID,ctrlSlider);
                    }
                    else if( selectType == TCTT_LISTBOX )
                    {
                        CTCListBox *ctrlList = (CTCListBox*)selectHwnd;
                        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(selectID,ctrlList);
                    }
					else if( selectType == TCTT_HOTKEY )
					{
						CTCHotKeyCtrl *ctrlHotKey = (CTCHotKeyCtrl*)selectHwnd;
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(selectID,ctrlHotKey);
					}
					else if( selectType == TCTT_EXPLORER )
					{
						CTCExplorer *ctrlExplorer = (CTCExplorer*)selectHwnd;
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(selectID,ctrlExplorer);
					}
					else if( selectType == TCTT_GRIDCTRL )
					{
						CGridCtrl *ctrlGridCtrl = (CGridCtrl*)selectHwnd;
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(selectID,ctrlGridCtrl);
					}
				}
			}

			m_ControlTracker.Track( point ,pMsg->wParam,true);

			if(0 == m_ControlTracker.m_arrRectanglesSelected.GetSize())
			{
				CDiaTCDesign *pDesign = (CDiaTCDesign*)(this->GetParent()->GetParent());
				int index = pDesign->m_ControlTracker.IsIncludeControl(this->GetParent());
				pDesign->m_ControlTracker.Select(pDesign->m_ControlTracker.m_arrRectanglesAll[index]);		
				CString ctrlID = pDesign->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(ctrlID,(CTCTab*)this->GetParent());
				pDesign->Invalidate(TRUE);
			}
		}
		return TRUE;
	//鼠标左键按下消息
	case WM_LBUTTONUP:
		//提取消息中的参数,模拟OnLButtonDown函数的形参
		//正在添加控件的状态
		if (type != TCTOOLTYPE::TCTT_MAX )
		{
			CTCDesignView *pView=NULL;

			CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
			CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

			if(!pCurrent)
				return FALSE;

			pCurrent->m_DiaDesign.m_ControlTracker.DeSelectAll();

			for(int i=0;i<pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
			{
				if( pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd() != this->GetParent() )
				{
					if( pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
					{
						CTCTab *pTab = (CTCTab *)pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
						for(int j=0;j<pTab->getTabPageCount();j++)
						{
							((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelectAll();
						}									
					}
				}
			}

			pCurrent->m_DiaDesign.Invalidate();

			if(pCurrent!=NULL)
			{
				//判断控件类型
				switch (type)
				{
					////////////////////////静态文本控件/////////////////
				case TCTT_STATIC:
					{
						//创建文本控件,并添加到橡皮筋数组容器中
						AddCtrl(CRect(point.x,point.y,point.x+30,point.y+15),TCTT_STATIC,TCPARENT_TABPAGE);
						break;
					}
					////////////////////////按钮控件/////////////////
				case TCTT_BUTTON:
					{
						AddCtrl(CRect(point.x,point.y,point.x+65,point.y+28),TCTT_BUTTON,TCPARENT_TABPAGE);
						
						break;
					}
					////////////////////////编辑框控件/////////////////
				case TCTT_EDIT:
					{
						AddCtrl(CRect(point.x,point.y,point.x+60,point.y+20),TCTT_EDIT,TCPARENT_TABPAGE);
						 
						break;
					}
					////////////////////////单选按钮控件/////////////////
				case TCTT_RADIO:
					{
						AddCtrl(CRect(point.x,point.y,point.x+60,point.y+20),TCTT_RADIO,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////复选框控件///////////////
				case TCTT_CHECK:
					{
						AddCtrl(CRect(point.x,point.y,point.x+60,point.y+20),TCTT_CHECK,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////分组框///////////////////
				case TCTT_GROUP:
					{
						AddCtrl(CRect(point.x,point.y,point.x+100,point.y+100),TCTT_GROUP,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////图片框///////////////////
				case TCTT_PICTURE:
					{
						AddCtrl(CRect(point.x,point.y,point.x+100,point.y+70),TCTT_PICTURE,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////下拉框///////////////////
				case TCTT_COMBOBOX:
					{
						AddCtrl(CRect(point.x,point.y,point.x+100,point.y+20),TCTT_COMBOBOX,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////进度条控件///////////////
				case TCTT_PROGRESS:
					{
						AddCtrl(CRect(point.x,point.y,point.x+120,point.y+20),TCTT_PROGRESS,TCPARENT_TABPAGE);
						
						break;
					}
					////////////////////////浏览框控件////////////////
				case TCTT_PATH:
					{
						AddCtrl(CRect(point.x,point.y,point.x+120,point.y+25),TCTT_PATH,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////滑动条控件//////////////////
				case TCTT_SLIDER:
					{
						AddCtrl(CRect(point.x,point.y,point.x+100,point.y+22),TCTT_SLIDER,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////日期框控件///////////////////
				case TCTT_DATE:
					{
						AddCtrl(CRect(point.x,point.y,point.x+95,point.y+22),TCTT_DATE,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////列表框控件////////////////////
				case TCTT_LISTBOX:
					{
						AddCtrl(CRect(point.x,point.y,point.x+95,point.y+50),TCTT_LISTBOX,TCPARENT_TABPAGE);
						
						break;
					}
					///////////////////////热键控件////////////////////
				case TCTT_HOTKEY:
					{
						AddCtrl(CRect(point.x,point.y,point.x+60,point.y+20),TCTT_HOTKEY,TCPARENT_TABPAGE);				
						break;
					}
					///////////////////////浏览器控件///////////////////
				/*case TCTT_EXPLORER:
					{
						AddCtrl(CRect(point.x,point.y,point.x+80,point.y+40),TCTT_EXPLORER,TCPARENT_TABPAGE);
						break;
					}*/
					///////////////////////表格控件///////////////////////
				case TCTT_GRIDCTRL:
					{
						AddCtrl(CRect(point.x,point.y,point.x+200,point.y+100),TCTT_GRIDCTRL,TCPARENT_TABPAGE);

						break;
					}	
				}
			}
			//控件已经添加,把状态设置为非添加状态
			pMainFrame->ResetSelectToolType();
		}
		return TRUE;
	case WM_KEYDOWN:
		if(pMsg->wParam == 'Z' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
			Undo();
		if(pMsg->wParam == 'Y' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
			Redo();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CTCTabPage::Undo()
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
	if(pCurrent != NULL)
		pCurrent->m_DiaDesign.m_undoManager.Undo();
}

void CTCTabPage::Redo()
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
	if(pCurrent != NULL)
		pCurrent->m_DiaDesign.m_undoManager.Redo();
}

void CTCTabPage::DeSelectAll()
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent != NULL)
	{
		pCurrent->m_DiaDesign.m_ControlTracker.DeSelectAll();
		for(int i=0;i<pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd() != this->GetParent() )
			{
				if( pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
				{
					CTCTab *pTab = (CTCTab *)pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
					for(int j=0;j<pTab->getTabPageCount();j++)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelectAll();
					}									
				}
			}
		}
		pCurrent->m_DiaDesign.Invalidate();
	}	
}

void CTCTabPage::AddCtrl(CRect rc,TCTOOLTYPE type,TCPARENTTYPE parentType)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
	if(pCurrent != NULL)
	{
		AddCtrlCommand *pCommand = new AddCtrlCommand(this,rc,type,parentType);
		pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
	}	
}

void CTCTabPage::OnSizeCtrl(CWnd* pCtrl,CRect rc,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
	if(pCurrent != NULL)
	{
		OnSizeCtrlCommand *pCommand = new OnSizeCtrlCommand(this,pCtrl,rc,ctrlType,parentWndType);
		pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
	}
}

void CTCTabPage::ClearCtrl(CWnd* pCtrl,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
	if(pCurrent != NULL)
	{
		ClearCtrlCommand *pCommand = new ClearCtrlCommand(this,pCtrl,ctrlType,parentWndType);
		pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
	}
}

  
void CTCTabPage::SetTCButton(CTCButton *pButton,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pButton->SetFont(pFont);
	
	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_BUTTON);
	else
		id = ctrlid;
	pButton->TCSetID(id);
	CControlRect *rectButton =new CControlRect(rc,pButton,TCTT_BUTTON,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectButton);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectButton);
	else
		m_ControlTracker.Select(rectButton);
	if(show)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(id,pButton);
}

void CTCTabPage::SetTCStatic(CTCStatic *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{

	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_STATIC);
	else
		id = ctrlid;
	CControlRect *rectStatic =new CControlRect(rc,pCtrl,TCTT_STATIC,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectStatic);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectStatic);
	else
		m_ControlTracker.Select(rectStatic);
	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(id,pCtrl);
}

void CTCTabPage::SetTCEdit(CTCEdit *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	pCtrl->SetBackColor(RGB(255,255,255));

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_EDIT);
	else
		id = ctrlid;

	CControlRect *rectEdit =new CControlRect(rc,pCtrl,TCTT_EDIT,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectEdit);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectEdit);
	else
		m_ControlTracker.Select(rectEdit);

	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(id,pCtrl); 
}

void CTCTabPage::SetTCRadio(CTCRadio *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_RADIO);
	else
		id = ctrlid;
	CControlRect *rectRadio =new CControlRect(rc,pCtrl,TCTT_RADIO,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectRadio);

	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectRadio);
	else
		m_ControlTracker.Select(rectRadio);

	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(id,pCtrl);
}

void CTCTabPage::SetTCCheck(CTCCheck *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_CHECK);
	else
		id = ctrlid;
	CControlRect *rectCheck =new CControlRect(rc,pCtrl,TCTT_CHECK,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectCheck);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectCheck);
	else
		m_ControlTracker.Select(rectCheck);
	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(id,pCtrl);
}

void CTCTabPage::SetTCCombox(CTCComboBox *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_COMBOBOX);
	else
		id = ctrlid;
	CControlRect *rectCombox =new CControlRect(rc,pCtrl,TCTT_COMBOBOX,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectCombox);

	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectCombox);
	else
		m_ControlTracker.Select(rectCombox);

	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(id,pCtrl);
}

void CTCTabPage::SetTCProgress(CTCProgress *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_PROGRESS);
	else
		id = ctrlid;

	CControlRect *rectProgress =new CControlRect(rc,pCtrl,TCTT_PROGRESS,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectProgress);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectProgress);
	else
		m_ControlTracker.Select(rectProgress);
	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(id,pCtrl);
}

void CTCTabPage::SetTCPath(CTCBrowse *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	pCtrl->EnableFolderBrowseButton();
	//enable the browse button and put the control in the folder browse mode
	pCtrl->EnableFileBrowseButton();
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_PATH);
	else
		id = ctrlid;

	CControlRect *rectEdit =new CControlRect(rc,pCtrl,TCTT_PATH,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectEdit);

	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectEdit);
	else
		m_ControlTracker.Select(rectEdit);
	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(id,pCtrl);
}

void CTCTabPage::SetTCGroupBox(CTCGroupBox *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	pCtrl->SetCatptionTextColor(RGB(100,149,237));
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	LOGFONT lg;
	pFont->GetLogFont(&lg);
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(lg);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_GROUP);
	else
		id = ctrlid;
	CControlRect *rectGroup =new CControlRect(rc,pCtrl,TCTT_GROUP,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectGroup);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectGroup);
	else
		m_ControlTracker.Select(rectGroup);
	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(id,pCtrl);
}

void CTCTabPage::SetTCListBox(CTCListBox *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_LISTBOX);
	else
		id = ctrlid;

	CControlRect *rectList =new CControlRect(rc,pCtrl,TCTT_LISTBOX,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectList);

	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectList);
	else
		m_ControlTracker.Select(rectList);

	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(id,pCtrl);
}

void CTCTabPage::SetTCPicture(CTCPicture *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_PICTURE);
	else
		id = ctrlid;
	CControlRect *rectPicture =new CControlRect(rc,pCtrl,TCTT_PICTURE,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectPicture);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectPicture);
	else
		m_ControlTracker.Select(rectPicture);
	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(id,pCtrl);
}

void CTCTabPage::SetTCSlider(CTCSlider *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_SLIDER);
	else
		id = ctrlid;

	CControlRect *rectSlider =new CControlRect(rc,pCtrl,TCTT_SLIDER,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectSlider);

	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectSlider);
	else
		m_ControlTracker.Select(rectSlider);

	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(id,pCtrl);
}

void CTCTabPage::SetTCHotKey(CTCHotKeyCtrl *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_HOTKEY);
	else
		id = ctrlid;

	CControlRect *rectHotKey =new CControlRect(rc,pCtrl,TCTT_HOTKEY,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectHotKey);

	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectHotKey);
	else
		m_ControlTracker.Select(rectHotKey);

	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(id,pCtrl);
}

void CTCTabPage::SetTCDate(CTCDate *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);	

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_DATE);
	else
		id = ctrlid;

	CControlRect *rectDate =new CControlRect(rc,pCtrl,TCTT_DATE,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectDate);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectDate);
	else
		m_ControlTracker.Select(rectDate);
	if(show)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(id,pCtrl);
}

void CTCTabPage::SetTCExplorer(CTCExplorer *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	//pCtrl->SetBackColor(RGB(255,255,255));
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_EXPLORER);
	else
		id = ctrlid;
	CControlRect *rectExplorer =new CControlRect(rc,pCtrl,TCTT_EXPLORER,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectExplorer);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectExplorer);
	else
		m_ControlTracker.Select(rectExplorer);
	if(show)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(id,pCtrl);
}

void CTCTabPage::SetTCGridCtrl(CGridCtrl *pCtrl,CRect rc,BOOL show,CString ctrlid,bool isPaste)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(pCurrent == NULL)
		return;

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);				
	CString id;
	if(ctrlid.IsEmpty())
		id = pCurrent->m_DiaDesign.m_ControlTracker.BuildValidCtlID(TCTT_GRIDCTRL);
	else
		id = ctrlid;

	CControlRect *rectGridCtrl =new CControlRect(rc,pCtrl,TCTT_GRIDCTRL,id,TCPARENT_TABPAGE,this);
	m_ControlTracker.Add(rectGridCtrl);
	if(!isPaste)
		m_ControlTracker.SetCreateCtrlSelected(rectGridCtrl);
	else
		m_ControlTracker.Select(rectGridCtrl);
	if(show)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(id,pCtrl);
}
