#include "stdafx.h"
#include "ControlTracker.h"
#include "ControlRect.h"
#include "MainFrm.h"
#include "TCRunTimeManager.h"
#include "TCTabPage.h"
#include "Docommand.h"

CControlTracker::CControlTracker() : m_pwndParent( NULL )
{
    m_BasicElement = NULL;
}

CControlTracker::~CControlTracker()
{

}

void CControlTracker::Create(CWnd* pWnd)
{
	//这个断言会在Tab创建Redo的时候报错
    //ASSERT( m_pwndParent == NULL );
    m_pwndParent = pWnd;
}
void CControlTracker::Add( CControlRect* pObject )
{
	//插入控件到橡皮组中
	Insert(pObject);
	//新控件置顶
	//CorrectZOrders(pObject->GetCWnd());
	TCCorrectZOrders();
}

void CControlTracker::JustAdd( CControlRect* pObject )
{
	m_arrRectanglesAll.Add(pObject);
}

void CControlTracker::Insert(CControlRect* pObject)
{
	//无控件时,不需要规则
	if (m_arrRectanglesAll.GetCount() == 0)
	{
		m_arrRectanglesAll.SetAtGrow(0, pObject);
		return;
	}

	for (int i = m_arrRectanglesAll.GetCount(); i > 0; i--)
	m_arrRectanglesAll.SetAtGrow(i, m_arrRectanglesAll[i - 1]);
	//插入新加的控件
	m_arrRectanglesAll.SetAtGrow(0, pObject);
}

/////////////////////////////////////////////////////////////////////////////////////////
//把新添加的控件置顶
void CControlTracker::CorrectZOrders( CWnd* pWndInsertAfter )
{	
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )	
    {
        m_arrRectanglesAll.GetAt( i )->GetCWnd()->SetWindowPos( pWndInsertAfter , 0,0,0,0, SWP_NOMOVE   | SWP_NOSIZE   | SWP_SHOWWINDOW   );
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//TC自定义Z序排列
void CControlTracker::TCCorrectZOrders()
{	
	//控件个数
	int count = m_arrRectanglesAll.GetCount();
	//把其他控件,按向前叠加的方式,插入到最后一个控件z序的后面
	for (int i = count - 2; i >= 0; i--)
		m_arrRectanglesAll[i]->GetCWnd()->SetWindowPos(m_arrRectanglesAll[count - 1]->GetCWnd(),
			NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);
	//把最后一个控件放到z序最后.至此,所有控件通过向前叠加的方式,z序已排好
	if (count > 1)
		m_arrRectanglesAll[count - 1]->GetCWnd()->SetWindowPos(m_arrRectanglesAll[count - 2]->GetCWnd(),
			NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);
}

void CControlTracker::TCSetCurrentCtrlZorder(int pos,CWnd* pCtrl)
{

	//原来当前控件的位置
	int oldPos = 0,newPos = 0;
	CControlRect* pTemp = NULL;
	for(int i=0;i<m_arrRectanglesAll.GetSize();i++)
	{
		if(m_arrRectanglesAll[i]->GetCtlType() == TCTT_TAB)
		{
			if(m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==pCtrl->m_hWnd)
			{  
				oldPos = i;
				newPos = m_arrRectanglesAll.GetSize() - pos;
				pTemp = m_arrRectanglesAll[i];
				//以前的位置
				//先移动要重新排列的控件
				if(oldPos < newPos)
				{
					for(int loop=0;loop<newPos-oldPos;loop++)
					{
						m_arrRectanglesAll.SetAt(oldPos+loop,m_arrRectanglesAll[oldPos+loop+1]);
					}
					//将当前控件移动到新位置
					m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
					TCCorrectZOrders();
					return;
				}
				else if(oldPos > newPos)
				{
					for(int loop=0;loop<oldPos-newPos;loop++)
					{
						m_arrRectanglesAll.SetAt(oldPos-loop,m_arrRectanglesAll[oldPos-loop-1]);
					}
					//将当前控件移动到新位置
					m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
					TCCorrectZOrders();
					return;
				}		
			}
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());

				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd()==pCtrl)
						{
							oldPos = j;
							newPos = ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize() - pos;
							pTemp = ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j];
							//以前的位置
							//先移动要重新排列的控件
							if(oldPos < newPos)
							{
								for(int loop=0;loop<newPos-oldPos;loop++)
								{
									((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.SetAt(oldPos+loop,((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[oldPos+loop+1]);
								}
								//将当前控件移动到新位置
								((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
								((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.TCCorrectZOrders();
								return;
							}
							else if(oldPos > newPos)
							{
								for(int loop=0;loop<oldPos-newPos;loop++)
								{
									((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.SetAt(oldPos-loop,((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[oldPos-loop-1]);
								}
								//将当前控件移动到新位置
								((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
								((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.TCCorrectZOrders();
								return;
							}		
						}
					}
				}
			}
		}
		else
		{
			if(m_arrRectanglesAll[i]->GetCWnd() == pCtrl)
			{
				oldPos = i;
				newPos = m_arrRectanglesAll.GetSize() - pos;
				pTemp = m_arrRectanglesAll[i];
				//以前的位置
				//先移动要重新排列的控件
				if(oldPos < newPos)
				{
					for(int loop=0;loop<newPos-oldPos;loop++)
					{
						m_arrRectanglesAll.SetAt(oldPos+loop,m_arrRectanglesAll[oldPos+loop+1]);
					}
					//将当前控件移动到新位置
					m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
					TCCorrectZOrders();
					return;
				}
				else if(oldPos > newPos)
				{
					for(int loop=0;loop<oldPos-newPos;loop++)
					{
						m_arrRectanglesAll.SetAt(oldPos-loop,m_arrRectanglesAll[oldPos-loop-1]);
					}
					//将当前控件移动到新位置
					m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
					TCCorrectZOrders();
					return;
				}		
			}
		}
	}
}

int CControlTracker::TCGetControlTrackerSize(CWnd* pCtrl)
{
	//先判断Tab中
	for(int i=0;i<m_arrRectanglesAll.GetSize();i++)
	{
		if (m_arrRectanglesAll[i]->GetCtlType() == TCTT_TAB)
		{
			if(m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==pCtrl->m_hWnd)
				return m_arrRectanglesAll.GetSize();
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());

				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd()==pCtrl)
							return ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					}
				}
			}
		}
		else
		{
			if(m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==pCtrl->m_hWnd)
				return m_arrRectanglesAll.GetSize();
		}
	}
}

////////////////////////////当前创建的控件置为选中状态/////////////////////////////
void CControlTracker::SetCreateCtrlSelected( CControlRect* pObject)
{
	//首先将以前选中的DeSelect掉
	DeSelectAll();
	m_arrRectanglesSelected.Add( pObject );
	pObject->m_bSelected = TRUE;
	m_pwndParent->Invalidate();
}
////////////////////////////当前创建的控件置为选中状态/////////////////////////////
BOOL CControlTracker::IsSelected( CControlRect* pObject ) const
{
    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {
        if( m_arrRectanglesSelected[i] == pObject )
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CControlTracker::IsBasicElement( CControlRect* pObject ) const
{
    if( m_BasicElement != NULL && m_BasicElement == pObject)
    {
        return TRUE;
    }

    return FALSE;
}

void CControlTracker::SelectUnConditional( CControlRect* pObject )
{
    m_arrRectanglesSelected.Add( pObject );
    pObject->m_bSelected = TRUE;
    //////不能在这儿添加画框选中后的基准控件吗
}

//////////////////////高亮显示的相关操作/////////////////////////////
//void CControlTracker::SelectWithHighLight( CControlRect* pObject )
//{
//    m_BasicElement = pObject;
//    pObject->m_highLight = TRUE;
//}

//////////////////////高亮显示的相关操作/////////////////////////////
BOOL CControlTracker::IsCotainHighLight()
{
    for(int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {
        if(m_arrRectanglesSelected[i]->m_highLight && m_arrRectanglesSelected[i]->m_bSelected )
        { 	
            return TRUE;
        }
    }
    return FALSE;
}
///////////////////高亮显示的相关操作//////////////////////////////


///////////////////画框得到默认基准控件////////////////////////////
BOOL CControlTracker::CreateDefaultBasicCtrl()
{
    int size = m_arrRectanglesSelected.GetSize();
  
	if(size < 1)
		return FALSE;

	else if(size == 1)
		m_arrRectanglesSelected.GetAt( 0 )->TCUpdateProperty();

	else
	{
		m_arrRectanglesSelected.GetAt( 0 )->m_highLight = TRUE;
		m_BasicElement = m_arrRectanglesSelected.GetAt( 0 );
		m_BasicElement->TCUpdateProperty();
	}

    this->m_pwndParent->Invalidate();
    return TRUE;
}
///////////////////画框得到默认基准控件////////////////////////////

BOOL CControlTracker::Select( CControlRect* pObject )	
{
    if( ! IsSelected( pObject ) )
    {
        SelectUnConditional( pObject );

        return TRUE;
    }

    return FALSE;
}


//锁定
void CControlTracker::Lock( CControlRect* pObject )
{
	if( pObject != NULL )
		pObject->m_bLockedUp = TRUE;
}

//解锁
void CControlTracker::UnLock( CControlRect* pObject )
{
	if( pObject != NULL )
		pObject->m_bLockedUp = FALSE;
}

BOOL CControlTracker::Toggle( CControlRect* pObject )
{
    if( IsSelected( pObject )&&IsCotainHighLight())
    {
        ///反选
        //DeSelect( pObject );
        for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
        {
            m_arrRectanglesSelected[i]->m_highLight = FALSE;
        }
        pObject->m_bSelected = TRUE;
        pObject->m_highLight = TRUE;
        m_BasicElement = pObject;
		m_BasicElement->TCUpdateProperty();
        return TRUE;
    }
    else if( IsSelected( pObject )&&!IsCotainHighLight() )
    {
        pObject->m_bSelected = TRUE;
        pObject->m_highLight = TRUE;
        m_BasicElement = pObject;
		m_BasicElement->TCUpdateProperty();
        return TRUE;
    }
    else
    {
        //选中
        Select(pObject);
        if(!IsCotainHighLight())
        {
            pObject->m_bSelected = TRUE;
            pObject->m_highLight = TRUE;
            m_BasicElement = pObject;
			m_BasicElement->TCUpdateProperty();
        }
        return TRUE;
    }

}

BOOL CControlTracker::DeSelect( CControlRect* pObject )
{
    //临时数组
    CArray< CControlRect*, CControlRect* > TempArray;

    if( IsSelected( pObject ) )
    {
        for(int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
        {
            if( m_arrRectanglesSelected[i] == pObject )
            { 				
                pObject->m_bSelected = FALSE;
                pObject->m_highLight = FALSE;
                if(m_BasicElement ==pObject)
                    m_BasicElement=NULL;
				//return TRUE;
            }
			//这里注释掉,是因为选中的状态的切换问题,会影响到删除控件的操作
            else
            {
                TempArray.Add( m_arrRectanglesSelected[i] );
            }
			//这里注释掉,是因为选中的状态的切换问题,会影响到删除控件的操作
        }

        m_arrRectanglesSelected.RemoveAll();

        for(int i=0; i < TempArray.GetSize(); i++ )
        {
            m_arrRectanglesSelected.Add( TempArray[i] );
        }

        return ( ! pObject->m_bSelected );
    }

    return FALSE;
}


//////////////////////////////////////////////////////////////
BOOL CControlTracker::RemoveFromAll(CWnd* ctl)
{
		int index = IsIncludeControl(ctl);
		if( index == -1 )
			return FALSE;
		else
		{
			m_arrRectanglesAll.RemoveAt(index);
			return TRUE;
		}
}

int CControlTracker::DeSelectAll()
{
    int UpperBound = m_arrRectanglesSelected.GetSize();

    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {
        m_arrRectanglesAll[i]->m_bSelected = FALSE;
        m_arrRectanglesAll[i]->m_highLight = FALSE;
    }

    m_arrRectanglesSelected.RemoveAll();
    m_BasicElement = NULL;
    return UpperBound;
}

BOOL CControlTracker::Track( const CPoint & point, UINT nFlags , BOOL bTrackRubberBand )
{
    ASSERT( m_pwndParent != NULL );

    int InitialCount = 0;
    CRect InitialBoundRect;
    int pX, pY;

    //遍历数组元素然后移动
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {				
        int nHit = m_arrRectanglesAll[i]->HitTest( point );
        //没有拖动控件大小
        if( nHit != CRectTracker::hitNothing  )
        {
            // See if the control key was down
            //看功能键是否按下
            if( ( nFlags & MK_CONTROL ) != 0 )
            {
                //Toggle是切换控件的选中状态的
                Toggle( m_arrRectanglesAll[i] );
                m_pwndParent->Invalidate();
                return TRUE;						
            }						
            // See if the shift key was down
            if( ( nFlags & MK_SHIFT ) != 0 )
            {
                Toggle( m_arrRectanglesAll[i] );
                m_pwndParent->Invalidate();
                return TRUE;
            }

            if( ! IsSelected( m_arrRectanglesAll[i] ) ) 
                InitialCount = DeSelectAll();

            Select( m_arrRectanglesAll[i] );

            if( nHit == CRectTracker::hitMiddle )
            {		
						
                InitialBoundRect = GetBoundsRect();

				//锁定处理
				for(int count = 0;count < m_arrRectanglesSelected.GetCount(); count++)
				{
					if( m_arrRectanglesSelected[count]->m_bLockedUp )
					{
						m_pwndParent->Invalidate();
						return TRUE;
					}
				}

                m_rect.CopyRect( & InitialBoundRect );
                CRectTracker::Track( m_pwndParent, point );

                pX = InitialBoundRect.left - m_rect.left;
                pY = InitialBoundRect.top - m_rect.top;
		
                // Offset each selected control

				//重新设置每个控件的位置
				//避免没有移动位置就添加撤销恢复操作
				if( m_rect != InitialBoundRect )
				{
					SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
					pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());
					for( int j = 0;j < m_arrRectanglesSelected.GetSize();j++ )
					{
						CRect rect(m_arrRectanglesSelected[j]->m_rect.left - pX, 
							m_arrRectanglesSelected[j]->m_rect.top - pY,
							m_arrRectanglesSelected[j]->m_rect.right - pX,
							m_arrRectanglesSelected[j]->m_rect.bottom - pY);
						pCommand->m_TCCtrlSort[j].SetTCCtrl(m_arrRectanglesSelected.GetAt( j )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( j )->GetCtlType());
					}
					switch(m_parentWndType)
					{
					case TCPARENT_DESDIN:
						((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
						break;
					case TCPARENT_TABPAGE:
						((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
						break;
					default:
						break;
					}
				}
				else
				{
					int curselect = m_arrRectanglesSelected.GetCount();
					if(curselect>1)
					{
						for(int k=0;k<curselect;k++)
						{
							if(m_arrRectanglesSelected[k]->TCGetHighSelected())
								m_arrRectanglesSelected[k]->TCUpdateProperty();
						}
					}
					else if(curselect==1)
						m_arrRectanglesSelected[0]->TCUpdateProperty();
					m_pwndParent->Invalidate();
					return TRUE;
				}
			}
            else
            {
				//锁定处理
				if( m_arrRectanglesAll[i]->m_bLockedUp )
					return TRUE;
                m_arrRectanglesAll[i]->Track(m_pwndParent,point);				
            }
            m_pwndParent->Invalidate();
            return TRUE;
        }
    }	
    // Exit if the shift key or the control key was down
    //按下功能键是退出
    if( ( ( nFlags & MK_SHIFT ) != 0 ) || ( ( nFlags & MK_CONTROL ) != 0 ) )
    {
        return FALSE;
    }

    // Deselect all the controls
    //去掉选中的所有控件
    DeSelectAll();

    if( bTrackRubberBand == FALSE )
    {
        m_pwndParent->Invalidate( WM_PAINT );
        return FALSE;
    }

    // Start the selection rect
    //这儿开始画选取框
    TrackRubberBand( m_pwndParent, point );
    //Rect的当前大小好像每一次拖动后都要正规化
    m_rect.NormalizeRect();

    CRect temp;
    // Now select objects according to the resized rectangle
    /////根据画出的范围看是否选中控件::总是进来
    for(int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {
        temp.IntersectRect( & m_rect, & m_arrRectanglesAll[i]->m_rect );

        if( m_arrRectanglesAll[i]->m_rect.EqualRect( & temp ) )
        {
            Select(m_arrRectanglesAll[i] );	
			//这儿是进行默认基准控件的操作
			//CreateDefaultBasicCtrl(false);
        }
    }
	//这儿是进行默认基准控件的操作
	CreateDefaultBasicCtrl();
    //刷新
    m_pwndParent->Invalidate();
    return FALSE;
}

//选中控件左移一个位置
BOOL CControlTracker::MoveLeftOne( )
{
    CRect parentRect;
    m_pwndParent->GetClientRect(&parentRect);
    //m_pwndParent->ClientToScreen(&parentRect);
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());

    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        CRect ctlRect;
        //这儿是设置光标
        m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&ctlRect);
        m_pwndParent->ScreenToClient(&ctlRect);
        if(ctlRect.left-1<parentRect.left)
        {
            ctlRect.left=parentRect.left;
            ctlRect.right=ctlRect.left+ctlRect.Width();
        }
        else
        {
            ctlRect.left=ctlRect.left-1;
            ctlRect.right=ctlRect.right-1;
        }
		//在多选的情况,只刷新基准控件
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),ctlRect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());

    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
    //m_pwndParent->Invalidate();
    return TRUE;
}

BOOL CControlTracker::MoveRightOne()
{
    CRect parentRect;
    m_pwndParent->GetClientRect(&parentRect);
    //m_pwndParent->ClientToScreen(&parentRect);
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());

    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        CRect ctlRect;
        //这儿是设置光标
        m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&ctlRect);
        m_pwndParent->ScreenToClient(&ctlRect);
        if(ctlRect.right+1>parentRect.right)
        {
            ctlRect.left=parentRect.right-ctlRect.Width();
            ctlRect.right=parentRect.right;
        }
        else
        {
            ctlRect.left=ctlRect.left+1;
            ctlRect.right=ctlRect.right+1;
        }
		//在多选的情况,只刷新基准控件
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),ctlRect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());

    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}

    //m_pwndParent->Invalidate();

    return TRUE;
}

BOOL CControlTracker::MoveTopOne()
{
    CRect parentRect;
    m_pwndParent->GetClientRect(&parentRect);
    //m_pwndParent->ClientToScreen(&parentRect);
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());
    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        CRect ctlRect;
        //这儿是设置光标
        m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&ctlRect);
        m_pwndParent->ScreenToClient(&ctlRect);
        if(ctlRect.top-1<parentRect.top)
        {
            ctlRect.top=parentRect.top;
            ctlRect.bottom=ctlRect.top+ctlRect.Height();
        }
        else
        {
            ctlRect.top=ctlRect.top-1;
            ctlRect.bottom=ctlRect.bottom-1;
        }
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),ctlRect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());

    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
    return TRUE;
}

BOOL CControlTracker::MoveBottomOne()
{
    CRect parentRect;
    m_pwndParent->GetClientRect(&parentRect);
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());
    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        CRect ctlRect;
        //这儿是设置光标
        m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&ctlRect);
        m_pwndParent->ScreenToClient(&ctlRect);
        if(ctlRect.bottom+1>parentRect.bottom)
        {
            ctlRect.top=parentRect.bottom-ctlRect.Height();
            ctlRect.bottom=parentRect.bottom;
        }
        else
        {
            ctlRect.top=ctlRect.top+1;
            ctlRect.bottom=ctlRect.bottom+1;
        }
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),ctlRect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
		
    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
    return TRUE;
}

CRect CControlTracker::GetBoundsRect() const
{
    int Left(0), Right(0), Top(0), Bottom(0);
    int pLeft(0), pRight(0), pTop(0), pBottom(0);

    pLeft = m_arrRectanglesSelected[0]->m_rect.left;
    pTop = m_arrRectanglesSelected[0]->m_rect.top;
    pRight = m_arrRectanglesSelected[0]->m_rect.right;
    pBottom = m_arrRectanglesSelected[0]->m_rect.bottom;

    for( int i = 1; i < m_arrRectanglesSelected.GetSize(); i++ )
    {
        Left = m_arrRectanglesSelected[ i ]->m_rect.left;
        Top = m_arrRectanglesSelected[ i ]->m_rect.top;
        Right = m_arrRectanglesSelected[ i ]->m_rect.right;
        Bottom = m_arrRectanglesSelected[ i ]->m_rect.bottom;

        if( Left < pLeft )
            pLeft = Left;

        if( Top < pTop )
            pTop = Top;

        if( Bottom > pBottom )
            pBottom = Bottom;

        if( Right > pRight )
            pRight = Right;
    }

    return CRect( pLeft, pTop, pRight, pBottom );
}

void CControlTracker::GetBoundsPointCur(POINT clientPoint,LPPOINT lpPoint)
{
    CRect BoundRect=GetBoundsRect();
    lpPoint->x=clientPoint.x-BoundRect.left;
    lpPoint->y=clientPoint.y-BoundRect.top;
    return;
}

void CControlTracker::Draw( CDC* pDC ) const
{
    // Loop can iterate through all elements of the selected array
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {		
		if( m_arrRectanglesAll[i]->m_bSelected )
		{
			if(m_arrRectanglesAll[i]->m_highLight)
			{
				if(m_arrRectanglesAll[i]->m_bLockedUp)
				{
					//这儿画控件被选中且为基准控件后的橡皮筋 
					CPen pen(PS_SOLID,1,RGB(255,0,0)); 
					m_arrRectanglesAll[i]->MyDraw(pDC,&pen,TRUE);
				}
				else
				{
					//这儿画控件被选中且为基准控件后的橡皮筋 
					CPen pen(PS_SOLID,1,RGB(255,0,0)); 
					m_arrRectanglesAll[i]->MyDraw(pDC,&pen);
				}
			}
			else/* if(m_arrRectanglesAll[i]->m_highLight == FALSE)*/
			{
				if(m_arrRectanglesAll[i]->m_bLockedUp)
				{
					//这儿画控件被选中且为基准控件后的橡皮筋 
					//CPen pen(PS_DOT,1,RGB(255,255,128)); 
 					//m_arrRectanglesAll[i]->MyDraw(pDC,&pen);
					m_arrRectanglesAll[i]->DrawLocked(pDC);
				}
				else
				{
					//这儿只是画被普通选中后的控件
					m_arrRectanglesAll[i]->Draw(pDC);
				}	
			}
		}    
    }	
}

BOOL CControlTracker::SetCursor( UINT nHitTest, UINT message )
{
    ASSERT( m_pwndParent != NULL );

    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {	
		//这儿是设置光标
		if( m_arrRectanglesAll[i]->SetCursor(m_pwndParent, nHitTest))
			return TRUE; 
    }	

    return FALSE;
}


//这个是判断是否包含了此控件,并返回期位置
int CControlTracker::IsIncludeControl(CWnd* ctl)
{
    if(!ctl)
        return -1;
	/////////////////////////////3-28号设置Z序添加/////////////////////////////
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {	
		if(m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			if(m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==ctl->m_hWnd)
				return i;
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());

				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd()==ctl)
							return j;
					}
				}
			}
		}
        /////////////////////////////3-28号设置Z序添加/////////////////////////////
        if( m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==ctl->m_hWnd)
            return i;
    }	
    return -1;
}

int CControlTracker::IsInSelectControl(CWnd* ctl)
{
    if(!ctl)
        return -1;
    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        //这儿是设置光标
        if( m_arrRectanglesSelected[i]->GetCWnd()->m_hWnd==ctl->m_hWnd)
            return i;
    }	
    return -1;

}

//这个判断控件的名称只判断当前的设计面版和全局的界面设计config中的ID数据
bool CControlTracker::IsExistID(CString id)
{
	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{	
		//这儿是设置光标
		id.Trim();
		//如果有TAB控件先判断TAB控件里面是否存在此ID
		if( m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			if( m_arrRectanglesAll[i]->GetCtlID()==id )
				return true;
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());

				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlID()==id)
							return true;
					}
				}
			}
		}
		else if( m_arrRectanglesAll[i]->GetCtlID()==id)
			return true;
	}	

	return false;
}


CWnd* CControlTracker::GetCtlByID(CString id)
{
	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{	
		//这儿是设置光标
		id.Trim();

		//如果是Table控件要遍历Table立面的类型
		if( m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());
			for(int i=0;i<pTab->getTabPageCount();i++)
			{
				for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
				{
					if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlID()==id)
						return ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd();
				}
			}
		}
		else if( m_arrRectanglesAll[i]->GetCtlID()==id)
			return m_arrRectanglesAll[i]->GetCWnd();
	}	
    return NULL;
}

CControlRect* CControlTracker::GetRectByID(CString id)
{
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {	
        //这儿是设置光标
        id.Trim();

		//如果是Table控件要遍历Table里面的类型
		if( m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB )
		{
			if(m_arrRectanglesAll[i]->GetCtlID() == id)
				return m_arrRectanglesAll[i];
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());
				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					int TabNum = ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					for(int j=0;j<TabNum;j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlID()==id)
							return ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j];
					}
				}
			}
		}
		else if( m_arrRectanglesAll[i]->GetCtlID()==id)
            return m_arrRectanglesAll[i];
    }	
    return NULL;
}

CControlRect * CControlTracker::GetDisplayPropCtl()
{
    CControlRect * rtn=NULL;
    int size=m_arrRectanglesSelected.GetSize();
    if(size==0)
        return NULL;

    rtn=m_arrRectanglesSelected[0];

    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        //这儿是设置光标
        if( m_arrRectanglesSelected[i]->m_highLight)
            rtn=m_arrRectanglesSelected[i];
    }	
    return rtn;
}

bool CControlTracker::ChangeCtlID(CString oldID,CString newID)
{
	if(!MyValidator::validateNameNoException(newID))
		return false;

    if(IsExistID(newID))
        return false;

    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {	
        //这儿是设置光标
        oldID.Trim();
		if( m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB )
		{
			if( m_arrRectanglesAll[i]->GetCtlID() == oldID )
			{
				m_arrRectanglesAll[i]->SetCtlID(newID);
				return true;
			}
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());
				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					int TabNum = ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					for(int j=0;j<TabNum;j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlID() == oldID)
						{
							((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->SetCtlID(newID);
							return true;
						}
					}
				}
			}
		}
		else if( m_arrRectanglesAll[i]->GetCtlID()==oldID)
		{
			m_arrRectanglesAll[i]->SetCtlID(newID);
			return true;
		}        
    }		
    return false;

}

void CControlTracker::Update()
{

}

CString CControlTracker::BuildValidCtlID(TCTOOLTYPE type)
{
    if(CTCRunTimeManager::g_LangType==1)
    {
        if(type==TCTT_STATIC)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("Static"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_GROUP)
        {

            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("group"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }


            return _T("");
        }
        else if(type==TCTT_EDIT)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("edit"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_BUTTON)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("button"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_RADIO)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("radio"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_CHECK)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("check"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_COMBOBOX)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("ComboBox"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_PICTURE)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("picture"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }       
        else if(type==TCTT_PROGRESS)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("progress"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }     
        else if(type==TCTT_PATH)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("path"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_SLIDER)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("slider"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_DATE)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("date"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_LISTBOX)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("list"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
		else if(type==TCTT_TAB)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("tab"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        } 	
		else if(type==TCTT_TIMER)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("timer"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        } 	
		else if(type==TCTT_HOTKEY)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("hotkey"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
		else if(type==TCTT_EXPLORER)
		{
			for(int i=0;i<MAXCTRLLENGTH;i++)
			{
				CString id;
				id.Format(_T("%s%d"),_T("explorer"),i);
				if(!IsExistID(id))
				{
					return id;
				}
			}

			return _T("");
		}
		else if(type==TCTT_GRIDCTRL)
		{
			for(int i=0;i<MAXCTRLLENGTH;i++)
			{
				CString id;
				id.Format(_T("%s%d"),_T("gridctrl"),i);
				if(!IsExistID(id))
				{
					return id;
				}
			}

			return _T("");
		}
    }
    else if(CTCRunTimeManager::g_LangType == 0)
    {
        if(type==TCTT_STATIC)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("标签"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else  if(type==TCTT_EDIT)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("编辑框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_GROUP)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("分组框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_BUTTON)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("按钮"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_RADIO)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("单选框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_CHECK)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("复选框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_COMBOBOX)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("下拉框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_PICTURE)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("图片框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        } 
        else if(type==TCTT_PROGRESS)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("进度条"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_PATH)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("浏览框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_SLIDER)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("滑块"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_DATE)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("日期框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_LISTBOX)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("列表框"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
	    else if(type==TCTT_TAB)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("选项卡"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
		else if(type==TCTT_TIMER)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("定时器"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }
            return _T("");
        } 
		else if(type==TCTT_HOTKEY)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("热键"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }
            return _T("");
        } 
		else if(type==TCTT_EXPLORER)
		{
			for(int i=0;i<MAXCTRLLENGTH;i++)
			{
				CString id;
				id.Format(_T("%s%d"),_T("浏览器"),i);
				if(!IsExistID(id))
				{
					return id;
				}
			}

			return _T("");
		}
		else if(type==TCTT_GRIDCTRL)
		{
			for(int i=0;i<MAXCTRLLENGTH;i++)
			{
				CString id;
				id.Format(_T("%s%d"),_T("表格"),i);
				if(!IsExistID(id))
				{
					return id;
				}
			}

			return _T("");
		}
    }
}
/////////////这儿遍历选中的控件然后垂直居中操作///////////////////////////////////
void CControlTracker::CenterWindowV()
{
	int numOfCtrl = m_arrRectanglesSelected.GetSize();

	//只选中一个控件的时候
	if( 1 == numOfCtrl )
	{
		CRect rectPanel;
		CRect rectStart;
		this->m_pwndParent->GetClientRect(&rectPanel);

		m_arrRectanglesSelected.GetAt( 0 )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);

		if(rectPanel.Height()/2 == rectStart.top+rectStart.Height()/2)
			return;

		SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
		pCommand->SetTCCtrlNum(numOfCtrl);

		
        //m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectPanel.Height()/2-rectStart.Height()/2,rectStart.Width(),rectStart.Height(),TRUE);

		CRect rect(rectStart.left,rectPanel.Height()/2-rectStart.Height()/2,rectStart.left+rectStart.Width(),rectPanel.Height()/2-rectStart.Height()/2+rectStart.Height());
        //m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
        //this->m_pwndParent->ScreenToClient(&rect);	
        //m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[0].SetTCCtrl(m_arrRectanglesSelected.GetAt( 0 )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( 0 )->GetCtlType());

		switch(m_parentWndType)
		{
		case TCPARENT_DESDIN:
			((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
			break;
		case TCPARENT_TABPAGE:
			((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
			break;
		default:
			break;
		}
		return;
	}
	

	CRect selectRect = GetBoundsRect();
	int selectHeight=selectRect.Height()/2;

	CRect diaRect;
	this->m_pwndParent->GetClientRect(&diaRect);
	int BasicHeight = diaRect.Height()/2;
	int avarage = BasicHeight - (selectRect.top + selectHeight);

	if(selectRect.top + selectHeight == BasicHeight)
		return;

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(numOfCtrl);

	for(int i=0;i<numOfCtrl;i++)
	{
			CRect rectStart;
			m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
			this->m_pwndParent->ScreenToClient(&rectStart);		

			CRect rect(rectStart.left,rectStart.top+avarage,rectStart.right,rectStart.top+avarage+rectStart.Height());
			//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
			//this->m_pwndParent->ScreenToClient(&rect);	
			//m_arrRectanglesSelected[i]->SetRect(rect); 
			pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}    
}
/////////////这儿遍历选中的控件然后垂直居中操作//////////////////////////////////////

/////////////这儿遍历选中的控件然后水平居中操作///////////////////////////////////
void CControlTracker::CenterWindowH()
{
    int numOfCtrl = m_arrRectanglesSelected.GetSize();

	if( 1 == numOfCtrl  )
	{
		CRect rectPanel;
		CRect rectStart;
		this->m_pwndParent->GetClientRect(&rectPanel);

		m_arrRectanglesSelected.GetAt( 0 )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);

		if(rectPanel.Width()/2 == rectStart.left+rectStart.Width()/2)
			return;

		SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
		pCommand->SetTCCtrlNum(numOfCtrl);
		
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectPanel.Width()/2-rectStart.Width()/2,rectStart.top,rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(rectPanel.Width()/2-rectStart.Width()/2,rectStart.top,rectPanel.Width()/2-rectStart.Width()/2+rectStart.Width(),rectStart.top+rectStart.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[0].SetTCCtrl(m_arrRectanglesSelected.GetAt( 0 )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( 0 )->GetCtlType());
		
		switch(m_parentWndType)
		{
		case TCPARENT_DESDIN:
			((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
			break;
		case TCPARENT_TABPAGE:
			((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
			break;
		default:
			break;
		}
		return;
	}

	CRect selectRect = GetBoundsRect();
	int selectWidth=selectRect.Width()/2;

	CRect diaRect;
	this->m_pwndParent->GetClientRect(&diaRect);
	int BasicWidth = diaRect.Width()/2;
	int avarage = BasicWidth - (selectRect.left + selectWidth);

	if(selectRect.left + selectWidth == BasicWidth)
		return;

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(numOfCtrl);

	for(int i=0;i<numOfCtrl;i++)
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left+avarage,rectStart.top,rectStart.Width(),rectStart.Height(),TRUE);

		CRect rect(rectStart.left+avarage,rectStart.top,rectStart.left+avarage+rectStart.Width(),rectStart.top+rectStart.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect); 
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
///////////这儿遍历选中的控件然后水平居中操作//////////////////////////////////////

////////////////////////选中控件左对齐////////////////////////////////////////
void CControlTracker::AlignLeft()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.left != ctrlRect.left)
			break;

		if(i == Index - 1)
			return;
	}
	
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);

    for(int i=0; i < Index; i++ )	
    {
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(m_Basicrect.left,rectStart.top,rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(m_Basicrect.left,rectStart.top,m_Basicrect.left+rectStart.Width(),rectStart.top+rectStart.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect); 
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
////////////////////////选中控件左对齐/////////////////////////////////////////

//////////////////////////////选中控件右对齐/////////////////////////////
void CControlTracker::AlignRight()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.right != ctrlRect.right)
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
    for(int i=0; i < Index; i++ )	
    {
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(,m_Basicrect.right-rectStart.Width(),rectStart.top,rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(m_Basicrect.right-rectStart.Width(),rectStart.top,m_Basicrect.right,rectStart.bottom);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		// this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////选中控件右对齐/////////////////////////////

//////////////////////////////选中控件低端对齐/////////////////////////////
void CControlTracker::AlignBottom()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.bottom != ctrlRect.bottom)
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,m_Basicrect.bottom-rectStart.Height(),rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(rectStart.left,m_Basicrect.bottom-rectStart.Height(),rectStart.right,m_Basicrect.bottom);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////选中控件低端对齐/////////////////////////////

//////////////////////////////选中控件顶端对齐/////////////////////////////
void CControlTracker::AlignTop()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.top != ctrlRect.top)
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,m_Basicrect.top,rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(rectStart.left,m_Basicrect.top,rectStart.right,m_Basicrect.top+rectStart.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////选中控件顶端对齐/////////////////////////////

/////////////////////////使控件以基准控件为标准大小一致/////////////////////
void CControlTracker::SetSameSize()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i = 0; i < Index; i++)
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if( m_Basicrect.Width() != ctrlRect.Width() || m_Basicrect.Height() != ctrlRect.Height() )
			break;

		if( i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	
    for(int i=0; i < Index; i++ )	
    {
			CRect rectStart;
			m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
			this->m_pwndParent->ScreenToClient(&rectStart);
			//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectStart.top,m_Basicrect.Width(),m_Basicrect.Height(),TRUE);

			CRect rect(rectStart.left,rectStart.top,rectStart.left+m_Basicrect.Width(),rectStart.top+m_Basicrect.Height());
		   // m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
			//this->m_pwndParent->ScreenToClient(&rect);	
			//m_arrRectanglesSelected[i]->SetRect(rect);
			pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////使选中控件高度相同////////////////////
void CControlTracker::SetSameHeight()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int count = 0;count < Index;count++)
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( count )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.Height() != ctrlRect.Height())
			break;
		if(count == Index - 1)
			return;
	}
	
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);

	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectStart.top,rectStart.Width(),m_Basicrect.Height(),TRUE);
		CRect rect(rectStart.left,rectStart.top,rectStart.right,rectStart.top+m_Basicrect.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////

/////////////////////使选中控件宽度相同 /////////////////////////////////////
void CControlTracker::SetSameWidth()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL )
        return;

	CRect m_Basicrect;
	m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if( m_Basicrect.Width() != ctrlRect.Width() )
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectStart.top,m_Basicrect.Width(),rectStart.Height(),TRUE);

		CRect rect(rectStart.left,rectStart.top,rectStart.left+m_Basicrect.Width(),rectStart.bottom);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////选中控件垂直居中对齐///////////////////////////////////////
void CControlTracker::CenterAlignV()
{
	int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL )
        return;

	CRect m_Basicrect;
	m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect rectCtrl;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectCtrl);
		this->m_pwndParent->ScreenToClient(&rectCtrl);

		if(m_Basicrect.top + m_Basicrect.Height()/2 != rectCtrl.top + rectCtrl.Height()/2)
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectStart.top,m_Basicrect.Width(),rectStart.Height(),TRUE);

		CRect rect(rectStart.left,m_Basicrect.top+m_Basicrect.Height()/2-rectStart.Height()/2,rectStart.right,m_Basicrect.bottom-m_Basicrect.Height()/2+rectStart.Height()/2);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}  
}
/////////////////////////////////////////////////////////////////////////

////////////////////////////选中控件水平居中对齐///////////////////////////////////
void CControlTracker::CenterAlignH()
{
	int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL )
        return;

	CRect m_Basicrect;
	m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect rectCtrl;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectCtrl);
		this->m_pwndParent->ScreenToClient(&rectCtrl);

		if(m_Basicrect.left + m_Basicrect.Width()/2 != rectCtrl.left + rectCtrl.Width()/2)
			break;

		if(i == Index - 1)
			return;
	}

	
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);

		CRect rect(m_Basicrect.left+m_Basicrect.Width()/2-rectStart.Width()/2,rectStart.top,m_Basicrect.right-m_Basicrect.Width()/2+rectStart.Width()/2,rectStart.bottom);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	} 
}
////////////////////////////选中控件水平居中///////////////////////////////////

///////////////////////////////////////均匀水平间隔////////////////////////////////////
void CControlTracker::UniformSpaceH()
{
	int numOfCtrl = m_arrRectanglesSelected.GetSize();

	UniformCtrlCommand *pCommand = new UniformCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(numOfCtrl - 2);
	//控件排序,选择法
	for (int i = 0; i < numOfCtrl - 1; i++)
	{
		int k = i;
		for (int j = i + 1; j < numOfCtrl; j++)
		{
			//获取两个控件的坐标
			CRect rectJ, rectK;
			m_arrRectanglesSelected[j]->GetCWnd()->GetWindowRect(&rectJ);
			m_arrRectanglesSelected[k]->GetCWnd()->GetWindowRect(&rectK);
			//横向排序
			if (rectJ.left < rectK.left)
				k = j;
		}
		if (k != i)
		{
			CControlRect *temp = m_arrRectanglesSelected[i];
			m_arrRectanglesSelected[i] = m_arrRectanglesSelected[k];
			m_arrRectanglesSelected[k] = temp;
		}
	}
	
	//计算控件间的总间距//////////////////////////////////////////////
	int spacing = 0;
	for (int i = 0; i < numOfCtrl - 1; i++)
	{
		CRect rect1, rect2;
		//获取两个控件的坐标
		m_arrRectanglesSelected[i + 1]->GetCWnd()->GetWindowRect(&rect1);
		m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&rect2);
		//计算控件的横向间隔
		spacing += rect1.left - rect2.right;
	}

	//按平均间距布局控件///////////////////////////////////////////////
	//计算平均间距和余下的间隔
	int spacingAvg = abs(spacing) / (numOfCtrl - 1);
	int spacingRemain = abs(spacing) % (numOfCtrl - 1);

	//恢复平均间距的正负
	if (spacing < 0)
		spacingAvg = -spacingAvg;

	/*CRect rect1,TempRect1;
	m_arrRectanglesSelected[0]->GetCWnd()->GetWindowRect(&rect1);
	TempRect1 = rect1;
	this->m_pwndParent->ScreenToClient(&rect1);
	pCommand->m_TCCtrlSort[0].SetTCCtrl(m_arrRectanglesSelected[0]->GetCWnd(),TempRect1,rect1,m_arrRectanglesSelected[0]->GetCtlType());*/

	//控件布局
	for (int i = 1; i < numOfCtrl - 1 ; i++)
	{
		//获得前后两两控件的坐标值
		CRect rect1, rect2,rectTemp;
		m_arrRectanglesSelected[i - 1]->GetCWnd()->GetWindowRect(&rect1);
		m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&rect2);

		rectTemp = rect2;

		//获取控件的宽度
		int width = rect2.right - rect2.left;
		//根据前一个控件和平均间距,重定位控件
		rect2.left = rect1.right + spacingAvg;
		//加上余下的间隔
		if (i <= spacingRemain)
			rect2.left += ((spacing > 0) ? 1 : -1);
		//恢复控件的宽度
		rect2.right = rect2.left + width;

		//定位控件,移动控件到均距的位置
		this->m_pwndParent->ScreenToClient(&rect2);
		m_arrRectanglesSelected[i]->GetCWnd()->MoveWindow(&rect2);
		//m_arrRectanglesSelected[i]->SetRect(rect2);	
		pCommand->m_TCCtrlSort[i - 1].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rectTemp,rect2,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}

	/*CRect rect2,TempRect2;
	m_arrRectanglesSelected[numOfCtrl - 1]->GetCWnd()->GetWindowRect(&rect2);
	TempRect2 = rect2;
	this->m_pwndParent->ScreenToClient(&rect2);
	pCommand->m_TCCtrlSort[numOfCtrl - 1].SetTCCtrl(m_arrRectanglesSelected[numOfCtrl - 1]->GetCWnd(),TempRect2,rect2,m_arrRectanglesSelected[numOfCtrl - 1]->GetCtlType());*/

	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}

////////////////////////////////////均匀垂直间隔////////////////////////////////////
void CControlTracker::UniformSpaceV()
{
	int numOfCtrl = m_arrRectanglesSelected.GetSize();
	UniformCtrlCommand *pCommand = new UniformCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(numOfCtrl - 2);

	for (int i = 0; i < numOfCtrl - 1; i++)
	{
		int k = i;
		for (int j = i + 1; j < numOfCtrl; j++)
		{
			//获取两个控件的坐标
			CRect rectJ, rectK;
			m_arrRectanglesSelected[j]->GetCWnd()->GetWindowRect(&rectJ);
			m_arrRectanglesSelected[k]->GetCWnd()->GetWindowRect(&rectK);
			//纵向排序
			if (rectJ.top < rectK.top)
				k = j;
		}
		if (k != i)
		{
			CControlRect *temp = m_arrRectanglesSelected[i];
			m_arrRectanglesSelected[i] = m_arrRectanglesSelected[k];
			m_arrRectanglesSelected[k] = temp;
		}
	}

	//计算控件间的总间距////////////////////////////////////////////////////
	int spacing = 0;
	for (int i = 0; i < numOfCtrl - 1; i++)
	{
		CRect rect1, rect2;
		//获取两个控件的坐标
		m_arrRectanglesSelected[i + 1]->GetCWnd()->GetWindowRect(&rect1);
		m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&rect2);
		//计算控件的纵向间隔
		spacing += rect1.top - rect2.bottom;
	}

	//按平均间距布局控件/////////////////////////////////////////////////
	//计算平均间距和余下的间隔
	int spacingAvg = abs(spacing) / (numOfCtrl - 1);
	int spacingRemain = abs(spacing) % (numOfCtrl - 1);

	

		//恢复平均间距的正负
		if (spacing < 0)
			spacingAvg = -spacingAvg;

		/*CRect rect1,TempRect1;
		m_arrRectanglesSelected[0]->GetCWnd()->GetWindowRect(&rect1);
		TempRect1 = rect1;
		this->m_pwndParent->ScreenToClient(&rect1);
		pCommand->m_TCCtrlSort[0].SetTCCtrl(m_arrRectanglesSelected[0]->GetCWnd(),TempRect1,rect1,m_arrRectanglesSelected[0]->GetCtlType());*/

	//控件布局
	for (int i = 1; i < numOfCtrl - 1; i++)
	{
		//获得前后两两控件的坐标值
		CRect rect1, rect2,tempRect;
		m_arrRectanglesSelected[i - 1]->GetCWnd()->GetWindowRect(&rect1);
		m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&rect2);

		tempRect = rect2;
		
		//获取控件的高度
		int height = rect2.bottom - rect2.top;
		//根据前一个控件和平均间距,重定位控件
		rect2.top = rect1.bottom + spacingAvg;
		//加上余下的间隔
		if (i <= spacingRemain)
			rect2.top += ((spacing > 0) ? 1 : -1);
		//恢复控件的高度
		rect2.bottom = rect2.top + height;

		//定位控件,移动控件到均距的位置
		this->m_pwndParent->ScreenToClient(&rect2);
		m_arrRectanglesSelected[i]->GetCWnd()->MoveWindow(&rect2);
		//m_arrRectanglesSelected[i]->SetRect(rect2);	
		pCommand->m_TCCtrlSort[i - 1].SetTCCtrl(m_arrRectanglesSelected[i]->GetCWnd(),tempRect,rect2,m_arrRectanglesSelected[i]->GetCtlType());
	}

	/*CRect rect2,TempRect2;
	m_arrRectanglesSelected[numOfCtrl - 1]->GetCWnd()->GetWindowRect(&rect2);
	TempRect2 = rect2;
	this->m_pwndParent->ScreenToClient(&rect2);
	pCommand->m_TCCtrlSort[numOfCtrl - 1].SetTCCtrl(m_arrRectanglesSelected[numOfCtrl - 1]->GetCWnd(),TempRect2,rect2,m_arrRectanglesSelected[numOfCtrl - 1]->GetCtlType());*/

	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}