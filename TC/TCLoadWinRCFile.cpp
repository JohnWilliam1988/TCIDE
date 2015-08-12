#include "StdAfx.h"
#include "TCLoadWinRCFile.h"
#include "matchLib.h"
#include "TCCtlHeads.h"
#include "DiaTCDesign.h"


TCLoadWinRCFile::TCLoadWinRCFile(CString winInfo/*csFilePath/*,BOOL readFile*/)
{
	m_pBuffer=NULL;
	m_isEnd=false;
	m_length=0;
	m_index=0;

	//if(readFile)   
	//{
	//	HANDLE hReadFile = CreateFileW(csFilePath,      // name of the readfile
	//		GENERIC_READ,           // open for writing
	//		FILE_SHARE_READ,        // do not share
	//		NULL,                   // default security
	//		OPEN_EXISTING,          // overwrite existing
	//		FILE_ATTRIBUTE_NORMAL,  // normal file
	//		NULL);                  // no attr. template

	//if (hReadFile == INVALID_HANDLE_VALUE) 
	//{ 
	//	CString csError;
	//	csError.Format(_T("加载界面配置文件失败: %s"),csFilePath);
	//	MessageBox(0,csError,_T("TC"),NULL); 
	//	//exit(0) ;
	//}

	//	DWORD dwFileSize = GetFileSize(hReadFile,  NULL);
	//	if(dwFileSize)
	//	{
	//		//读出配置文件内容
	//		DWORD dwReadSize = dwFileSize/sizeof(TCHAR)+1;
	//		m_pBuffer=new TCHAR[dwReadSize];
	//		m_length=dwReadSize;
	//		::memset(m_pBuffer,NULL,(dwReadSize)*sizeof(TCHAR));
	//		DWORD dword;
	//		ReadFile(hReadFile,m_pBuffer,dwFileSize,&dword,NULL);
	//		CloseHandle(hReadFile);
	//	}
	//	else
	//	{
	//		CString csError;
	//		csError.Format(_T("加载界面配置文件失败,空的界面配置文件: %s"),csFilePath);
	//		MessageBox(0,csError,_T("TC"),NULL); 
	//	}
	//	m_index++;
	//}
	//else
	//{
		m_length=winInfo.GetLength()+1;
		m_pBuffer=new TCHAR[m_length];	
		::memset(m_pBuffer,NULL,m_length);
		::wcscpy(m_pBuffer,winInfo.GetBuffer());
		winInfo.ReleaseBuffer();
	//}
}

TCLoadWinRCFile::~TCLoadWinRCFile()
{
	if(m_pBuffer)
		delete m_pBuffer;
}

bool TCLoadWinRCFile::GetLineString(CString &str)
{
	str.Empty();
	// Loop through the current chars in the buffer
	while (m_index < m_length)
	{
		// Get the current char out of the buffer
		TCHAR curCh = m_pBuffer[m_index];
		//
		//  See if its a white space char. If so, then process it. Else
		//  we've hit a non-space and need to return.
		//
		if (curCh==0x9||curCh==0x20||curCh==0xD||curCh==0xA)
		{
			// Eat this char
			//fCharIndex++;

			//
			//  'curCh' is a whitespace(x20|x9|xD|xA), so we only can have
			//  end-of-line combinations with a leading chCR(xD) or chLF(xA)
			//
			//  100000 x20
			//  001001 x9
			//  001010 chLF
			//  001101 chCR
			//  -----------
			//  000110 == (chCR|chLF) & ~(0x9|0x20)
			//
			//  if the result of thelogical-& operation is
			//  true  : 'curCh' must be xA  or xD
			//  false : 'curCh' must be x20 or x9
			//
			if ( ( curCh & (chCR|chLF) & ~(0x9|0x20) ) == 0 )
			{
				m_index++;
				str.AppendChar(curCh);
			}
			else
			{
				//handleEOL(curCh, false);
				m_index++;
				if(m_pBuffer[m_index]==chLF)
					m_index++;
				return true;
			}
		}
		else
		{
			str.AppendChar(curCh);
			// Eat this char
			m_index++;
		}
	}

	return str.GetLength()!=0;
}

CString TCLoadWinRCFile::GetCtrlType(CString &str)
{
	str.Trim();
	CString csRet;
	int index=str.Find(L' ');//查找空格符
	if(index==-1)
	{
		return str;	//说明是最后一个字符串
	}
	csRet=str.Left(index);
	str=str.Right(str.GetLength()-index-1);	
	return csRet;
}
CString TCLoadWinRCFile::GetCtrlID(CString &str)
{
	str.Trim();
	CString csRet;
	int index=str.Find(L' ');//查找空格符
	if(index==-1)
	{
		return csRet;	//说明是最后一个字符串
	}
	csRet=str.Left(index);
	str=str.Right(str.GetLength()-index-1);	
	return csRet;
}
//找出字符常量并处理转义字符
CString TCLoadWinRCFile::GetString(CString &str)//找出字符常量并处理转义字符
{

		str.Trim();
	CString tempcsstr;
	wchar_t temphelp[1024*256]={0};
	wchar_t *pointstr=str.LockBuffer();//temphelp;
	int i=0,size=0;
	if(*pointstr=='\"')
	{
		do
		{
			if(*pointstr=='\\')
			{
				if(*(pointstr+1)=='\"'||*(pointstr+1)=='\\')//碰到转义符的时候！
				{
					temphelp[i]=*(pointstr+1);
					pointstr+=2;//跳过转义符号！
					i++;
					size+=2;
				}
			}
			else
			{
				temphelp[i]=*pointstr;
				*pointstr++;
				i++;
				size++;
			}
			
		}
		while((*pointstr)!='\"');
	}
	str=str.Right(str.GetLength()-size-1);//吧字符串在原字符串中截取掉
	tempcsstr.Format(_T("%s"),&temphelp[1]);
	return tempcsstr;

	//str.Trim();
	//CString tempcsstr;
	//tempcsstr.Empty();
	//wchar_t *pointstr=str.LockBuffer();//temphelp;
	//int i=0,size=0;
	//if(*pointstr=='\"')
	//{
	//	pointstr++;
	//	do
	//	{
	//		if(*pointstr=='\\')
	//		{
	//			if(*(pointstr+1)=='\"'||*(pointstr+1)=='\\')//碰到转义符的时候！
	//			{
	//				tempcsstr.AppendChar(*(pointstr+1));
	//				pointstr+=2;//跳过转义符号！
	//				i++;
	//				size+=2;
	//			}
	//		}
	//		else
	//		{
	//			tempcsstr.AppendChar(*pointstr);
	//			*pointstr++;
	//			i++;
	//			size++;
	//		}
	//		
	//	}
	//	while((*pointstr)!='\"');
	//}
	//str=str.Right(str.GetLength()-size-1);//吧字符串在原字符串中截取掉
	//return tempcsstr;
}

//单独处理tab的名称
CString TCLoadWinRCFile::GetTabText(CString &lineBuf)
{
	lineBuf.Trim();
	CString csRet;
	int index=lineBuf.Find(L' ');//查找空格符
	if(index==-1)
	{
		index=lineBuf.GetLength();
	}
	csRet=lineBuf.Left(index);
	lineBuf=lineBuf.Right(lineBuf.GetLength()-index-1);	
	return csRet;
}

//得到控件的大小
bool TCLoadWinRCFile::GetCtrlRECT(CString &str,LPRECT rc)
{
	str.Trim();
	int index=str.Find(L',');//查找空格符
	if(index==-1)
	{
		return false;	//说明是最后一个字符串
	}
	CString csRet;
	csRet=str.Left(index);
	str=str.Right(str.GetLength()-index-1);	
	rc->left=_wtoi(csRet);
	index=str.Find(L',');//查找空格符
	if(index==-1)
	{
		return false;	//说明是最后一个字符串
	}
	csRet=str.Left(index);
	str=str.Right(str.GetLength()-index-1);	
	rc->top=_wtoi(csRet);
	index=str.Find(L',');//查找空格符
	if(index==-1)
	{
		return false;	//说明是最后一个字符串
	}
	csRet=str.Left(index);
	str=str.Right(str.GetLength()-index-1);	
	rc->right=_wtoi(csRet);
	index=str.Find(L' ');//查找空格符
	if(index==-1)
	{
		return false;	//说明是最后一个字符串
	}
	csRet=str.Left(index);
	str=str.Right(str.GetLength()-index-1);	
	rc->bottom=_wtoi(csRet);
	return true;
}

void TCLoadWinRCFile::GetCtrlProporty(TCTOOLTYPE type,CWnd *CWnd,CString &lineBuf)
{
	switch(type)
	{
	case TCTT_TWIN:
		{
			CDiaTCDesign *pTWin=(CDiaTCDesign *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("最小化"))
					pTWin->TCSetMinButton(_wtoi(value));
				else if(name == _T("最大化"))
					pTWin->TCSetMaxButton(_wtoi(value));
				else if(name==_T("背景颜色"))
				{
					if(_wtoi(value)==0x7fffffff)
					{
						pTWin->m_TcDlgConfig.m_BkgrColor=0x0;
					}
					else
					{
						pTWin->m_TcDlgConfig.m_BkgrColor=_wtoi(value);
					}
				}
				else if(name==_T("背景图片"))
					pTWin->TCSetBackgroundImage(value);
			}
			break;
		}
	case TCTT_STATIC:
		{
			CTCStatic *pStatic=(CTCStatic *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("字体"))
				{
					CString font;
					DWORD textclolor;
					DWORD Bgclolor;
					int size;
					int style;
					value.Trim();
					value.Delete(0);
					int index=value.Find(',');
					for(int i=0;i<4;i++)
					{
						value.Trim();//过滤空格符号
						if(i==0)
							font=value.Left(index);
						else if(i==1)
							textclolor=_wtoi(value.Left(index));
						else if(i==2)
						{
							CString csHelp=value.Left(index);
							if(csHelp==_T("FFFFFFFF"))
								Bgclolor=::GetSysColor(COLOR_BTNFACE);
							else
								Bgclolor=_wtoi(csHelp);
						}
						else if(i==3)
							size=_wtoi(value.Left(index));
						value=value.Right(value.GetLength()-index-1);
						index=value.Find(',');
					}
					index=value.Find(')');
					style=_wtoi(value.Left(index));
					pStatic->SetBackground(Bgclolor);
					pStatic->SetFontStatic(font,size,textclolor,style);
				}
			}
			break;
		}
	case TCTT_GROUP:
		{
			CTCGroupBox *pGroup=(CTCGroupBox *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("对齐"))
				{
					pGroup->SetAlignment(_wtoi(value));
				}
				else if(name==_T("边框颜色"))
				{
					pGroup->SetBorderColor(_wtoi(value));
				}
				else if(name==_T("标题背景颜色"))
				{
					pGroup->SetBackgroundColor(_wtoi(value),pGroup->GetClientBackgroundColor());
				}
				else if(name==_T("客户区颜色"))
				{
					pGroup->SetBackgroundColor(pGroup->GetTitleBackgroundColor(),_wtoi(value));
				}
				else if(name==_T("标题颜色"))
				{
					pGroup->SetCatptionTextColor(_wtoi(value));
				}
				else if(name==_T("分组框模式"))
				{
					pGroup->SetTCGroupStyle((CTCGroupBox::TCGroupBoxStyle)_wtoi(value));
				}
				else if(name==_T("字体"))
				{
					LOGFONT lf;
					value.Trim();
					value.Delete(0);
					int index=value.Find(',');
					for(int i=0;i<13;i++)
					{
						value.Trim();//过滤空格符号
						if(i==0)
							lf.lfHeight=_wtoi(value.Left(index));
						else if(i==1)
							lf.lfWidth=_wtoi(value.Left(index));
						else if(i==2)
							lf.lfEscapement=_wtoi(value.Left(index));
						else if(i==3)
							lf.lfOrientation=_wtoi(value.Left(index));
						else if(i==4)
							lf.lfWeight=_wtoi(value.Left(index));
						else if(i==5)
							lf.lfItalic=_wtoi(value.Left(index));
						else if(i==6)
							lf.lfUnderline=_wtoi(value.Left(index));
						else if(i==7)
							lf.lfStrikeOut=_wtoi(value.Left(index));
						else if(i==8)
							lf.lfCharSet=_wtoi(value.Left(index));
						else if(i==9)
							lf.lfOutPrecision=_wtoi(value.Left(index));
						else if(i==10)
							lf.lfClipPrecision=_wtoi(value.Left(index));
						else if(i==11)
							lf.lfQuality=_wtoi(value.Left(index));
						else if(i==12)
							lf.lfPitchAndFamily=_wtoi(value.Left(index));
						value=value.Right(value.GetLength()-index-1);
						index=value.Find(',');
					}
					index=value.Find(')');
					lstrcpyW(lf.lfFaceName,value.Left(index));
					//CFont* lpFont = new CFont();
					//lpFont->CreateFontIndirectW(&lf);
					pGroup->SetFont(lf);
					//delete lpFont;
				}
			}
			break;
		}
	case TCTT_BUTTON:
		{
			CTCButton *pButton=(CTCButton *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("多行"))
				{
					pButton->TCSetButtonMulity(_wtoi(value));
				}
				else if(name==_T("默认按钮"))
				{
					pButton->TCSetDefButton(_wtoi(value));
				}
				else if(name==_T("有效"))
				{
					pButton->TCSetAbled(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_EDIT:
		{
			CTCEdit *pEdit=(CTCEdit *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("有效"))
				{
					pEdit->TCSetAbled(_wtoi(value));
				}
				else if(name==_T("多行"))
				{
					pEdit->TCSetMulityLine(_wtoi(value));
				}
				else if(name==_T("密文"))//匹配特征字符串
				{
					pEdit->TCSetPassWord(_wtoi(value));
				}
				else if(name==_T("数字"))
				{
					pEdit->TCSetNumber(_wtoi(value));
				}
				else if(name==_T("只读"))
				{
					pEdit->TCSetReadOnly(_wtoi(value));
				}
				else if(name==_T("文本颜色"))//匹配特征字符串
				{
					pEdit->SetTextColor(_wtoi(value));
				}
				else if(name==_T("背景颜色"))//匹配特征字符串
				{
					pEdit->SetBackColor(_wtoi(value));
				}
				else if(name==_T("垂直"))
				{
					pEdit->TCSetVertiScroll(_wtoi(value));
				}
				else if(name==_T("水平"))
				{
					pEdit->TCSetHorizScroll(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_RADIO:
		{
			CTCRadio *pRadio=(CTCRadio *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("是否选中"))
				{
					pRadio->TCSetCheck(_wtoi(value));
				}
				else if(name==_T("有效"))
				{
					pRadio->TCSetAbled(_wtoi(value));
				}
				else if(name==_T("标题居左"))
				{
					pRadio->TCSetTextLeft(_wtoi(value));
				}
				else if(name==_T("分组"))
				{
					pRadio->TCSetGroup(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_CHECK:
		{
			CTCCheck *pCheck=(CTCCheck *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("是否选中"))
				{
					pCheck->TCSetCheck(_wtoi(value));
				}
				else if(name==_T("有效"))
				{
					pCheck->TCSetAbled(_wtoi(value));
				}
				else if(name==_T("标题居左"))
				{
					pCheck->TCSetTextLeft(_wtoi(value));
				}
				
			}
			break;
		}
	case TCTT_COMBOBOX:
		{
			CTCComboBox *pCombox=(CTCComboBox *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("有效"))
				{
					pCombox->TCSetAbled(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_LISTBOX:
		{
			CTCListBox *pListbox=(CTCListBox *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("有效"))
				{
					pListbox->TCSetAbled(_wtoi(value));
				}
				else if(name==_T("默认滚动"))
				{
					pListbox->TCSetDefaultRool(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_PROGRESS:
		{
			CTCProgress *pProgress=(CTCProgress *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("垂直"))
				{
					pProgress->TCSetVertical(_wtoi(value));
				}
				else if(name==_T("有效"))
				{
				   pProgress->TCSetAbled(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_DATE:
		{
			CTCDate* pDate=(CTCDate *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("有效"))
				{
					pDate->TCSetAbled(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_SLIDER:
		{
			CTCSlider *pSlider=(CTCSlider *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("垂直"))
				{
					pSlider->TCSetVertical(_wtoi(value));
				}
				else if(name==_T("有效"))
				{
					pSlider->TCSetAbled(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_PICTURE:
		{
			CTCPicture *pPic=(CTCPicture *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("图片名称"))
				{
					pPic->SetRCPicture(value);
				}
			}
			break;
		}
	case TCTT_PATH:
		{
			CTCBrowse *pPath=(CTCBrowse *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("浏览模式"))
				{
					int mode=_wtoi(value);
					CMFCEditBrowseCtrl::BrowseMode Mode=(CMFCEditBrowseCtrl::BrowseMode)mode;
					if(Mode == CMFCEditBrowseCtrl::BrowseMode::BrowseMode_File)
						pPath->TCSetMode(CMFCEditBrowseCtrl::BrowseMode::BrowseMode_File);
					else
						pPath->TCSetMode(CMFCEditBrowseCtrl::BrowseMode::BrowseMode_Folder);
				}
				else if(name==_T("有效"))
				{
					pPath->TCSetAbled(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_TIMER:
		{
			CTCTimer *pTimer=(CTCTimer *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("频率"))
				{
					pTimer->SetTimer(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_HOTKEY:
		{
			CTCHotKeyCtrl* pHotKey=(CTCHotKeyCtrl *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				
				if(name==_T("虚拟键值"))
				{
					pHotKey->m_wVirtualKeyCode=_wtoi(value);
				}
				else if(name==_T("模态值"))
				{
					pHotKey->m_wModifiers=_wtoi(value);
				}
				else if(name==_T("有效"))
				{
					pHotKey->TCSetAbled(_wtoi(value));
				}
			}
			break;
		}
	case TCTT_EXPLORER:
		{
			CTCExplorer* pExplorer=(CTCExplorer *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("URL"))
				{
					pExplorer->TCSetURL(value);
				}
			}
			break;
		}
	case TCTT_GRIDCTRL:
		{
			CGridCtrl *pGrid=(CGridCtrl *)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("行数"))
				{
					pGrid->SetRowCount(_wtoi(value));
				}
				else if(name==_T("列数"))
				{
					pGrid->SetColumnCount(_wtoi(value));
				}
				else if(name==_T("固定行数"))
				{
					pGrid->SetFixedRowCount(_wtoi(value));
				}
				else if(name==_T("固定列数"))
				{
					pGrid->SetFixedColumnCount(_wtoi(value));
				}
				else if(name==_T("文本颜色"))
				{
					pGrid->GetDefaultCell(FALSE,FALSE)->SetTextClr(_wtoi(value));
				}
				else if(name==_T("背景颜色"))
				{
					pGrid->GetDefaultCell(FALSE,FALSE)->SetBackClr(_wtoi(value));
				}
				else if(name==_T("固定项文本颜色"))
				{
					pGrid->SetFixedTextColor(_wtoi(value));
				}
				else if(name==_T("固定项背景颜色"))
				{
					pGrid->SetFixedBkColor(_wtoi(value));
				}
				else if(name==_T("水平网格"))
				{
					if(_wtoi(value) && pGrid->IsShowVertiLines())
						pGrid->SetGridLines(GVL_BOTH);
					else if(_wtoi(value) && !pGrid->IsShowVertiLines())
						pGrid->SetGridLines(GVL_HORZ);
					else if(!_wtoi(value) && pGrid->IsShowVertiLines())
						pGrid->SetGridLines(GVL_VERT);
					else 
						pGrid->SetGridLines(GVL_NONE);
				}
				else if(name==_T("垂直网格"))
				{
					if(_wtoi(value) && pGrid->IsShowHorizLines())
						pGrid->SetGridLines(GVL_BOTH);
					else if(_wtoi(value) && !pGrid->IsShowHorizLines())
						pGrid->SetGridLines(GVL_VERT);
					else if(!_wtoi(value) && pGrid->IsShowHorizLines())
						pGrid->SetGridLines(GVL_HORZ);
					else
						pGrid->SetGridLines(GVL_NONE);
				}
				else if(name == _T("可编辑"))
				{
					pGrid->TCSetAllSellsEditAbled(_wtoi(value));
				}
				else if(name == _T("排序"))
				{
					pGrid->SetHeaderSort(_wtoi(value));
				}
				else if(name == _T("填充"))
				{
					if(_wtoi(value))
						pGrid->ExpandToFit(_wtoi(value));
					else
					{
						for(int row = 0;row < pGrid->GetRowCount(); row++)
						{
							pGrid->SetRowHeight(row,25);
						}
					}				
				}
				else if(name == _T("选中"))
				{
					pGrid->EnableSelection(_wtoi(value));
				}
				else if(name == _T("固定行选中"))
				{
					pGrid->SetFixedRowSelection(_wtoi(value));
				}
				else if(name == _T("固定列选中"))
				{
					pGrid->SetFixedColumnSelection(_wtoi(value));
				}
				else if(name == _T("列表模式"))
				{
					pGrid->SetListMode(_wtoi(value));
				}
				else if(name==_T("字体"))
				{
					LOGFONT lf;
					value.Trim();
					value.Delete(0);
					int index=value.Find(',');
					for(int i=0;i<13;i++)
					{
						value.Trim();//过滤空格符号
						if(i==0)
							lf.lfHeight=_wtoi(value.Left(index));
						else if(i==1)
							lf.lfWidth=_wtoi(value.Left(index));
						else if(i==2)
							lf.lfEscapement=_wtoi(value.Left(index));
						else if(i==3)
							lf.lfOrientation=_wtoi(value.Left(index));
						else if(i==4)
							lf.lfWeight=_wtoi(value.Left(index));
						else if(i==5)
							lf.lfItalic=_wtoi(value.Left(index));
						else if(i==6)
							lf.lfUnderline=_wtoi(value.Left(index));
						else if(i==7)
							lf.lfStrikeOut=_wtoi(value.Left(index));
						else if(i==8)
							lf.lfCharSet=_wtoi(value.Left(index));
						else if(i==9)
							lf.lfOutPrecision=_wtoi(value.Left(index));
						else if(i==10)
							lf.lfClipPrecision=_wtoi(value.Left(index));
						else if(i==11)
							lf.lfQuality=_wtoi(value.Left(index));
						else if(i==12)
							lf.lfPitchAndFamily=_wtoi(value.Left(index));
						value=value.Right(value.GetLength()-index-1);
						index=value.Find(',');
					}
					index=value.Find(')');
					lstrcpyW(lf.lfFaceName,value.Left(index));
					CFont* lpFont = new CFont();
					lpFont->CreateFontIndirectW(&lf);
					pGrid->SetFont(lpFont);
					pGrid->AutoSize();
					delete lpFont;
				}
				else if(name==_T("列属性"))
				{
					for(int col=0;col<pGrid->GetColumnCount();col++)
					{
						CString rowPro;
						if(!GetGridRowProperty(value,rowPro))
							break;
						CString rName,rValue;
						while(GetGridRowNextProperty(rowPro,rName,rValue))
						{
							if(rName==_T("列名"))
							{
								rValue.Trim();
								rValue.Delete(0);
								rValue.Delete(rValue.GetLength()-1);
								pGrid->SetItemText(0,col,rValue);
							}
							else if(rName==_T("列宽"))
							{
								pGrid->SetColumnWidth(col,_wtoi(rValue));
							}
							else if(rName==_T("类型"))
							{
								for(int row = 0;row < pGrid->GetRowCount(); row++)
								{
									switch(_wtoi(rValue))
									{
									case TCGRID_CTRL_NOMAL:
										pGrid->SetCellType(row,col,RUNTIME_CLASS(CGridCell));
										pGrid->SetItemData(0,col,TCGRID_CTRL_NOMAL);
										break;
									case TCGRID_CTRL_COMBOX:
										if(!pGrid->IsCellFixed(row,col))
										{		
											if(!pGrid->TCGetColReadInfo(0,col))
												pGrid->SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));
										}
										pGrid->SetItemData(row,col,TCGRID_CTRL_COMBOX);
										break;
									case TCGRID_CTRL_READONLY:
										if(!pGrid->IsCellFixed(row,col))
										{
											pGrid->SetCellType(row,col, RUNTIME_CLASS(CGridCell));
											pGrid->SetItemState(row,col,pGrid->GetItemState(row,col)|GVIS_READONLY);
										}	
										pGrid->SetItemData(row,col,TCGRID_CTRL_READONLY);
										break;
									case TCGRID_CTRL_CHECK:
										if(!pGrid->IsCellFixed(row,col))
										{
											pGrid->SetCellType(row,col, RUNTIME_CLASS(CGridCellCheck));							
										}
										pGrid->SetItemData(0,col,TCGRID_CTRL_CHECK);
										break;
									case TCGRID_CTRL_NUM:
										if(!pGrid->IsCellFixed(row,col))
										{
											pGrid->SetCellType(row,col, RUNTIME_CLASS(CGridCellNumeric));
										}
										pGrid->SetItemData(row,col,TCGRID_CTRL_NUM);
										break;
									case TCGRID_CTRL_DATE:
										if(!pGrid->IsCellFixed(row,col))
										{
											pGrid->SetCellType(row,col, RUNTIME_CLASS(CGridCellDateTime));
											CGridCellDateTime* pCell = (CGridCellDateTime*)pGrid->GetCell(row,col);
											pCell->SetTime(CTime::GetCurrentTime());
											pGrid->RedrawCell(row,col);
										}
										pGrid->SetItemData(row,col,TCGRID_CTRL_DATE);
										break;
									/*case TCGRID_CTRL_URL:
										if(!pGrid->IsCellFixed(row,col))
										{
											pGrid->SetCellType(row,col, RUNTIME_CLASS(CGridURLCell));
											pGrid->SetItemText(row,col,_T("www.tyuyan.com"));
										}
										pGrid->SetItemData(row,col,TCGRID_CTRL_URL);
										break;*/
									default:
										break;
									}
									
								}	
							}
							else if(rName==_T("只读"))
							{	
								pGrid->TCSetColReadInfo(0,col,_wtoi(rValue));
							}
						}

					}

				}
			}
			break;
		}
	case TCTT_TAB:
		{
			CTCTab* pTab=(CTCTab*)CWnd;
			CString name,value;
			while(GetNextProporty(lineBuf,name,value))
			{
				if(name==_T("有效"))
				{
					pTab->TCSetAbled(_wtoi(value));
				}
				else if(name==_T("页数"))
				{
					//暂时页数没用
				}
			}
			break;
		}
	}
}

bool TCLoadWinRCFile::GetNextProporty(CString &lineBuf,CString &name,CString &value)
{
	wchar_t *pStr=lineBuf.LockBuffer();//temphelp;
	if(*pStr==L' '||*pStr==NULL)
		return false;
	if(*pStr==L'|')
	{
		lineBuf.Delete(0);
		return false;
	}
	int index=0;
	while((*pStr)&&(*pStr!=L':'))
	{
		pStr++;
		index++;
	}
	if(*pStr)
	{
		name=lineBuf.Left(index);
		lineBuf=lineBuf.Right(lineBuf.GetLength()-index-1);	
	}
	else
		return false;
	index=0;
	pStr=lineBuf.LockBuffer();//temphelp;
	while(*pStr!=L'|')
	{
		if(*pStr=='\"')
		{
			do
			{
				if(*pStr=='\\')
				{
					if(*(pStr+1)=='\"'||*(pStr+1)=='\\')//碰到转义符的时候！
					{
						pStr+=2;//跳过转义符号！
						index+=2;
					}
					else
						throw;
				}
				else
				{
					pStr++;
					index++;
				}

			}
			while((*pStr)!='\"');
		}
		pStr++;
		index++;
	}
	if(*pStr)
	{
		value=lineBuf.Left(index);
		lineBuf=lineBuf.Right(lineBuf.GetLength()-index-1);	
	}
	return true;
}
//
bool TCLoadWinRCFile::GetCtrlEvent(TCTOOLTYPE type,CWnd *CWnd,CString &lineBuf)
{
	switch(type)
	{
	case TCTT_TWIN:
		{
			CDiaTCDesign *pTWin=(CDiaTCDesign *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("窗口初始化"))
					pTWin->m_TcDlgConfig.m_eDiaInit=value;
				else if(name==_T("窗口销毁"))
					pTWin->m_TcDlgConfig.m_eDestory=value;
				else if(name==_T("窗口关闭"))
					pTWin->m_TcDlgConfig.m_eClose=value;
				else if(name==_T("消息路由"))
					pTWin->m_TcDlgConfig.m_ePreTrans=value;
				else if(name==_T("消息过程"))
					pTWin->m_TcDlgConfig.m_eWinProc=value;
			}
			break;
		}
	case TCTT_STATIC:
		{
			CTCStatic *pStatic=(CTCStatic *)CWnd;
			CString name,value;
			GetNextEvent(lineBuf,name,value);
			if(name==_T("鼠标单击"))
			{
				pStatic->TCSetClickEvent(value);
			}
			break;
		}
	case TCTT_GROUP:
		{
			CTCGroupBox *pGroup=(CTCGroupBox *)CWnd;
			CString name,value;
			GetNextEvent(lineBuf,name,value);
			//预留
			break;
		}
	case TCTT_BUTTON:
		{
			CTCButton *pbutton=(CTCButton*)CWnd;
			CString name,value;
			GetNextEvent(lineBuf,name,value);
			if(name==_T("鼠标单击"))
			{
				pbutton->TCSetClickEvent(value);
			}
			break;
		}
	case TCTT_EDIT:
		{
			CTCEdit *pEdit=(CTCEdit *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("内容改变"))//匹配特征字符串
				{
					pEdit->TCSetEnChangeEvent(value);
				}
				else if(name==_T("获得焦点"))//匹配特征字符串
				{
					pEdit->TCSetFocusEvent(value);
				}
				else if(name==_T("失去焦点"))//匹配特征字符串
				{
					pEdit->TCSetKillfocusEvent(value);
				}
			}
			break;
	}
	case TCTT_RADIO:
		{
			CTCRadio *pRadio=(CTCRadio *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("左键单击"))
				{
					pRadio->TCSetRadioEvent(value);
				}
			}
			break;
		}
	case TCTT_CHECK:
		{
			CTCCheck *pCheck=(CTCCheck *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("左键单击"))
				{
					pCheck->TCSetCheckEvent(value);
				}
			}
			break;
		}
	case TCTT_COMBOBOX:
		{
			CTCComboBox *pCombox=(CTCComboBox *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("左键单击"))
				{
					pCombox->SetLeftDownEvent(value);
				}
				else if(name==_T("选择改变"))
				{
					pCombox->SetEnChangeEvent(value);
				}
			}
			break;
		}
	case TCTT_LISTBOX:
		{
			CTCListBox *pListbox=(CTCListBox *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("左键双击"))
				{
					pListbox->TCSetListBoxDoubleClick(value);
				}
				else if(name==_T("选择改变"))
				{
					pListbox->TCSetListBoxSelChange(value);
				}
			}
			break;
		}
	case TCTT_PROGRESS:
		{
			CTCProgress *pProgress=(CTCProgress *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				//预留
			}
			break;
		}
	case TCTT_DATE:
		{
			CTCDate* pDate=(CTCDate *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				//预留
			}
			break;
		}
	case TCTT_SLIDER:
		{
			CTCSlider *pSlider=(CTCSlider *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("位置改变"))
				{
					pSlider->TCSetSliderChangeEvent(value);
				}
			}
			break;
		}
	case TCTT_PICTURE:
		{
			CTCPicture *pPic=(CTCPicture *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("左键单击"))
				{
					pPic->TCSetClickEvent(value);
				}
			}
			break;
		}
	case TCTT_PATH:
		{
			CTCBrowse *pPath=(CTCBrowse *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("选择改变"))
				{
					pPath->TCSetEnChangeEvent(value);
				}
			}
			break;
		}
	case TCTT_TIMER:
		{
			CTCTimer *pTimer=(CTCTimer *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("时钟"))
				{
					pTimer->TCSetTimeEvent(value);
				}
			}
			break;
		}
	case TCTT_HOTKEY:
		{
			CTCHotKeyCtrl* pHotKey=(CTCHotKeyCtrl *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("热键"))
				{
					pHotKey->TCSetHotKeyEvent(value);
				}
				else if(name==_T("失去焦点"))
				{
					pHotKey->TCSetKillFocusEvent(value);
				}
			}
			break;
		}
	case TCTT_EXPLORER:
		{
			CTCExplorer* pExplorer=(CTCExplorer *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("加载完成"))
				{
					pExplorer->TCSetloadComplete(value);
				}
			}
			break;
		}
	case TCTT_TAB:
		{
			CTCTab* pTab=(CTCTab *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("选择改变"))
				{
					pTab->TCSetSelChange(value);
				}
			}
			break;
		}
	case TCTT_GRIDCTRL:
		{
			CGridCtrl* pGridCtrl=(CGridCtrl *)CWnd;
			CString name,value;
			while(GetNextEvent(lineBuf,name,value))
			{
				if(name==_T("左键单击"))
				{
					pGridCtrl->TCSetClickEvent(value);
				}
				else if(name == _T("编辑开始"))
				{
					pGridCtrl->TCSetBeforeEditEvent(value);
				}
				else if(name == _T("编辑结束"))
				{
					pGridCtrl->TCSetAfterEditEvent(value);
				}
				else if(name == _T("点击排序"))
				{
					pGridCtrl->TCSetHeadSortEvent(value);
				}
			}
			break;
		}
	}
	return true;
}
//
bool TCLoadWinRCFile::GetNextEvent(CString &lineBuf,CString &name,CString &value)
{
	lineBuf.Trim();
	int index=lineBuf.Find(L':');//查找空格符
	if(index==-1)
	{
		return false;	//说明是最后一个字符串
	}
	name=lineBuf.Left(index);
	lineBuf=lineBuf.Right(lineBuf.GetLength()-index-1);	
	index=lineBuf.Find(L'|');//查找空格符
	if(index==-1)
	{
		return false;	//说明是最后一个字符串
	}
	value=lineBuf.Left(index);
	lineBuf=lineBuf.Right(lineBuf.GetLength()-index-1);	
	return true;
}

//表格属性专用方法
bool TCLoadWinRCFile::GetGridRowProperty(CString &lineBuf,CString &rowPro)
{
	int index=0;
	wchar_t *pointstr=lineBuf.LockBuffer();//temphelp;
	while(*pointstr!=L';')
	{
		if(*pointstr=='\"')
		{
			do
			{
				if(*pointstr=='\\')
				{
					if(*(pointstr+1)=='\"'||*(pointstr+1)=='\\')//碰到转义符的时候！
					{
						pointstr+=2;//跳过转义符号！
						index+=2;
					}
				}
				else
				{
					pointstr++;
					index++;
				}

			}
			while((*pointstr)!='\"');
		}
		pointstr++;
		index++;
	}

	rowPro=lineBuf.Left(index);
	lineBuf=lineBuf.Right(lineBuf.GetLength()-index-1);	
	return rowPro.GetLength()!=0;

}

//表格一个属性
bool TCLoadWinRCFile::GetGridRowNextProperty(CString &lineBuf,CString &name,CString &value)
{
	wchar_t *pStr=lineBuf.LockBuffer();//temphelp;

	if(!(*pStr))
		return false;

	int index=0;
	while(*pStr!=L':')
	{
		if(*pStr=='\"')
		{
			do
			{
				if(*pStr=='\\')
				{
					if(*(pStr+1)=='\"'||*(pStr+1)=='\\')//碰到转义符的时候！
					{
						pStr+=2;//跳过转义符号！
						index+=2;
					}
					else
						throw;
				}
				else
				{
					pStr++;
					index++;
				}

			}
			while((*pStr)!='\"');
		}
		pStr++;
		index++;
	}
	if(*pStr)
	{
		name=lineBuf.Left(index);
		lineBuf=lineBuf.Right(lineBuf.GetLength()-index-1);	
	}
	
	index=0;
	pStr=lineBuf.LockBuffer();//temphelp;
	while(*pStr&&*pStr!=L',')
	{
		if(*pStr=='\"')
		{
			do
			{
				if(*pStr=='\\')
				{
					if(*(pStr+1)=='\"'||*(pStr+1)=='\\')//碰到转义符的时候！
					{
						pStr+=2;//跳过转义符号！
						index+=2;
					}
					else
						throw;
				}
				else
				{
					pStr++;
					index++;
				}

			}
			while((*pStr)!='\"');
		}
		pStr++;
		index++;
	}

	value=lineBuf.Left(index);
	lineBuf=lineBuf.Right(lineBuf.GetLength()-index-1);	
	

	return value.GetLength()!=0;
}

//处理下拉框和列表框用户自定义的字符串数据
bool TCLoadWinRCFile::GetStrValue(CString &lineBuf,CStringArray &strarry,bool addEmpty)
{
	int n;
	CString tempcsstr,csHelpText;
	lineBuf.Trim();
	wchar_t temphelp[MAX_PATH]={0};
	wchar_t *pointstr=lineBuf.LockBuffer();//temphelp;
	int i=0,size=0;
	if(*pointstr=='\"')
	{
		do
		{
			if(*pointstr=='\\')
			{
				if(*(pointstr+1)=='\"'||*(pointstr+1)=='\\'||*(pointstr+1)=='\|')//碰到转义符的时候！
				{
					temphelp[i]=*(pointstr+1);
					pointstr+=2;//跳过转义符号！
					i++;
					size+=2;
				}
			}
			else
			{
				temphelp[i]=*pointstr;
				*pointstr++;
				i++;
				size++;
			}
			if(*pointstr=='\|')//遇到|线分隔符号时候截断
			{
				csHelpText.Format(_T("%s"),&temphelp[1]);
				if(!addEmpty)
				{
					if(!csHelpText.IsEmpty())
						strarry.Add(csHelpText);
				}
				else
					strarry.Add(csHelpText);
				i=0;
				memset(temphelp,0,sizeof(temphelp));
			}
		}
		while((*pointstr)!='\"');
	}

	if(size!=0)
		n=size+1;

	lineBuf=lineBuf.Right(lineBuf.GetLength()-size-1);

	return true;
}

 TCHAR *TCLoadWinRCFile::TCGetBuffer()
 {
	 return m_pBuffer;
 }