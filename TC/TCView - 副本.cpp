// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// TCView.cpp : CTCView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TC.h"
#endif

#include "MainFrm.h"
#include "TCDoc.h"
#include "TCView.h"
#include "TCRunTimeManager.h"

//////////////////////////�༭�����ͷ/////////////////////////
#include "..\scintilla\include\scintilla.h"
#include "..\scintilla\include\scilexer.h"

#include "DiaTViewFind.h"

#define SC_MARKNUM_FOLDEREND 25  //�۵�״̬���༶�м䣩
#define SC_MARKNUM_FOLDEROPENMID 26  //չ��״̬���༶�м䣩
#define SC_MARKNUM_FOLDERMIDTAIL 27  //���۵������β�����༶�м䣩
#define SC_MARKNUM_FOLDERTAIL 28  //���۵������β��
#define SC_MARKNUM_FOLDERSUB 29   //���۵��Ĵ����
#define SC_MARKNUM_FOLDER 30     //�۵�״̬
#define SC_MARKNUM_FOLDEROPEN 31 //չ��״̬

//��Щ��ǵ�������0xFE000000��ͬ��ͷ�ļ����Ѿ�������ˣ�
#define SC_MASK_FOLDERS 0xFE000000
#define MARGIN_FOLD_INDEX 2

//////////////////////////�༭�����ͷ/////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTCView

IMPLEMENT_DYNCREATE(CTCView, CView)

	BEGIN_MESSAGE_MAP(CTCView, CView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_UPDATE_COMMAND_UI(ID_TC_NEW_DEL_BP, &CTCView::OnUpdateTcNewDelBp)
		ON_UPDATE_COMMAND_UI(ID_TC_DELETEALLBP, &CTCView::OnUpdateTcDeleteallbp)
		ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CTCView::OnUpdateEditUndo)
		ON_COMMAND(ID_EDIT_UNDO, &CTCView::OnEditUndo)
		ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CTCView::OnUpdateEditRedo)
		ON_COMMAND(ID_EDIT_REDO, &CTCView::OnEditRedo)
		ON_COMMAND(ID_EDIT_CUT, &CTCView::OnEditCut)
		ON_COMMAND(ID_EDIT_COPY, &CTCView::OnEditCopy)
		ON_COMMAND(ID_EDIT_PASTE, &CTCView::OnEditPaste)
		ON_COMMAND(ID_EDIT_DELETE, &CTCView::OnEditDelete)
		ON_COMMAND(ID_EDIT_SELECT_ALL, &CTCView::OnEditSelectAll)
		ON_COMMAND(ID_TC_NEW_DEL_BP, &CTCView::OnTcNewDelBp)
		ON_COMMAND(ID_TC_DELETEALLBP, &CTCView::OnTcDeleteallbp)
		ON_COMMAND(ID_EDIT_ADDCOMMENT, &CTCView::OnEditAddcomment)
		ON_UPDATE_COMMAND_UI(ID_EDIT_ADDCOMMENT, &CTCView::OnUpdateEditAddcomment)
		ON_COMMAND(ID_EDIT_REMOVECOMMENT, &CTCView::OnEditRemovecomment)
		ON_UPDATE_COMMAND_UI(ID_EDIT_REMOVECOMMENT, &CTCView::OnUpdateEditRemovecomment)
		ON_COMMAND(ID_EDIT_FIND, &CTCView::OnEditFind)
		ON_WM_DESTROY()
		ON_WM_SHOWWINDOW()
		ON_COMMAND(ID_TC_TYPESETTING, &CTCView::OnTcTypesetting)
		ON_COMMAND(ID_EDIT_GOBACK, &CTCView::OnEditGoback)
		ON_UPDATE_COMMAND_UI(ID_EDIT_GOBACK, &CTCView::OnUpdateEditGoback)
		ON_COMMAND(ID_EDIT_GOFORWARD, &CTCView::OnEditGoforward)
		ON_UPDATE_COMMAND_UI(ID_EDIT_GOFORWARD, &CTCView::OnUpdateEditGoforward)
		ON_UPDATE_COMMAND_UI(ID_TC_TYPESETTING, &CTCView::OnUpdateTcTypesetting)
		ON_COMMAND(ID_EDIT_TOGGLEFOLDALL, &CTCView::OnEditTogglefoldall)
	END_MESSAGE_MAP()

	// CTCView ����/����

	CTCView::CTCView()
	{
		// TODO: �ڴ˴���ӹ������
		//m_isFirst=false;
		notFirst=false;
		m_rowCount=0;
		m_allSize=0;
		m_isFold=true;
	}

	CTCView::~CTCView()
	{
	}

	BOOL CTCView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: �ڴ˴�ͨ���޸�
		//  CREATESTRUCT cs ���޸Ĵ��������ʽ

		//���ñ༭��������
		//cs.lpszClass = _T("Scintilla");

		return CView::PreCreateWindow(cs);
	}

	// CTCView ����

	void CTCView::OnDraw(CDC* /*pDC*/)
	{
		CTCDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	}

	void CTCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CTCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CTCView ���

#ifdef _DEBUG
	void CTCView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CTCView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CTCDoc* CTCView::GetDocument() const // �ǵ��԰汾��������
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTCDoc)));
		return (CTCDoc*)m_pDocument;
	}
#endif //_DEBUG

	//�ָ��ִ�
	void my_split(TCHAR* sourcestr, TCHAR *oldstr, std::wstring *temp, int &rlen);
	void my_split(TCHAR* sourcestr, TCHAR *oldstr, std::wstring *temp, int &rlen)
	{
		TCHAR sourcebak[1024]={0};//���ı�Դ������һ��
		wcscpy(sourcebak,sourcestr);
		TCHAR *pch=wcstok(sourcebak,oldstr);
		rlen=0;
		while(pch!=NULL)
		{
			temp[rlen]=pch;
			pch=wcstok(NULL,oldstr);
			rlen++;
		}
	}

	// CTCView ��Ϣ�������
	int CTCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  �ڴ������ר�õĴ�������
		if (!m_ScintillaWnd.Create(_T("TC"), WS_EX_CLIENTEDGE|WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 10000))
		{
			AfxMessageBox(L"can't create scintilla!");
			return -1;
		}

		InitialiseEditor();
		UpdateLineNumberWidth();

		m_listView.Create(&m_ScintillaWnd,5325388,CRect(10,10,180,180),WS_CHILD|WS_BORDER|LVS_SMALLICON/*|LVS_EDITLABELS*/|WS_VSCROLL|LVS_NOLABELWRAP|LVS_SINGLESEL); 
		m_listView.setImageList();
		m_listView.m_pView = this;

		return 0;
	}

	void CTCView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: �ڴ˴������Ϣ����������
		if (m_ScintillaWnd.GetSafeHwnd())
			m_ScintillaWnd.MoveWindow(0, 0, cx, cy);
	}

	void CTCView::InitialiseEditor(void)
	{
		const wchar_t* tcKeywords=
			_T("int double string bool false true ���� ������ �ַ��� �߼��� �� �� ")
			_T("namespace endnamespace function endfunction if else endif while endwhile repeat endrepeat select endselect case default return continue break \
			   �ռ� �ռ���� ���� ���ܽ��� ��� ���� ������� ѭ�� ѭ������ �ظ� �ظ����� ѡ�� ���� Ĭ�� ѡ����� ���� ���� ���� ");

		//���õ�һ����ʾ�����Ǵ����к�
		SendEditor(SCI_SETMARGINTYPEN,0, SC_MARGIN_NUMBER);   
		SendEditor(SCI_SETMARGINWIDTHN,0, 42);  

		//0��ҳ�ߣ����Ϊ9����ʾ0�ű��(0..0001B)   
		//���ñ�ǵ�ǰ��ɫ   
		SendEditor(SCI_SETMARGINTYPEN,1,SC_MARGIN_SYMBOL);   
		SendEditor(SCI_SETMARGINSENSITIVEN, 1, TRUE); //��Ӧ�����Ϣ  //SendEditor(SCI_SETMARGINWIDTHN,0, 9);   
		//SendEditor(SCI_SETMARGINMASKN,0, 0x01);  
		SendEditor(SCI_MARKERDEFINE,1,SC_MARK_CIRCLE);
		SendEditor(SCI_MARKERSETFORE,1,0x0000ff);//0-��ɫ  
		SendEditor(SCI_MARKERSETBACK,1,0x00ffff);//0-��ɫ   

		SendEditor(SCI_MARKERDEFINE,2,SC_MARK_ARROW);
		SendEditor(SCI_MARKERSETFORE,2,0x00ffff);//0-��ɫ  
		SendEditor(SCI_MARKERSETBACK,2,0x0000ff);//0-��ɫ   

		//����ȫ�ַ�� 
		SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT,(sptr_t)L"Verdana"); 
		SendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT,10); 
		SendEditor(SCI_STYLECLEARALL); 

		SendEditor(SCI_SETLEXER, SCLEX_AU3); 
		SendEditor(SCI_SETKEYWORDS, 0, (sptr_t)tcKeywords);

		//�������ø����﷨Ԫ��ǰ��ɫ
		SendEditor(SCI_STYLESETFORE, SCE_AU3_KEYWORD, 0x00FF0000);         //�ؼ���
		SendEditor(SCI_STYLESETFORE, SCE_AU3_STRING, 0x001515A3);          //�ַ���
		//SendEditor(SCI_STYLESETFORE, SCE_AU3_CHARACTER, 0x001515A3);     //�ַ�
		SendEditor(SCI_STYLESETFORE, SCE_AU3_NUMBER, 0x008B8600);          //����

		//SendEditor(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x00808080);    //Ԥ���뿪��
		SendEditor(SCI_STYLESETFORE, SCE_AU3_COMMENT, 0x00008000);         //��ע��
		//SendEditor(SCI_STYLESETFORE, SCE_AU3_COMMENTLINE, 0x00008000);   //��ע��
		//SendEditor(SCI_STYLESETFORE, SCE_AU3_COMMENTDOC, 0x00008000);    //�ĵ�ע�ͣ�/**��ͷ��
		//���ϵ�ǰ�и�������
		SendEditor(SCI_SETCARETLINEVISIBLE, TRUE);
		SendEditor(SCI_SETCARETLINEBACK, 0xd0ffff);

		//�۵�����
		SendEditor(SCI_SETPROPERTY,(sptr_t)L"fold",(sptr_t)L"1"); 
		SendEditor(SCI_SETPROPERTY,(sptr_t)L"fold.compact",(sptr_t)L"1"); 
		SendEditor(SCI_SETPROPERTY,(sptr_t)L"fold.html",(sptr_t)L"1"); 
		SendEditor(SCI_SETPROPERTY,(sptr_t)L"fold.comment",(sptr_t)L"2"); 
		SendEditor(SCI_SETPROPERTY,(sptr_t)L"fold.preprocessor",(sptr_t)L"1"); 

		SendEditor(SCI_SETMARGINTYPEN, MARGIN_FOLD_INDEX, SC_MARGIN_SYMBOL);//ҳ������ 
		SendEditor(SCI_SETMARGINMASKN, MARGIN_FOLD_INDEX, SC_MASK_FOLDERS); //ҳ������ 
		SendEditor(SCI_SETMARGINWIDTHN, MARGIN_FOLD_INDEX, 13);             //ҳ�߿�� 
		SendEditor(SCI_SETMARGINSENSITIVEN, MARGIN_FOLD_INDEX, TRUE);       //��Ӧ�����Ϣ 

		// �۵���ǩ��ʽ 
		SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);  
		SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);  
		SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND,  SC_MARK_BOXPLUSCONNECTED); 
		SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED); 
		SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE); 
		SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);  
		SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE); 

		// �۵���ǩ��ɫ 
		SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0x00A5A5A5); 
		SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0x00A5A5A5); 
		SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0x00A5A5A5);
		SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDER, 0x00A5A5A5);
		SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEREND, 0x00A5A5A5);     
		SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPEN, 0x00A5A5A5); 
		SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPENMID, 0x00A5A5A5); 
		SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERMIDTAIL, 0x00A5A5A5);

		SendEditor(SCI_SETFOLDFLAGS, 16/*|4*/, 0); //����۵������۵��е����¸���һ������ 
	}

	void CTCView::UpdateLineNumberWidth()
	{
		wchar_t tchLines[6];      
		wsprintf(tchLines,L" %i", m_ScintillaWnd.SendMessage( SCI_GETLINECOUNT,0,0));    
		m_ScintillaWnd.SendMessage(SCI_TEXTWIDTH,STYLE_LINENUMBER,(LPARAM)tchLines);
		m_ScintillaWnd.SendMessage(SCI_STYLESETFORE,STYLE_LINENUMBER,(LPARAM)0x008B8600);
	}

	void CTCView::TCSetFocus()
	{
		SendEditor(SCI_GRABFOCUS);
	}

	//��ת��ָ��λ��
	void CTCView::TCGotoPos(int pos)
	{
		m_ScintillaWnd.SendMessage(SCI_GOTOPOS,pos,(LPARAM)NULL);
	}

	//��ת��ָ����
	void CTCView::TCGotoLine(int line)
	{
		m_ScintillaWnd.SendMessage(SCI_GOTOLINE,line,(LPARAM)NULL);
	}

	//�õ�ָ���е�λ��
	int CTCView::TCLineIndex(int line)
	{
		return SendEditor(SCI_POSITIONFROMLINE,line);
	}

	//ָ��λ�ò�������
	void CTCView::TCInserText(int startPos,int endPos,const TCHAR* text)
	{
		SendEditor(SCI_SETSEL,startPos,endPos);
		SendEditor(SCI_REPLACESEL,NULL,(LPARAM)text);
	}

	//�õ��༭���ݵĳ���
	//
	int CTCView::TCGetLength()
	{
		return SendEditor(SCI_GETLENGTH,0,0);
	}

	void CTCView::TCDrawCurrentBP(int line)
	{
		if(line==0)
			return;
		SendEditor(SCI_MARKERDELETEALL,2,NULL);
		SendEditor(SCI_MARKERADD, line-1/*�����1����Ϊ �������к���1��ʼ,�༭������0��ʼ*/, 2);   
		DisplayLine(line);
	}

	bool CTCView::TCGetFunList(CString namespDot,CString &csList)
	{
		csList.Empty();
		namespDot.Trim();
		if(namespDot.Right(1)!=_T("."))
			return false;

		bool hasData=false;

		namespDot.Delete(namespDot.GetLength()-1);

		const MyApiDllList* mydlllist=MyRuntimeManager::fApiFrame->getList();

		const MyApiDll* helpdll=mydlllist->getHeaddllNode();

		while(helpdll)
		{
			const MyApiNamespaceList* helpnameSL;
			helpnameSL=helpdll->getNamespaceList();

			const MyApiNamespace* manSpace;
			manSpace=helpnameSL->getHeadNode();
			while(manSpace)   //  ����namespacelist
			{
				const MyApiFunctionList* mflist;			
				mflist=manSpace->getFunctionList();
				const MyApiFunction* mF;
				mF=mflist->getHeadNode();

				const XMLCh* nameStr=manSpace->getNameSpacename();

				if(XMLString::compareString(namespDot,nameStr)==0)
				{
					while(mF)
					{  
						const XMLCh* helpfunc=mF->getFuncionName();
						CString csFun=helpfunc;
						csList+=csFun;
						csList+=_T(" ");
						mF=mF->getNextNode();
					}
					hasData=true;
				}
				manSpace=manSpace->getNextNode();
			}
			helpdll=helpdll->getNextNode();
		}
		return hasData;
	}

	int CTCView::TCGetCurrentPos()
	{
		return  m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS);
	}

	void CTCView::TCDeleteDebugBP()
	{
		m_ScintillaWnd.SendMessage(SCI_MARKERDELETEALL,2);
	}

	bool CTCView::TCGetFunParam(CString csNameSPDot,CString csFun,CString &csList)
	{
		csList.Empty();
		csNameSPDot.Trim();
		if(csNameSPDot.Right(1)!=_T("."))
			return false;

		csFun.Trim();
		if(csFun.Right(1)!=_T("("))
			return false;

		csNameSPDot.Delete(csNameSPDot.GetLength()-1);
		csFun.Delete(csFun.GetLength()-1);

		CString example=MyRuntimeManager::fApiFrame->findFunExample(csNameSPDot,csFun);
		if(example.IsEmpty())
			return false;
		//////////////ע��,������APIXML����д��example����� ';"�ֺ�,ȫ���滻�����з���,
		//////////////���Ծ������治Ӧ�ô���';'�ŵ��ַ�������
		//example.Trim();
		//example.Replace(_T("|"),_T("\r\n"));

		CString XMLStr=example;
		//�õ�����ԭ��
		CString str;
		CString tempFunProto;
		int n;
		n=XMLStr.Find(L"|");
		str=XMLStr.Right(XMLStr.GetLength()-n-1);
		n=str.Find(L"|");
		tempFunProto=str.Left(n);
		tempFunProto.Trim();
		csList=tempFunProto;
		return true;
	}

	//void CTCView::CheckMenus(void)
	//{
	//  EnableAMenuItem(ID_FILE_SAVE, GetDocument()->IsModified());
	//	EnableAMenuItem(ID_EDIT_UNDO, SendEditor(EM_CANUNDO));
	//	EnableAMenuItem(ID_EDIT_REDO, SendEditor(SCI_CANREDO));
	//	EnableAMenuItem(ID_EDIT_PASTE, SendEditor(EM_CANPASTE));
	//}
	//
	//void CTCView::EnableAMenuItem(int id, bool enable)
	//{
	//	CMainFrame *pFrame = ::
	//	if (enable)
	//		::EnableMenuItem(pFrame->m_hMenu, id, MF_ENABLED | MF_BYCOMMAND);
	//	else
	//		::EnableMenuItem(pFrame->m_hMenu, id, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
	//}

	void CTCView::showIntellisenseWindow()
	{
		//���Position     
		CPoint caret_pos = GetCaretPos();
		int LINE_HEIGHT = SendEditor(SCI_TEXTHEIGHT);
		int move_height = caret_pos.y  + LINE_HEIGHT; // 15;
		RECT rct;
		m_listView.GetWindowRect(&rct);
		m_listView.MoveWindow(caret_pos.x,move_height,rct.right-rct.left,rct.bottom-rct.top);
		m_listView.ShowWindow();
		//m_listView.SetFocus();
	}

	void CTCView::hideIntellisenseWindow()
	{ 
		m_listView.HideWindow();
		m_listView.m_isSend = true;
	}

	BOOL CTCView::PreTranslateMessage(MSG* pMsg)
	{
		// TODO: �ڴ����ר�ô����/����û���
		 
		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam != VK_BACK)
				StatubarsShowRowAndCul();
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == L'C' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{
			m_ScintillaWnd.SendMessage(SCI_COPY);
			//StatubarsShowRowAndCul();
			return true;
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == L'X' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{

			m_ScintillaWnd.SendMessage(SCI_CUT);
			//StatubarsShowRowAndCul();
			return true;
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == L'V' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{ 

			m_ScintillaWnd.SendMessage(SCI_PASTE);
			//StatubarsShowRowAndCul();
			return true;
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == L'A' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{
			m_ScintillaWnd.SendMessage(SCI_SELECTALL);
			//StatubarsShowRowAndCul();
			return true;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'Z' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{
			m_ScintillaWnd.SendMessage(SCI_UNDO);
			//StatubarsShowRowAndCul();
			return true;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'F' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{
			OnEditFind();
			return true;
		}
		if( pMsg->message == WM_LBUTTONUP )
		{
			StatubarsShowRowAndCul();
			//�������ʱ������ʾ����
			hideIntellisenseWindow();

		}
		if(pMsg->message == WM_MOUSEMOVE)
		{
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->RemoveToolTip(&m_ScintillaWnd);
			static bool isMouseTip=false;
			wchar_t line[1024]={0}; //���浱ǰ����µĵ��� 
			POINT point=pMsg->pt;
			m_ScintillaWnd.ScreenToClient(&point);
			int pos=SendEditor(SCI_POSITIONFROMPOINT,point.x,point.y);  
			const int line_number = SendEditor(SCI_LINEFROMPOSITION,pos); 
			m_ScintillaWnd.SendMessage(SCI_GETLINE,line_number,(LPARAM)line);//��ǰ������ʼλ�� 
			int lineStart=m_ScintillaWnd.SendMessage(SCI_POSITIONFROMLINE,line_number);//��ǰ������ʼλ�� 
			int curPos=pos-lineStart;
			int lineLength=::wcslen(line);
			int nShowLeng=0;	
			wchar_t textShow[1024]={0}; //���浱ǰ����µĵ��� 
			for(int i=curPos;i<lineLength;i++)
			{
				if((XMLString::isAlpha(line[i])||XMLString::isDigit(line[i]) ||XMLString::isChinese(line[i])||line[i]==chUnderscore||line[i]==chPeriod))
				{
					textShow[i]=line[i];
					nShowLeng++;
				}																																																																																																																																																																																																							
				else 
					break;
			}
			for(int j=curPos;j>=0;j--)
			{
				if((XMLString::isAlpha(line[j])||XMLString::isDigit(line[j]) ||XMLString::isChinese(line[j])||line[j]==chUnderscore||line[j]==chPeriod))
				{
					textShow[j]=line[j];
					nShowLeng++;
				}
				else
					break;
			}
			if(nShowLeng)
			{           
				TCHAR * pToShow=textShow;
				for(int i=0;i<256;i++)
				{
					if(*pToShow)
						break;
					pToShow++;
				}
				CString csNSFUN=pToShow;
				int dotIndex=csNSFUN.Find(_T('.'));
				if(dotIndex!=-1)
				{
					CString namsSP=csNSFUN.Mid(0,dotIndex);
					CString nameFun=csNSFUN.Mid(dotIndex+1,csNSFUN.GetLength()-dotIndex);

					CString example=MyRuntimeManager::fApiFrame->findFunExample(namsSP,nameFun);
					if(!example.IsEmpty())
					{
						isMouseTip=true;
						//m_ScintillaWnd.SendMessage(SCI_CALLTIPSHOW,pos,sptr_t(example.GetBuffer()));//��ʾ�����ʾ 
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->GiveToolTip(&m_ScintillaWnd, example, NULL);
					}
				}
			}
			else
			{
				if(m_ScintillaWnd.SendMessage(SCI_CALLTIPACTIVE)&&isMouseTip)
				{
					m_ScintillaWnd.SendMessage(SCI_CALLTIPCANCEL);//��ʾ�����ʾ 
					isMouseTip=false;
				}
			}
		}

		if( pMsg->wParam == VK_BACK )
		{
			hideIntellisenseWindow();
			StatubarsShowRowAndCul();
			//wchar_t word[1000]; //���浱ǰ����µĵ��� 
			//wchar_t wordPre[1000]; //���浱ǰ����µĵ��� 
			//TextRange tr;    //����SCI_GETTEXTRANGE���� 
			int pos = m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ�� 
			//int startpos = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,pos-1);//��ǰ������ʼλ�� 
			//int endpos = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,pos-1);//��ǰ������ֹλ�� 

			//int startposPre = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,startpos-1);//��ǰ������ʼλ�� 
			//int endposPre = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,startpos-1);//��ǰ������ֹλ�� 

			//TextRange trPre;    //����SCI_GETTEXTRANGE���� 
			//trPre.chrg.cpMin = startposPre;  //�趨�������䣬ȡ������ 
			//trPre.chrg.cpMax = endposPre; 
			//trPre.lpstrText = wordPre; 

			//m_ScintillaWnd.SendMessage(SCI_GETTEXTRANGE,0, sptr_t(&trPre)); 

			//tr.chrg.cpMin = startpos;  //�趨�������䣬ȡ������ 
			//tr.chrg.cpMax = endpos; 
			//tr.lpstrText = word;
			//m_ScintillaWnd.SendMessage(SCI_GETTEXTRANGE,0, sptr_t(&tr));

			int nStart=-1;
			int nEnd=-1;

			///////////////////////////���λ��///////////////////////

			///////////////////////////������ڵ��ַ����/////////////////
			int indexChar = pos; //myRichEdit->CharFromPos(pot);
			nEnd=indexChar;
			//int nline=myRichEdit->LineFromChar(indexChar);
			int nline = SendEditor(SCI_LINEFROMPOSITION,pos);

			//int nBegin = myRichEdit->LineIndex(nline);
			int nBegin = m_ScintillaWnd.SendMessage(SCI_POSITIONFROMLINE,nline);;
			TCHAR *lineBuf;
			//int nLineLength=myRichEdit->LineLength(nBegin);
			int nLineLength=SendEditor(SCI_LINELENGTH,nline);

			if(nLineLength<=1)
				nLineLength=16;

			if(nLineLength>=1024)
				return false;

			lineBuf=new TCHAR[nLineLength+1];
			::memset(lineBuf,0,(nLineLength+1)*sizeof(TCHAR));
			m_ScintillaWnd.SendMessage(SCI_GETLINE,nline,(LPARAM)lineBuf);
			//myRichEdit->GetLine(nline, lineBuf,nLineLength); 

			TCHAR textShow[1024];
			memset(textShow,0,1024*sizeof(TCHAR));
			int scanleng=1024;
			if(nLineLength<1024)
				scanleng=nLineLength;
			int nShowLeng=0;

			for(int j=indexChar-nBegin-1;j>=0;j--)
			{
				if((XMLString::isAlpha(lineBuf[j])||XMLString::isDigit(lineBuf[j]) ||XMLString::isChinese(lineBuf[j])||lineBuf[j]==chUnderscore||lineBuf[j]==chPeriod))
				{
					if(nStart==-1)
						nStart=indexChar+j;


					textShow[j]=lineBuf[j];
					nShowLeng++;
				}
				else 
					break;
			}

			if(nShowLeng)
			{
				TCHAR * pToShow=textShow;
				for(int i=0;i<1024;i++)
				{
					if(*pToShow)
						break;
					pToShow++;
				}

				if(nStart!=-1||nEnd!=-1)
				{
					if(m_listView.doFilter(pToShow,nStart,nEnd))
					{
						showIntellisenseWindow();
						//return true;
					}
					//else
					//	hideIntellisenseWindow();
				}
				//else
				//	hideIntellisenseWindow();
			}
			//else
			//hideIntellisenseWindow();

			delete [] lineBuf;
		}

		if(pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP)
		{
			StatubarsShowRowAndCul();
			if(m_listView.m_isShow)
			{
				m_listView.SetFocus();
				//��һ�ΰ�������ʱͬʱ����ʾ���ڷ���һ�����µ���Ϣ
				if(m_listView.m_isSend)
					m_listView.SendMessage(WM_KEYDOWN, pMsg->wParam, 0);
				if( pMsg->hwnd == m_ScintillaWnd.m_hWnd )
					return true;
			}
		}

		if( pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT )
			StatubarsShowRowAndCul();

		if(pMsg->wParam == VK_SHIFT)
			hideIntellisenseWindow();

		return CView::PreTranslateMessage(pMsg);
	}

	void CTCView::StatubarsShowRowAndCul()
	{
		const int pos = SendEditor(SCI_GETCURRENTPOS); 
		const int line_number = SendEditor(SCI_LINEFROMPOSITION,pos);
		const int col_number = SendEditor(SCI_GETCOLUMN,pos);
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCSetStatusRowAndCol(line_number+1,col_number+1);
	}

	void CTCView::OnUpdateTcNewDelBp(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		if(CTCRunTimeManager::g_IsLoadProj)
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CTCView::OnUpdateTcDeleteallbp(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		if(CTCRunTimeManager::g_IsLoadProj)
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CTCView::OnUpdateEditUndo(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		if(SendEditor(EM_CANUNDO))
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CTCView::OnEditUndo()
	{
		// TODO: �ڴ���������������
		m_ScintillaWnd.SendMessage(SCI_UNDO);
		StatubarsShowRowAndCul();
	}

	void CTCView::OnUpdateEditRedo(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		if(SendEditor(SCI_CANREDO))
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CTCView::OnEditRedo()
	{
		// TODO: �ڴ���������������
		m_ScintillaWnd.SendMessage(SCI_REDO);
		StatubarsShowRowAndCul();
	}

	void CTCView::OnEditCut()
	{
		// TODO: �ڴ���������������
		m_ScintillaWnd.SendMessage(SCI_CUT);
		StatubarsShowRowAndCul();
	}

	void CTCView::OnEditCopy()
	{
		// TODO: �ڴ���������������
		m_ScintillaWnd.SendMessage(SCI_COPY);
		StatubarsShowRowAndCul();
	}

	void CTCView::OnEditPaste()
	{
		// TODO: �ڴ���������������
		m_ScintillaWnd.SendMessage(SCI_PASTE);
		StatubarsShowRowAndCul();
	}

	void CTCView::OnEditDelete()
	{
		// TODO: �ڴ���������������
		m_ScintillaWnd.SendMessage(SCI_CLEAR);
		StatubarsShowRowAndCul();
	}

	void CTCView::OnEditSelectAll()
	{
		// TODO: �ڴ���������������
		m_ScintillaWnd.SendMessage(SCI_SELECTALL);
		StatubarsShowRowAndCul();
	}

	void CTCView::TCAddComment()
	{
		m_ScintillaWnd.SendMessage(TC_ADDCOMMENT);
		StatubarsShowRowAndCul();
	}

	void CTCView::TCActiveEditView()
	{
		//this->GetParent()->SetFocus();
		//this->SetFocus();
		SendEditor(SCI_GRABFOCUS);
	}

	//9-4�� �½�ɾ���ϵ�
	void CTCView::OnTcNewDelBp()
	{
		// TODO: �ڴ���������������
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CString csPath=GetDocument()->GetPathName();
		csPath.Trim();
		csPath.MakeLower();
		//���Ҫ�õ���ǰ��������к�
		const int pos = SendEditor(SCI_GETCURRENTPOS); 
		const int line_number = SendEditor(SCI_LINEFROMPOSITION,pos);
		//���Ҫ�õ���ǰ��������к�
		LRESULT state =SendEditor(SCI_MARKERGET,line_number);
		if((state & (1 << 1)) != 0)
		{
			SendEditor(SCI_MARKERDELETE, line_number, 1);
			pFrame->TCDebugDeleteBP(csPath,line_number+1/*�����1����Ϊ�к���������1��ʼ��,���༭���ڲ���0��ʼ��*/);
		}
		else
		{
			SendEditor(SCI_MARKERADD, line_number, 1);
			pFrame->TCDebugAddBP(csPath,line_number+1/*�����1����Ϊ�к���������1��ʼ��,���༭���ڲ���0��ʼ��*/);
		}
	}

	//9-4ɾ�����жϵ����()
	void CTCView::OnTcDeleteallbp()
	{
		// TODO: �ڴ���������������
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		//9-13�޸�ɾ�����жϵ�,Ҫ�����-1
		SendEditor(SCI_MARKERDELETEALL,-1);
		pFrame->TCDebugDeleteAllBP();

	}


	void CTCView::OnEditAddcomment()
	{
		// TODO: �ڴ���������������
		TCAddComment();
		StatubarsShowRowAndCul();
	}


	void CTCView::OnUpdateEditAddcomment(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
	}


	void CTCView::OnEditRemovecomment()
	{
		// TODO: �ڴ���������������
		m_ScintillaWnd.SendMessage(TC_REMOVECOMMENT);
		StatubarsShowRowAndCul();
	}


	void CTCView::OnUpdateEditRemovecomment(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
	}


	void CTCView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
	{
		// TODO: �ڴ����ר�ô����/����û���
		StatubarsShowRowAndCul();

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_currentViewType = TCVIEW_FILE;

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->ResetPropertyCombox();

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->InsertCtrlToProCom(_T("TC�ĵ�������ӵ�"));
		/////////////////���ͬ���ļ�����/////////////////////
		CString filePath = GetDocument()->GetPathName();

		filePath.Trim();
		filePath.MakeLower();

		//�����ȡ�ļ��Ĵ�С
		UINT lenth=TCGetLength();

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCSetFileInfo(lenth);

		CString fileEx=filePath.Right(2);
		if(fileEx==_T(".t"))
		{
			int iLength = lstrlen(filePath) ;
			int iIndex = filePath.ReverseFind(L'\\') ;
			CString fileName = filePath.Right(iLength - iIndex - 1); 
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(fileName,filePath);
		}
		/////////////////���ͬ���ļ�����////////////////////
		//this->TCToggleFoldLoad();
		CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
		//if(!bActivate)
		//{
		//	if(!m_isFirst)
		//	{
		//		SendEditor(SCI_GOTOLINE,10000);
		//	    SendEditor(SCI_GOTOLINE,0);
		//		TCToggleFoldLoad();
		//		m_isFirst=true;
		//	}

		//}
		//TCToggleFoldLoad();
	}


	void CTCView::OnEditFind()
	{
		// TODO: �ڴ���������������
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCActiveFindWnd();

        //TCToggleFoldLoad();
	}

	void CTCView::TCFindText(CString text,int flag,int start,int end)
	{
		int flags = SCFIND_MATCHCASE | SCFIND_WHOLEWORD;

		TextToFind ttf;

		ttf.chrg.cpMin = start;

		ttf.chrg.cpMax = end;

		ttf.lpstrText = text.GetBuffer();

		int pos = SendEditor(SCI_FINDTEXT, flag, (LPARAM)&ttf);

		if(pos==-1)
		{
			SendEditor(SCI_SETSEL,0,0);
		}
		else
		{
			TCGotoPos(pos);
			SendEditor(SCI_SETSEL,pos,pos+::wcslen(text));
		}
	}

	void CTCView::TCReplaceSelText(CString old,CString toReplace,int flag)
	{
		TCHAR selString[1024]={0};
		SendEditor(SCI_GETSELTEXT,NULL,(LPARAM)selString);

		if(CTCRunTimeManager::isSameFile(selString,old))
		{
			SendEditor(SCI_REPLACESEL,NULL,(LPARAM)toReplace.GetBuffer());
		}

		//������һ��
		//int flags = SCFIND_MATCHCASE | SCFIND_WHOLEWORD;

		TextToFind ttf;

		ttf.chrg.cpMin = this->TCGetCurrentPos();

		ttf.chrg.cpMax = this->TCGetLength();

		ttf.lpstrText = old.GetBuffer();

		int pos = SendEditor(SCI_FINDTEXT, flag, (LPARAM)&ttf);

		if(pos==-1)
		{
			SendEditor(SCI_SETSEL,0,0);
		}
		else
		{
			TCGotoPos(pos);
			SendEditor(SCI_SETSEL,pos,pos+::wcslen(old));
		}

	}


	void CTCView::OnInitialUpdate()
	{
		CView::OnInitialUpdate();

		// TODO: �ڴ����ר�ô����/����û���
		//this->TCToggleFoldLoad();
		//SendEditor(SCI_GOTOLINE,100000);
		//SendEditor(SCI_GOTOLINE,0);
		
		m_allSize = SendEditor(SCI_GETLENGTH); //����ܴ�С
		m_rowCount = SendEditor(SCI_GETLINECOUNT); //���������
		if(!notFirst)
			notFirst = true;
	}


	void CTCView::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: �ڴ˴������Ϣ����������
		TCToggleFoldSave();
	}


	LRESULT CTCView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		// TODO: �ڴ����ר�ô����/����û���
		//if(message == WM_NOTIFY)
		//{	
		//	static int LastProcessedChar = 0;   


		//	SCNotification* notify = (SCNotification*) lParam; 
		//	if(notify->nmhdr.code == SCN_MARGINCLICK ){ 
		//		// ȷ����ҳ�ߵ���¼� 
		//		if(notify->margin==2)
		//		{
		//			// ȷ����ҳ�ߵ���¼� 
		//			const int line_number = SendEditor(SCI_LINEFROMPOSITION,notify->position); 
		//			SendEditor(SCI_TOGGLEFOLD, line_number); 
		//		}
		//		if(notify->margin==1)
		//		{
		//			CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		//			CString csPath=GetDocument()->GetPathName();
		//			csPath.Trim();
		//			csPath.MakeLower();
		//			const int line_number = SendEditor(SCI_LINEFROMPOSITION,notify->position); 
		//			LRESULT state =SendEditor(SCI_MARKERGET,line_number);
		//			if((state & (1 << 1)) != 0)
		//			{
		//				SendEditor(SCI_MARKERDELETE, line_number, 1);
		//				pFrame->TCDebugDeleteBP(csPath,line_number+1/*�����1����Ϊ�к���������1��ʼ��,���༭���ڲ���0��ʼ��*/);
		//			}
		//			else
		//			{
		//				SendEditor(SCI_MARKERADD, line_number, 1);
		//				pFrame->TCDebugAddBP(csPath,line_number+1/*�����1����Ϊ�к���������1��ʼ��,���༭���ڲ���0��ʼ��*/);
		//			}
		//		}
		//	}

		//	switch (notify->nmhdr.code) {

		//		//case SCN_MODIFIED:    
		//		//	//This notification is sent when the text or     
		//		//	//styling of the document changes or is about     
		//		//	//to change   
		//		//	{
		//		//		//::AfxMessageBox(_T("SCN_MODIFIED"));
		//		//		//GetDocument()->SetModifiedFlag(true);
		//		//		////CheckMenus();
		//		//		////�����������޸ĵ��ļ��ı���,���ļ���������*
		//		//		//CString csTitle=GetDocument()->GetPathName();
		//		//		//CString csOldTitle=GetDocument()->GetTitle();
		//		//		//int iLength = lstrlen(csTitle) ;
		//		//		//int iIndex = csTitle.ReverseFind(L'\\') ;
		//		//		//CString fileName = csTitle.Right(iLength - iIndex - 1); 
		//		//		//fileName.AppendChar(L'*');
		//		//		//if(csOldTitle!=fileName)
		//		//		//	GetDocument()->SetTitle(fileName);
		//		//		break;
		//		//	}
		//	case SCN_CHARADDED:
		//		{
		//			//SendEditor(SCN_MODIFIED, 0, 0);

		//			//�����Զ�����
		//			LastProcessedChar = notify->ch;  
		//			hideIntellisenseWindow();
		//			//�����кź��к�
		//			StatubarsShowRowAndCul();

		//			wchar_t word[1000]; //���浱ǰ����µĵ��� 
		//			wchar_t wordPre[1000]; //���浱ǰ����µĵ��� 
		//			TextRange tr;    //����SCI_GETTEXTRANGE���� 
		//			int pos = m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ�� 
		//			int startpos = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,pos-1);//��ǰ������ʼλ�� 
		//			int endpos = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,pos-1);//��ǰ������ֹλ�� 

		//			int startposPre = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,startpos-1);//��ǰ������ʼλ�� 
		//			int endposPre = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,startpos-1);//��ǰ������ֹλ�� 

		//			TextRange trPre;    //����SCI_GETTEXTRANGE���� 
		//			trPre.chrg.cpMin = startposPre;  //�趨�������䣬ȡ������ 
		//			trPre.chrg.cpMax = endposPre; 
		//			trPre.lpstrText = wordPre; 
		//			m_ScintillaWnd.SendMessage(SCI_GETTEXTRANGE,0, sptr_t(&trPre)); 

		//			tr.chrg.cpMin = startpos;  //�趨�������䣬ȡ������ 
		//			tr.chrg.cpMax = endpos; 
		//			tr.lpstrText = word; 
		//			m_ScintillaWnd.SendMessage(SCI_GETTEXTRANGE,0, sptr_t(&tr));

		//			int nStart=-1;
		//			int nEnd=-1;

		//			///////////////////////////���λ��///////////////////////

		//			///////////////////////////������ڵ��ַ����/////////////////
		//			int indexChar = pos; //myRichEdit->CharFromPos(pot);
		//			nEnd=indexChar;
		//			//int nline=myRichEdit->LineFromChar(indexChar);
		//			int nline = SendEditor(SCI_LINEFROMPOSITION,pos);

		//			//int nBegin = myRichEdit->LineIndex(nline);
		//			int nBegin = m_ScintillaWnd.SendMessage(SCI_POSITIONFROMLINE,nline);;
		//			TCHAR *lineBuf;
		//			//int nLineLength=myRichEdit->LineLength(nBegin);
		//			int nLineLength=SendEditor(SCI_LINELENGTH,nline);

		//			if(nLineLength<=1)
		//				nLineLength=16;

		//			if(nLineLength>=1024)
		//				return false;

		//			lineBuf=new TCHAR[nLineLength+1];
		//			::memset(lineBuf,0,(nLineLength+1)*sizeof(TCHAR));
		//			m_ScintillaWnd.SendMessage(SCI_GETLINE,nline,(LPARAM)lineBuf);
		//			//myRichEdit->GetLine(nline, lineBuf,nLineLength); 

		//			TCHAR textShow[1024];
		//			memset(textShow,0,1024*sizeof(TCHAR));
		//			int scanleng=1024;
		//			if(nLineLength<1024)
		//				scanleng=nLineLength;
		//			int nShowLeng=0;

		//			for(int j=indexChar-nBegin-1;j>=0;j--)
		//			{
		//				if((XMLString::isAlpha(lineBuf[j])||XMLString::isDigit(lineBuf[j]) ||XMLString::isChinese(lineBuf[j])||lineBuf[j]==chUnderscore||lineBuf[j]==chPeriod))
		//				{
		//					if(nStart==-1)
		//						nStart=indexChar+j;
		//					textShow[j]=lineBuf[j];
		//					nShowLeng++;
		//				}
		//				else 
		//					break;
		//			}

		//			if(nShowLeng)
		//			{
		//				TCHAR * pToShow=textShow;
		//				for(int i=0;i<1024;i++)
		//				{
		//					if(*pToShow)
		//						break;
		//					pToShow++;
		//				}

		//				if(nStart!=-1||nEnd!=-1)
		//				{
		//					if(m_listView.doFilter(pToShow,nStart,nEnd))
		//					{
		//						showIntellisenseWindow();
		//					}
		//					//else
		//					//	hideIntellisenseWindow();
		//				}
		//				//else
		//				//	hideIntellisenseWindow();;
		//			}
		//			//else
		//			//	hideIntellisenseWindow();

		//			delete [] lineBuf;

		//			// ������ʾ���� 
		//			static const wchar_t* pCallTipNextWord = NULL;//��һ������λ�� 
		//			static const wchar_t* pCallTipCurDesc = NULL;//��ǰ��ʾ�ĺ�����Ϣ 
		//			if(notify->ch == L'(') //��������������ţ���ʾ������ʾ 
		//			{ 	
		//				static CString csParam;
		//				csParam.Empty();
		//				if(TCGetFunParam(wordPre,word,csParam))
		//				{
		//					//if(memcmp(g_szFuncList[i],word,sizeof(g_szFuncList[i])) == 0) 
		//					//if(	strcmp(g_szFuncList[i], word)==0)
		//					{     
		//						//�ҵ�������ô��ʾ��ʾ�� 
		//						pCallTipCurDesc = csParam; //��ǰ��ʾ�ĺ�����Ϣ 
		//						m_ScintillaWnd.SendMessage(SCI_CALLTIPSHOW,pos,sptr_t(pCallTipCurDesc));//��ʾ�����ʾ 
		//						const wchar_t *pStart = wcschr(pCallTipCurDesc,L'(')+1; //������һ������ 
		//						const wchar_t *pEnd = wcschr(pStart,L',');//�����б��Զ��ŷָ� 
		//						if(pEnd == NULL) pEnd = wcschr(pStart,L')');//�������һ�������������������� 
		//						m_ScintillaWnd.SendMessage(SCI_CALLTIPSETHLT, 
		//							pStart-pCallTipCurDesc, pEnd-pCallTipCurDesc); 
		//						pCallTipNextWord = pEnd+1;//ָ����һ����λ�� 
		//						break; 
		//					} 
		//				}
		//			} 
		//			else if(notify->ch == L')') //������������ţ��͹رպ�����ʾ 
		//			{ 
		//				m_ScintillaWnd.SendMessage(SCI_CALLTIPCANCEL); 
		//				pCallTipCurDesc = NULL; 
		//				pCallTipNextWord = NULL;                 
		//			} 
		//			else if(notify->ch == L',' && m_ScintillaWnd.SendMessage(SCI_CALLTIPACTIVE) && pCallTipCurDesc) 
		//			{ 
		//				//������Ƕ��ţ�������һ������ 
		//				const wchar_t *pStart = pCallTipNextWord; 
		//				const wchar_t *pEnd = wcschr(pStart,L','); 
		//				// 			pStart = pCallTipNextWord; 
		//				// 			pEnd = strchr(pStart,','); 
		//				if(pEnd == NULL) pEnd = wcschr(pStart,L')'); 
		//				if(pEnd == NULL) //û����һ�����������ر���ʾ 
		//					m_ScintillaWnd.SendMessage(SCI_CALLTIPCANCEL); 
		//				else 
		//				{ 
		//					m_ScintillaWnd.SendMessage(SCI_CALLTIPSETHLT,pStart-pCallTipCurDesc, pEnd-pCallTipCurDesc); 
		//					pCallTipNextWord = pEnd+1; 
		//				} 
		//			} 

		//			//LastProcessedChar = scn->ch; 
		//			//	if(LastProcessedChar!=13)
		//			//		break;

		//			//����ǰ�󵼺�,��¼λ�õ���ջ
		//			const int line_number = m_ScintillaWnd.SendMessage(SCI_LINEFROMPOSITION,pos); //ȡ���к�
		//			m_markposL.push_back(line_number);

		//			break;
		//		}

		//	case SCN_UPDATEUI:
		//		{
		//			//ʵ���Զ�������Ч��
		//			if(LastProcessedChar!=0)
		//			{
		//				int pos = SendEditor(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ��   
		//				int linecur = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ�õ�ǰ�� 				
		//				int startpos = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,pos-1);//��ǰ������ʼλ�� 
		//				int endpos = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,pos-1);//��ǰ������ֹλ�� 
		//				int linepos = SendEditor(SCI_POSITIONFROMLINE,linecur); //��ǰ����ʼλ��   
		//				int lineend = SendEditor(SCI_GETLINEENDPOSITION,linecur); //��ǰ����ʼλ�� 
		//				wchar_t line[1024]={0}; 
		//				m_ScintillaWnd.SendMessage(SCI_GETCURLINE,1024,sptr_t(line));
		//				wchar_t word[1024]={0}; 
		//				TextRange tr;    //����SCI_GETTEXTRANGE���� 

		//				tr.chrg.cpMin = startpos;  //�趨�������䣬ȡ������ 
		//				tr.chrg.cpMax = endpos; 
		//				tr.lpstrText = word; 
		//				m_ScintillaWnd.SendMessage(SCI_GETTEXTRANGE,0, sptr_t(&tr));
		//				m_ScintillaWnd.SendMessage(SCI_GETCURLINE,1024,sptr_t(line));
		//				if(CTCRunTimeManager::isMatchStatement(word))
		//				{
		//					int linestart=SendEditor(TC_BRACEMATCH,pos,sptr_t(word));
		//					if(linestart!=-1)
		//					{
		//						int nIndent = SendEditor(SCI_GETLINEINDENTATION,linestart);//�����е�����ֵ   
		//						// �滻�������ǰ��Ŀհ��ַ���ʹ֮������������һ��   
		//						wchar_t space[1024];   
		//						::wmemset(space,L' ',1024);
		//						space[nIndent] = 0;   
		//						//CString csLine=line;
		//						//csLine.Trim();
		//						//::wcscat(space,csLine);
		//						/*						SendEditor(SCI_SETSEL,linepos,lineend);
		//						SendEditor(SCI_REPLACESEL, 0, (sptr_t)space);*/   


		//						SendEditor(SCI_SETTARGETSTART, linepos);   
		//						SendEditor(SCI_SETTARGETEND, startpos);   
		//						SendEditor(SCI_REPLACETARGET,nIndent,(sptr_t)space);
		//					}
		//				}

		//				// ���������ǻس����򱣳�����һ������һ��   
		//				// �����һ�������Ч�ַ�Ϊ�����ţ��Ͷ������ĸ��ո�   
		//				if(LastProcessedChar == '\n')   
		//				{   
		//					if(linecur>0)
		//					{
		//						// �õ���һ����������   
		//						int nIndent = SendEditor(SCI_GETLINEINDENTATION,linecur-1);   
		//						wchar_t linepre[1024]={0}; 
		//						m_ScintillaWnd.SendMessage(SCI_GETLINE,linecur-1,sptr_t(linepre));
		//						if(CTCRunTimeManager::isBraceStatement(linepre))
		//						{
		//							wchar_t space[1024];   
		//							::wmemset(space,L' ',1024);
		//							space[nIndent+4] = 0;   
		//							CString csLine=line;
		//							csLine.Trim();
		//							::wcscat(space,csLine);
		//							SendEditor(SCI_SETSEL,linepos,lineend);
		//							SendEditor(SCI_REPLACESEL, 0, (sptr_t)space);  
		//						}
		//						else
		//						{
		//							if(pos==lineend)
		//							{
		//								wchar_t space[1024];   
		//								::wmemset(space,L' ',1024);
		//								space[nIndent] = 0;   
		//								CString csLine=line;
		//								csLine.Trim();
		//								::wcscat(space,csLine);
		//								SendEditor(SCI_SETSEL,linepos,lineend);
		//								SendEditor(SCI_REPLACESEL, 0, (sptr_t)space);  
		//							}
		//						}
		//					}
		//				}   
		//			}

		//			LastProcessedChar = 0;   
		//			break;
		//		}
		//	case SCN_SAVEPOINTREACHED:
		//		{
		//			GetDocument()->SetModifiedFlag(false);
		//			//::AfxMessageBox(_T("SAVEPOINTREACHED"));
		//			//isDirty = false;
		//			//GetDocument()->SetModifiedFlag(false);
		//			//CheckMenus();
		//			break;
		//		}
		//		//���ĵ�״̬��Ϊmodifiedʱ�����ᴥ��SCN_SAVEPOINTLEFT�¼�֪ͨ
		//	case SCN_SAVEPOINTLEFT:
		//		{
		//			//::AfxMessageBox(_T("SAVEPOINTLEFT"));
		//			GetDocument()->SetModifiedFlag(true);
		//			//CheckMenus();
		//			//�����������޸ĵ��ļ��ı���,���ļ���������*
		//			CString csTitle=GetDocument()->GetPathName();
		//			CString csOldTitle=GetDocument()->GetTitle();
		//			int iLength = lstrlen(csTitle) ;
		//			int iIndex = csTitle.ReverseFind(L'\\') ;
		//			CString fileName = csTitle.Right(iLength - iIndex - 1); 
		//			fileName.AppendChar(L'*');
		//			if(csOldTitle!=fileName)
		//				GetDocument()->SetTitle(fileName);
		//			//isDirty = true;
		//			//GetDocument()->SetModifiedFlag(true);
		//			//CheckMenus();
		//			break;					
		//		}
		//	case SCN_NEEDSHOWN: 
		//		{
		//			int pos = m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ��
		//			const int line_number = m_ScintillaWnd.SendMessage(SCI_LINEFROMPOSITION,pos); //ȡ���к�
		//			int rets = m_ScintillaWnd.SendMessage(SCI_GETFOLDEXPANDED , line_number);
		//			if(!rets)
		//				m_ScintillaWnd.SendMessage(SCI_TOGGLEFOLD, line_number);
		//		}
		//	}
		//} 
		return CView::WindowProc(message, wParam, lParam);
	}

	void CTCView::TCToggleFoldSave()
	{
		TCHAR buffer[1024]={L""};
		TCHAR temp[32]={0};
		int end_line = SendEditor(SCI_GETLINECOUNT) - 1;

		for(int i = end_line; i>=0 ;i--)
		{
			int rets = SendEditor(SCI_GETFOLDEXPANDED , i);
			if(!rets)
			{
				memset(temp,0,sizeof(TCHAR)*32);
				_itow(i,temp,10);
				wcscat(buffer,temp);
				wcscat(buffer,L"|");
			}
		}

		WritePrivateProfileString(L"Fold",L"IsFold",buffer,CTCRunTimeManager::g_ProjFilePath);
	}

	void CTCView::TCToggleFoldLoad()
	{
		//int rowCount=SendEditor(SCI_GETLINECOUNT);
		//for(int i=0;i<rowCount;i+=10)
		//{
		//	SendEditor(SCI_LINESCROLL,0,10);
		//}
		
		TCHAR buffer[1024]={0};
		int rlen=0;
		std::wstring rtemp[1024]={L""};

		GetPrivateProfileString(L"Fold",L"IsFold",L"",buffer,1024,CTCRunTimeManager::g_ProjFilePath);

		if(wcslen(buffer))
		{
			my_split(buffer,L"|",rtemp,rlen);

			int *p = new int[rlen];

			for(int i=0; i<rlen; i++)
			{
				p[i] =_wtoi(rtemp[i].c_str());
			}


			SendEditor(SCI_LINESCROLL,0,p[0]);
			SendEditor(SCI_LINESCROLL,0,-2);
			SendEditor(SCI_TOGGLEFOLD, p[0]);

			for(int i=0; i<rlen-1; i++)
			{
				SendEditor(SCI_LINESCROLL,0,(p[i+1]-p[i]+2));
				SendEditor(SCI_LINESCROLL,0,-2);
				SendEditor(SCI_TOGGLEFOLD, p[i+1]);
			}

			//for(int i=0; i<rlen; i++)
			//{
			//	LRESULT re=SendEditor(TC_TOGGLEFOLD, p[i]);
			//}
			SendEditor(SCI_GOTOLINE,0);
			delete[] p;
		}

	}

	//ȥ��߿ո�
	TCHAR* TrimL(TCHAR *ptr)
	{
		while(*ptr == L' ' || *ptr == L'\t')
			ptr++;
		return ptr;
	}

	void CTCView::TCTypesetting()
	{
		int start = SendEditor(SCI_GETSELECTIONSTART); //���ѡ�п�ʼλ��
		int end = SendEditor(SCI_GETSELECTIONEND);	    //���ѡ�н���λ��
		int line_start = SendEditor(SCI_LINEFROMPOSITION,start); //ѡ�п�ʼ��
		int line_end = SendEditor(SCI_LINEFROMPOSITION,end);     //ѡ�н�����
		wchar_t space[1024*64]={L""};   //����ո�
		CString replaceTemp;  //�����滻����
		int Lever = 0;
		bool iskeywords =false;
		for(int i = line_start; i <= line_end ; i++ )
		{
			wchar_t linepre[1024*64]={0}; //��һ������
			wchar_t linecur[1024*64]={0}; //��ǰ������
			::wmemset(space,L' ',1024*64);

			SendEditor(SCI_GETLINE, i - 1 ,sptr_t(linepre)); //���ָ��������
			SendEditor(SCI_GETLINE, i  ,sptr_t(linecur)); //���ָ��������

			if(CTCRunTimeManager::isMatchStatement(linecur)|| CTCRunTimeManager::isMatchStatementEx(linecur)) //�����ؼ���
				iskeywords = true;
			else
				iskeywords = false;
			//wcscpy(linepre , TrimL(linepre));
			wcscpy(linecur , TrimL(linecur));

			if((CTCRunTimeManager::isBraceStatement(linepre))) //�ж��Ƿ�ʼ�ؼ���
			{
				if(!iskeywords)
					Lever++;
				space[Lever*4] = 0;
				::wcscat(space,linecur);
				replaceTemp += space;
			}
			else
			{
				if(iskeywords&&Lever>0) //�����ؼ���
					Lever--;
				space[Lever*4] = 0;
				::wcscat(space,linecur);
				replaceTemp += space;
				iskeywords = false;
			}	
		}

		int len = replaceTemp.GetLength();

		if(len >= 2 )
		{
			if(replaceTemp[len-2]==L'\r' && replaceTemp[len-1]==L'\n') //ȥ��β�����з�
				replaceTemp = replaceTemp.Left(len-2);	
			else if(replaceTemp[len-1]==L'\n')
				replaceTemp = replaceTemp.Left(len-1);
		}

		////replaceTemp = replaceTemp.Left(len-2);	//ȥ��β�����з�
		//if(replaceTemp[len-2]==L'\r' && replaceTemp[len-1]==L'\n') //ȥ��β�����з�
		//	replaceTemp = replaceTemp.Left(len-2);	
		//else if(replaceTemp[len-1]==L'\n')
		//	replaceTemp = replaceTemp.Left(len-1);

		int linestartpos = SendEditor(SCI_POSITIONFROMLINE, line_start);   //��ǰ����ʼλ��
		int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, line_end); //��ǰ����ʼλ��
		SendEditor(SCI_SETSEL, linestartpos, lineendpos); //�Զ���ѡ��λ��
		SendEditor(SCI_REPLACESEL, 0, (sptr_t)replaceTemp.LockBuffer());
		replaceTemp.UnlockBuffer();
	}


	void CTCView::OnTcTypesetting()
	{
		// TODO: �ڴ���������������
		notFirst=false;
		TCTypesetting();
		notFirst=true;
	}


	void CTCView::OnEditGoback()
	{
		// TODO: �ڴ���������������
	if(!m_markposL.empty() && !m_markposLpos.empty())
	{
		int nowline = m_markposL.back();
		DisplayLine(nowline);
		int linestartpos = SendEditor(SCI_POSITIONFROMLINE, nowline);   //��ǰ����ʼλ��
		int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, nowline); //��ǰ�н���λ��
		int lienlength = lineendpos - linestartpos;  //�µ��г���
		int oldlinelength = m_markLineLenthL.back(); //�ϵ��г���
		int d_valuelinepos =  lienlength - oldlinelength;   //ÿ�г��Ȳ�ֵ
		int nowpos = m_markposLpos.back();
		SendEditor(SCI_GOTOPOS,nowpos + linestartpos + d_valuelinepos +1);

		m_markposL.pop_back();
		m_markposLpos.pop_back();
		m_markLineLenthL.pop_back();
		
		m_markposR.push_back(nowline);
		m_markposRpos.push_back(nowpos);
		m_markLineLenthR.push_back(oldlinelength); //����µ��г���
	}
	}

	void CTCView::OnUpdateEditGoback(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		if(!m_markposL.empty() && !m_markposLpos.empty())
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}


	void CTCView::OnEditGoforward()
	{
		// TODO: �ڴ���������������
		//this->SetFocus();
		//this->m_ScintillaWnd.SetFocus();
		if(!m_markposR.empty() && !m_markposRpos.empty())
		{

		int nowline = m_markposR.back();
		DisplayLine(nowline);
		int linestartpos = SendEditor(SCI_POSITIONFROMLINE, nowline);   //��ǰ����ʼλ��
		int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, nowline); //��ǰ�н���λ��
		int lienlength = lineendpos - linestartpos;  //�µ��г���
		int oldlinelength = m_markLineLenthR.back(); //�ϵ��г���
		int d_valuelinepos =  lienlength - oldlinelength;   //ÿ�г��Ȳ�ֵ	
		int nowpos = m_markposRpos.back();
		SendEditor(SCI_GOTOPOS,nowpos + linestartpos + d_valuelinepos +1);
		m_markposR.pop_back();
		m_markposRpos.pop_back();
		m_markLineLenthR.pop_back();

		m_markposL.push_back(nowline);
		m_markposLpos.push_back(nowpos);
		m_markLineLenthL.push_back(oldlinelength); //����µ��г���
		}
	}


	void CTCView::OnUpdateEditGoforward(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		if(!m_markposR.empty() && !m_markposRpos.empty())
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}


	BOOL CTCView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		// TODO: �ڴ����ר�ô����/����û���
	
			static int LastProcessedChar = 0;   


			SCNotification* notify = (SCNotification*) lParam; 
			if(notify->nmhdr.code == SCN_MARGINCLICK ){ 
				// ȷ����ҳ�ߵ���¼� 
				if(notify->margin==2)
				{
					// ȷ����ҳ�ߵ���¼� 
					const int line_number = SendEditor(SCI_LINEFROMPOSITION,notify->position); 
					SendEditor(SCI_TOGGLEFOLD, line_number); 
				}
				if(notify->margin==1)
				{
					CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
					CString csPath=GetDocument()->GetPathName();
					csPath.Trim();
					csPath.MakeLower();
					const int line_number = SendEditor(SCI_LINEFROMPOSITION,notify->position); 
					LRESULT state =SendEditor(SCI_MARKERGET,line_number);
					if((state & (1 << 1)) != 0)
					{
						SendEditor(SCI_MARKERDELETE, line_number, 1);
						pFrame->TCDebugDeleteBP(csPath,line_number+1/*�����1����Ϊ�к���������1��ʼ��,���༭���ڲ���0��ʼ��*/);
					}
					else
					{
						SendEditor(SCI_MARKERADD, line_number, 1);
						pFrame->TCDebugAddBP(csPath,line_number+1/*�����1����Ϊ�к���������1��ʼ��,���༭���ڲ���0��ʼ��*/);
					}
				}
			}

			switch (notify->nmhdr.code) {

				//case SCN_MODIFIED:    
				//	//This notification is sent when the text or     
				//	//styling of the document changes or is about     
				//	//to change   
				//	{
				//		//::AfxMessageBox(_T("SCN_MODIFIED"));
				//		//GetDocument()->SetModifiedFlag(true);
				//		////CheckMenus();
				//		////�����������޸ĵ��ļ��ı���,���ļ���������*
				//		//CString csTitle=GetDocument()->GetPathName();
				//		//CString csOldTitle=GetDocument()->GetTitle();
				//		//int iLength = lstrlen(csTitle) ;
				//		//int iIndex = csTitle.ReverseFind(L'\\') ;
				//		//CString fileName = csTitle.Right(iLength - iIndex - 1); 
				//		//fileName.AppendChar(L'*');
				//		//if(csOldTitle!=fileName)
				//		//	GetDocument()->SetTitle(fileName);
				//		break;
				//	}
			case SCN_CHARADDED:
				{
					//SendEditor(SCN_MODIFIED, 0, 0);

					//�����Զ�����
					LastProcessedChar = notify->ch;  
					hideIntellisenseWindow();
					//�����кź��к�
					StatubarsShowRowAndCul();

					wchar_t word[1000]; //���浱ǰ����µĵ��� 
					wchar_t wordPre[1000]; //���浱ǰ����µĵ��� 
					TextRange tr;    //����SCI_GETTEXTRANGE���� 
					int pos = m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ�� 
					int startpos = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,pos-1);//��ǰ������ʼλ�� 
					int endpos = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,pos-1);//��ǰ������ֹλ�� 

					int startposPre = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,startpos-1);//��ǰ������ʼλ�� 
					int endposPre = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,startpos-1);//��ǰ������ֹλ�� 

					TextRange trPre;    //����SCI_GETTEXTRANGE���� 
					trPre.chrg.cpMin = startposPre;  //�趨�������䣬ȡ������ 
					trPre.chrg.cpMax = endposPre; 
					trPre.lpstrText = wordPre; 
					m_ScintillaWnd.SendMessage(SCI_GETTEXTRANGE,0, sptr_t(&trPre)); 

					tr.chrg.cpMin = startpos;  //�趨�������䣬ȡ������ 
					tr.chrg.cpMax = endpos; 
					tr.lpstrText = word; 
					m_ScintillaWnd.SendMessage(SCI_GETTEXTRANGE,0, sptr_t(&tr));

					int nStart=-1;
					int nEnd=-1;

					///////////////////////////���λ��///////////////////////

					///////////////////////////������ڵ��ַ����/////////////////
					int indexChar = pos; //myRichEdit->CharFromPos(pot);
					nEnd=indexChar;
					//int nline=myRichEdit->LineFromChar(indexChar);
					int nline = SendEditor(SCI_LINEFROMPOSITION,pos);

					//int nBegin = myRichEdit->LineIndex(nline);
					int nBegin = m_ScintillaWnd.SendMessage(SCI_POSITIONFROMLINE,nline);;
					TCHAR *lineBuf;
					//int nLineLength=myRichEdit->LineLength(nBegin);
					int nLineLength=SendEditor(SCI_LINELENGTH,nline);

					if(nLineLength<=1)
						nLineLength=16;

					if(nLineLength>=1024)
						return false;

					lineBuf=new TCHAR[nLineLength+1];
					::memset(lineBuf,0,(nLineLength+1)*sizeof(TCHAR));
					m_ScintillaWnd.SendMessage(SCI_GETLINE,nline,(LPARAM)lineBuf);
					//myRichEdit->GetLine(nline, lineBuf,nLineLength); 

					TCHAR textShow[1024];
					memset(textShow,0,1024*sizeof(TCHAR));
					int scanleng=1024;
					if(nLineLength<1024)
						scanleng=nLineLength;
					int nShowLeng=0;

					for(int j=indexChar-nBegin-1;j>=0;j--)
					{
						if((XMLString::isAlpha(lineBuf[j])||XMLString::isDigit(lineBuf[j]) ||XMLString::isChinese(lineBuf[j])||lineBuf[j]==chUnderscore||lineBuf[j]==chPeriod))
						{
							if(nStart==-1)
								nStart=indexChar+j;
							textShow[j]=lineBuf[j];
							nShowLeng++;
						}
						else 
							break;
					}

					if(nShowLeng)
					{
						TCHAR * pToShow=textShow;
						for(int i=0;i<1024;i++)
						{
							if(*pToShow)
								break;
							pToShow++;
						}

						if(nStart!=-1||nEnd!=-1)
						{
							if(m_listView.doFilter(pToShow,nStart,nEnd))
							{
								showIntellisenseWindow();
							}
							//else
							//	hideIntellisenseWindow();
						}
						//else
						//	hideIntellisenseWindow();;
					}
					//else
					//	hideIntellisenseWindow();

					delete [] lineBuf;

					// ������ʾ���� 
					static const wchar_t* pCallTipNextWord = NULL;//��һ������λ�� 
					static const wchar_t* pCallTipCurDesc = NULL;//��ǰ��ʾ�ĺ�����Ϣ 
					if(notify->ch == L'(') //��������������ţ���ʾ������ʾ 
					{ 	
						static CString csParam;
						csParam.Empty();
						if(TCGetFunParam(wordPre,word,csParam))
						{
							//if(memcmp(g_szFuncList[i],word,sizeof(g_szFuncList[i])) == 0) 
							//if(	strcmp(g_szFuncList[i], word)==0)
							{     
								//�ҵ�������ô��ʾ��ʾ�� 
								pCallTipCurDesc = csParam; //��ǰ��ʾ�ĺ�����Ϣ 
								m_ScintillaWnd.SendMessage(SCI_CALLTIPSHOW,pos,sptr_t(pCallTipCurDesc));//��ʾ�����ʾ 
								const wchar_t *pStart = wcschr(pCallTipCurDesc,L'(')+1; //������һ������ 
								const wchar_t *pEnd = wcschr(pStart,L',');//�����б��Զ��ŷָ� 
								if(pEnd == NULL) pEnd = wcschr(pStart,L')');//�������һ�������������������� 
								m_ScintillaWnd.SendMessage(SCI_CALLTIPSETHLT, 
									pStart-pCallTipCurDesc, pEnd-pCallTipCurDesc); 
								pCallTipNextWord = pEnd+1;//ָ����һ����λ�� 
								break; 
							} 
						}
					} 
					else if(notify->ch == L')') //������������ţ��͹رպ�����ʾ 
					{ 
						m_ScintillaWnd.SendMessage(SCI_CALLTIPCANCEL); 
						pCallTipCurDesc = NULL; 
						pCallTipNextWord = NULL;                 
					} 
					else if(notify->ch == L',' && m_ScintillaWnd.SendMessage(SCI_CALLTIPACTIVE) && pCallTipCurDesc) 
					{ 
						//������Ƕ��ţ�������һ������ 
						const wchar_t *pStart = pCallTipNextWord; 
						const wchar_t *pEnd = wcschr(pStart,L','); 
						// 			pStart = pCallTipNextWord; 
						// 			pEnd = strchr(pStart,','); 
						if(pEnd == NULL) pEnd = wcschr(pStart,L')'); 
						if(pEnd == NULL) //û����һ�����������ر���ʾ 
							m_ScintillaWnd.SendMessage(SCI_CALLTIPCANCEL); 
						else 
						{ 
							m_ScintillaWnd.SendMessage(SCI_CALLTIPSETHLT,pStart-pCallTipCurDesc, pEnd-pCallTipCurDesc); 
							pCallTipNextWord = pEnd+1; 
						} 
					} 

					//LastProcessedChar = scn->ch; 
					//	if(LastProcessedChar!=13)
					//		break;

					//����ǰ�󵼺�,��¼λ�õ���ջ
					//const int line_number = m_ScintillaWnd.SendMessage(SCI_LINEFROMPOSITION,pos); //ȡ���к�
					if(!m_markposL.empty())
					{
						int lastLine;
						lastLine=m_markposL.back();
						if(lastLine!=nline)
							m_markposL.push_back(nline);
					}
					else
						m_markposL.push_back(nline);

					break;
				}

			case SCN_UPDATEUI:
				{
					//ʵ���Զ�������Ч��
					if(LastProcessedChar!=0)
					{
						int pos = SendEditor(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ��   
						int linecur = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ�õ�ǰ�� 				
						int startpos = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,pos-1);//��ǰ������ʼλ�� 
						int endpos = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,pos-1);//��ǰ������ֹλ�� 
						int linepos = SendEditor(SCI_POSITIONFROMLINE,linecur); //��ǰ����ʼλ��   
						int lineend = SendEditor(SCI_GETLINEENDPOSITION,linecur); //��ǰ����ʼλ�� 
						wchar_t line[1024]={0}; 
						m_ScintillaWnd.SendMessage(SCI_GETCURLINE,1024,sptr_t(line));
						wchar_t word[1024]={0}; 
						TextRange tr;    //����SCI_GETTEXTRANGE���� 

						tr.chrg.cpMin = startpos;  //�趨�������䣬ȡ������ 
						tr.chrg.cpMax = endpos; 
						tr.lpstrText = word; 
						m_ScintillaWnd.SendMessage(SCI_GETTEXTRANGE,0, sptr_t(&tr));
						m_ScintillaWnd.SendMessage(SCI_GETCURLINE,1024,sptr_t(line));
						if(CTCRunTimeManager::isMatchStatement(word))
						{
							int linestart=SendEditor(TC_BRACEMATCH,pos,sptr_t(word));
							if(linestart!=-1)
							{
								int nIndent = SendEditor(SCI_GETLINEINDENTATION,linestart);//�����е�����ֵ   
								// �滻�������ǰ��Ŀհ��ַ���ʹ֮������������һ��   
								wchar_t space[1024];   
								::wmemset(space,L' ',1024);
								space[nIndent] = 0;   
								//CString csLine=line;
								//csLine.Trim();
								//::wcscat(space,csLine);
								/*						SendEditor(SCI_SETSEL,linepos,lineend);
								SendEditor(SCI_REPLACESEL, 0, (sptr_t)space);*/   


								SendEditor(SCI_SETTARGETSTART, linepos);   
								SendEditor(SCI_SETTARGETEND, startpos);   
								SendEditor(SCI_REPLACETARGET,nIndent,(sptr_t)space);
							}
						}

						// ���������ǻس����򱣳�����һ������һ��   
						// �����һ�������Ч�ַ�Ϊ�����ţ��Ͷ������ĸ��ո�   
						if(LastProcessedChar == '\n')   
						{   
							if(linecur>0)
							{
								// �õ���һ����������   
								int nIndent = SendEditor(SCI_GETLINEINDENTATION,linecur-1);   
								wchar_t linepre[1024]={0}; 
								m_ScintillaWnd.SendMessage(SCI_GETLINE,linecur-1,sptr_t(linepre));
								if(CTCRunTimeManager::isBraceStatement(linepre))
								{
									wchar_t space[1024];   
									::wmemset(space,L' ',1024);
									space[nIndent+4] = 0;   
									CString csLine=line;
									csLine.Trim();
									::wcscat(space,csLine);
									SendEditor(SCI_SETSEL,linepos,lineend);
									SendEditor(SCI_REPLACESEL, 0, (sptr_t)space);  
								}
								else
								{
									if(pos==lineend)
									{
										wchar_t space[1024];   
										::wmemset(space,L' ',1024);
										space[nIndent] = 0;   
										CString csLine=line;
										csLine.Trim();
										::wcscat(space,csLine);
										SendEditor(SCI_SETSEL,linepos,lineend);
										SendEditor(SCI_REPLACESEL, 0, (sptr_t)space);  
									}
								}
							}
						}   
					}

					LastProcessedChar = 0;   
					break;
				}
			case SCN_SAVEPOINTREACHED:
				{
					GetDocument()->SetModifiedFlag(false);
					//::AfxMessageBox(_T("SAVEPOINTREACHED"));
					//isDirty = false;
					//GetDocument()->SetModifiedFlag(false);
					//CheckMenus();
					break;
				}
				//���ĵ�״̬��Ϊmodifiedʱ�����ᴥ��SCN_SAVEPOINTLEFT�¼�֪ͨ
			case SCN_SAVEPOINTLEFT:
				{
					//::AfxMessageBox(_T("SAVEPOINTLEFT"));
					GetDocument()->SetModifiedFlag(true);
					//CheckMenus();
					//�����������޸ĵ��ļ��ı���,���ļ���������*
					CString csTitle=GetDocument()->GetPathName();
					CString csOldTitle=GetDocument()->GetTitle();
					int iLength = lstrlen(csTitle) ;
					int iIndex = csTitle.ReverseFind(L'\\') ;
					CString fileName = csTitle.Right(iLength - iIndex - 1); 
					fileName.AppendChar(L'*');
					if(csOldTitle!=fileName)
						GetDocument()->SetTitle(fileName);
					//isDirty = true;
					//GetDocument()->SetModifiedFlag(true);
					//CheckMenus();
					break;					
				}
			case SCN_MODIFIED:
				{
					if ( (((notify->modificationType & SC_MOD_DELETETEXT)==SC_MOD_DELETETEXT) ||
						((notify->modificationType & SC_MOD_INSERTTEXT)==SC_MOD_INSERTTEXT)) && notFirst)
					{
						int pos = SendEditor(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ��
						int line_number = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ���к�
						int linestartpos = SendEditor(SCI_POSITIONFROMLINE, line_number);   //��ǰ����ʼλ��
						int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, line_number); //��ǰ�н���λ��
						if(lineendpos==linestartpos)
							SendEditor(SCI_MARKERDELETE ,line_number,1);//ɾ���ϵ�

						NavigationPos();
					}
				}
				break;
			case SCN_NEEDSHOWN: 
				{
					int pos = m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ��
					const int line_number = m_ScintillaWnd.SendMessage(SCI_LINEFROMPOSITION,pos); //ȡ���к�
					int rets = m_ScintillaWnd.SendMessage(SCI_GETFOLDEXPANDED , line_number);
					if(!rets)
						m_ScintillaWnd.SendMessage(SCI_TOGGLEFOLD, line_number);
				}
			}
		 
		return CView::OnNotify(wParam, lParam, pResult);
	}


void CTCView::NavigationPos()
{
    int nowline = SendEditor(SCI_GETLINECOUNT); //���������
	int	d_valueline=nowline - m_rowCount;  //������ֵ
	m_rowCount = nowline;

	int nowallsize = SendEditor(SCI_GETLENGTH); //ȡ�õ�ǰ��������
	int d_valuepos = nowallsize - m_allSize;  //��������
	m_allSize = nowallsize;

	int pos = SendEditor(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ��
	int line_number = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ���к�

	int sizeLpos = m_markposLpos.size();
	int sizeRpos = m_markposRpos.size();
	int sizeL = m_markposL.size();
	int sizeR = m_markposR.size();
	int sizeLengthR = m_markLineLenthR.size();
	int sizeLengthL = m_markLineLenthL.size();

	int linestartpos = SendEditor(SCI_POSITIONFROMLINE, line_number);   //��ǰ����ʼλ��
	int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, line_number); //��ǰ�н���λ��
	int d_valuelinepos = pos -linestartpos;    //ÿ��λ�ò�ֵ
	int lienlength = lineendpos - linestartpos; //�г���

	if(sizeL > 0)
	{
		////////////////////����L��ֵ///////////////////////
		for(int i=0; i<sizeLpos; i++)
		{
			if(m_markposL.back()>line_number)
			{
				m_markposLTmp.push_back(m_markposL.back() + d_valueline);
			}
			else
			{
				m_markposLTmp.push_back(m_markposL.back());	
			}
			m_markposL.pop_back();
		}

		for(int i=0; i<sizeLpos; i++)  
		{
			m_markposL.push_back(m_markposLTmp.back()); //��������
			m_markposLTmp.pop_back();
		}
		////////////////////����L��ֵ///////////////////////

		if(m_markposL.back() != line_number) //�������ͬһ�� ����µ�λ��,�к�,�г���
		{
			m_markposL.push_back(line_number);
			m_markposLpos.push_back(d_valuelinepos);
			m_markLineLenthL.push_back(lienlength);
		}
		else                                //ͬһ��,����µ�λ��,�г���
		{
			m_markposLpos.pop_back();
			m_markposLpos.push_back(d_valuelinepos);
			m_markLineLenthL.pop_back();
			m_markLineLenthL.push_back(lienlength);
		}
	}
	else
	{	
		m_markposL.push_back(line_number);
		m_markposLpos.push_back(d_valuelinepos);
		m_markLineLenthL.push_back(lienlength);
	}

	if(sizeRpos > 0)
	{
		////////////////////����R��ֵ///////////////////////
		for(int i=0; i<sizeRpos; i++) 
		{
			if(m_markposR.back()>line_number)
			{
				m_markposRTmp.push_back(m_markposR.back() + d_valueline);
			}
			else
			{
				m_markposRTmp.push_back(m_markposR.back());	
			}
			m_markposR.pop_back();	
		}

		for(int i=0; i<sizeRpos; i++)  
		{
			m_markposR.push_back(m_markposRTmp.back()); //��������
			m_markposRTmp.pop_back();
		}
		////////////////////����R��ֵ///////////////////////
	}

	////////////////////������///////////////////////
	const int clearLen = 100;
	const int clearSize = 50;

	if(sizeLpos > clearLen)
	{
		m_markposLpos.erase(m_markposLpos.begin(),m_markposLpos.end() - clearSize);
	}
	if(sizeRpos > clearLen)
	{
		m_markposRpos.erase(m_markposRpos.begin(),m_markposRpos.end() - clearSize);
	}
	if(sizeL > clearLen)
	{
		m_markposL.erase(m_markposL.begin(),m_markposL.end() - clearSize);
	}
	if(sizeR > clearLen)
	{
		m_markposR.erase(m_markposR.begin(),m_markposR.end() - clearSize);
	}
	if(sizeLengthL > clearLen)
	{
		m_markLineLenthL.erase(m_markLineLenthL.begin(),m_markLineLenthL.end() - clearSize);
	}
	if(sizeLengthR > clearLen)
	{
		m_markLineLenthR.erase(m_markLineLenthR.begin(),m_markLineLenthR.end() - clearSize);
	}
	////////////////////������///////////////////////
}

void CTCView::OnUpdateTcTypesetting(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

}

bool CTCView::isFuntionStatement(CString statement)
{
	statement.Trim();
	if(::wcsncmp(statement,_T("����"),::wcslen(_T("����")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("����")));
		if(ch==' ')
			return true;
	}
	else if(::wcsncmp(statement,_T("function"),::wcslen(_T("function")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("function")));
		if(ch==' ')
			return true;
	}
	return false;
}
//�۵�/�� ������
void CTCView::FoldToDefine(bool isFold)//0�۵�,1��
{
	int pos = SendEditor(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ��
	//  ��ȡ�۵�״̬
	TCHAR buffer[1024]={L""};
	TCHAR temp[32]={0};
	int p[1024]={0};
	int end_line = SendEditor(SCI_GETLINECOUNT) - 1;
	int foldSize=0;//�����۵�����
	if(isFold)
	{
		for(int i = end_line; i>=0 ;i--)
		{
			wchar_t linecur[1024*2]={0}; //��ǰ������
			SendEditor(SCI_GETLINE, i  ,sptr_t(linecur)); //���ָ��������

			if(isFuntionStatement(linecur)) //�Ƿ���
			{
				int rets = SendEditor(SCI_GETFOLDEXPANDED , i);
				if(rets)
				{
					p[foldSize++]=i;
				}
			}
		}
	}
	else 
	{
		for(int i = end_line; i>=0 ;i--)
		{
			wchar_t linecur[1024*2]={0}; //��ǰ������
			SendEditor(SCI_GETLINE, i  ,sptr_t(linecur)); //���ָ��������

			if(isFuntionStatement(linecur)) //�Ƿ���
			{
				int rets = SendEditor(SCI_GETFOLDEXPANDED , i);
				if(!rets)
				{
					p[foldSize++]=i;
				}
			}
		}
	}
	if(foldSize>0)
	{
		SendEditor(SCI_LINESCROLL,0,p[0]);
		SendEditor(SCI_LINESCROLL,0,-2);
		SendEditor(SCI_TOGGLEFOLD, p[0]);

		for(int i=0; i<foldSize-1; i++)
		{
			//SendEditor(SCI_LINESCROLL,0,(p[i+1]-p[i]+2));
			//SendEditor(SCI_LINESCROLL,0,-2);
			SendEditor(SCI_TOGGLEFOLD, p[i+1]);
		}
	}

	SendEditor(SCI_GOTOLINE,pos); //λ�ø�λ
}

void CTCView::OnEditTogglefoldall()
{
	// TODO: �ڴ���������������
	
	if(m_isFold)
	{
		FoldToDefine(m_isFold);
		m_isFold=false;
	}
	else
	{
		FoldToDefine(m_isFold);
		m_isFold=true;
	}
}

//��ʾ���۵�
void CTCView::DisplayLine(int line)
{
	int isvisible = SendEditor(SCI_GETLINEVISIBLE ,line);//�ж��ǵ�ǰ���Ƿ���ʾ
	if(!isvisible)
	{
		int linetmp = line-1;
		//���ϱ���ֱ�������۵��д��۵���
		while(linetmp)
		{
			int rets = SendEditor(SCI_GETFOLDEXPANDED , linetmp);
			if(!rets)
			{
				SendEditor(SCI_TOGGLEFOLD, linetmp);
				break;
			}
			--linetmp;
		}
	}

}