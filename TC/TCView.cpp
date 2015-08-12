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
		ON_WM_KEYDOWN()
	END_MESSAGE_MAP()

	// CTCView ����/����

	CTCView::CTCView()
	{
		// TODO: �ڴ˴���ӹ������
		//m_isFirst=false;
		notFirst=false;
		m_rowCount=0;
		//m_allSize=0;
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

		m_listFaterView.Create(IDD_DIALOG6,&m_ScintillaWnd);
		m_listFaterView.m_listBox.m_pView = this;
		//m_listFaterView.m_listView.m_pView = this;

		m_ToolTip.Create(&m_ScintillaWnd);
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
			_T("var com array false true null ���� ��� ���� �� �� �� ")
			_T("namespace endnamespace function endfunction if else endif for endfor while endwhile select endselect case default return continue break goto \
			   �ռ� �ռ���� ���� ���ܽ��� ��� ���� ������� ���� �������� ѭ�� ѭ������ ѡ�� ���� Ĭ�� ѡ����� ���� ���� ���� ��ת ");

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
		LPCTSTR fontName = CTCRunTimeManager::g_EditorFontName;
		//����ȫ�ַ�� 
		SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT,(sptr_t)fontName);
		SendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT,CTCRunTimeManager::g_EditorFontSize); 
		SendEditor(SCI_STYLESETBOLD, STYLE_DEFAULT,CTCRunTimeManager::g_EditorFontBold);
		SendEditor(SCI_SETTABWIDTH, CTCRunTimeManager::g_EditorTabWidth);
		//SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT,(sptr_t)L"������"); 
		//SendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT,11); 
		//SendEditor(SCI_STYLECLEARALL); 

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
		//SendEditor(SCI_SETCARETLINEVISIBLE, TRUE);
		//SendEditor(SCI_SETCARETLINEBACK, 0xd0ffff);
		

		SendEditor(SCI_STYLESETBACK,STYLE_BRACELIGHT,0xFFD6AD); //���Ÿ�����ɫ

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
		////���°� �޸���ת��ʾ��ȫ 2012.12.3
		//int line_number = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ���к�
		//m_ScintillaWnd.SendMessage(SCI_GOTOLINE,line_number-5,(LPARAM)NULL);
		////���°� �޸���ת��ʾ��ȫ 2012.12.3
		//m_ScintillaWnd.SendMessage(SCI_GOTOPOS,pos,(LPARAM)NULL);
	}

	//��ת��ָ����
	void CTCView::TCGotoLine(int line)
	{
		m_ScintillaWnd.SendMessage(SCI_GOTOLINE,line-5,(LPARAM)NULL);
		m_ScintillaWnd.SendMessage(SCI_GOTOLINE,line,(LPARAM)NULL);

		//int pos = m_ScintillaWnd.SendMessage(SCI_POSITIONFROMLINE,line);
		//m_ScintillaWnd.SendMessage(SCI_GOTOPOS,pos,(LPARAM)NULL);
		 this->TCActiveEditView();
	}

	//�õ�ָ���е�λ��
	int CTCView::TCLineIndex(int line)
	{
		return SendEditor(SCI_POSITIONFROMLINE,line);
	}

	//���°� ��õ�ǰ�༭���������ı�
	void CTCView::GetUserEidtText(CString &rstr)
	{
		int sumLength = 0;
		sumLength = SendEditor(SCI_GETLENGTH)+1;
		if(sumLength>1)
		{
			SendEditor(SCI_GETTEXT,sumLength,(WPARAM)(rstr.GetBuffer(sumLength)));
			rstr.ReleaseBuffer(sumLength);
		}
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

		for( int size = 0; size < CTCRunTimeManager::g_TCAip.m_allApi.size(); size++ )
		{
			CString helpDllName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].name;
			for( int nspa = 0; nspa <  CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp.size();  nspa++ )
			{
				CString helpSPName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].namespaceName;
				if( 0 == helpSPName.CompareNoCase(namespDot) )
				{
					for( int func = 0; func < CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun.size(); func++ )
					{
						CString csFun = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun[ func ].funcName;
						csList+=csFun;
					    csList+=_T(" ");
					}
				}

			}
		}

		//TC4.0�Ķ�
		//const MyApiDllList* mydlllist=MyRuntimeManager::fApiFrame->getList();

		//const MyApiDll* helpdll=mydlllist->getHeaddllNode();

		//while(helpdll)
		//{
		//	const MyApiNamespaceList* helpnameSL;
		//	helpnameSL=helpdll->getNamespaceList();

		//	const MyApiNamespace* manSpace;
		//	manSpace=helpnameSL->getHeadNode();
		//	while(manSpace)   //  ����namespacelist
		//	{
		//		const MyApiFunctionList* mflist;			
		//		mflist=manSpace->getFunctionList();
		//		const MyApiFunction* mF;
		//		mF=mflist->getHeadNode();

		//		const XMLCh* nameStr=manSpace->getNameSpacename();

		//		if(XMLString::compareString(namespDot,nameStr)==0)
		//		{
		//			while(mF)
		//			{  
		//				const XMLCh* helpfunc=mF->getFuncionName();
		//				CString csFun=helpfunc;
		//				csList+=csFun;
		//				csList+=_T(" ");
		//				mF=mF->getNextNode();
		//			}
		//			hasData=true;
		//		}
		//		manSpace=manSpace->getNextNode();
		//	}
		//	helpdll=helpdll->getNextNode();
		//}
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
		
		//tc4.0 �Ķ�
		//CString example=MyRuntimeManager::fApiFrame->findFunExample(csNameSPDot,csFun);
		//if(example.IsEmpty())
		//	return false;

		CString example = CTCRunTimeManager::g_TCAip.GetExample(csNameSPDot,csFun);
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
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->RemoveToolTip(this);
		//���Position     
		CPoint caret_pos = GetCaretPos();
		ClientToScreen(&caret_pos);
		int LINE_HEIGHT = SendEditor(SCI_TEXTHEIGHT);
		RECT rct; //listλ�ô�С
		m_listFaterView.GetWindowRect(&rct);
		int x=0,y=0,width=0,height=0;
		x = caret_pos.x-10;
		y = caret_pos.y  + LINE_HEIGHT;
		width = rct.right-rct.left;
		height = rct.bottom-rct.top;

		//�����С
 		RECT windowRect;
		GetDesktopWindow()->GetWindowRect(&windowRect);
 
		int listwidth = x + width;
		int listHeight = y + height;
	
		//��ʾ������Ļ��ת����
		if(listwidth > windowRect.right) 
		{
			x = x - (listwidth - windowRect.right);
		}
		if(listHeight > windowRect.bottom)
		{
			y = y - height - LINE_HEIGHT;
		}

		//m_listFaterView.m_listView.ModifyStyle(0,/*LVS_SHOWSELALWAYS|*/LVS_SINGLESEL);	
		//m_listFaterView.m_listView.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

		m_listFaterView.m_listBox.SetCurSel(0);
		m_listFaterView.MoveWindow(x,y,width,height);
		m_listFaterView.m_listBox.ShowWindow();
		ShowIntelisenseFunExample();
	}

	void CTCView::hideIntellisenseWindow()
	{ 
		//m_listFaterView.m_listView.HideWindow();
		m_listFaterView.m_listBox.HideWindow();
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RemoveTool(this);
	}
bool CTCView::LfetIfNote(int nowpos)
{
	int linecur = SendEditor(SCI_LINEFROMPOSITION,nowpos); //ȡ�õ�ǰ��
	int linestartpos = SendEditor(SCI_POSITIONFROMLINE, linecur);   //��ǰ����ʼλ��

	for(int i=linestartpos; i<nowpos-1; i++)
	{
		TCHAR ch1 = SendEditor(SCI_GETCHARAT, i);
		TCHAR ch2 = SendEditor(SCI_GETCHARAT, i+1);
		if(ch1==L'/' && ch2==L'/')
			return true;
	}
	return false;

}
	BOOL CTCView::PreTranslateMessage(MSG* pMsg)
	{
		// TODO: �ڴ����ר�ô����/����û���

		if( m_ToolTip.RelayEvent(pMsg) )
			return TRUE;

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RelayEvent(pMsg);

		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam != VK_BACK)
				StatubarsShowRowAndCul();
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == L'C' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{
			m_ScintillaWnd.SendMessage(SCI_COPY);
			return true;
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == L'X' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{

			m_ScintillaWnd.SendMessage(SCI_CUT);
			return true;
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == L'V' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{ 

			m_ScintillaWnd.SendMessage(SCI_PASTE);
			return true;
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == L'A' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{
			m_ScintillaWnd.SendMessage(SCI_SELECTALL);
			return true;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'Z' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{
			m_ScintillaWnd.SendMessage(SCI_UNDO);
			return true;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'G' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0)  /*&& (VK_LEFT && ::GetKeyState(VK_CONTROL)<0)*/)
		{
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->OnLineJumper();
			/*TCHAR buff[1024] = {0};
			if( FindAllContent(buff,0) )
			{
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_findInfo.clear();
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCFindAllContent();
			}*/
			return true;
		}

		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'F' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
		{
			OnEditFind();
			return true;
		}
		if(pMsg->message == WM_LBUTTONUP )
		{
			StatubarsShowRowAndCul();
			//�������ʱ������ʾ����
			hideIntellisenseWindow();

						//���Ÿ��� ���°� 2012.12.22
			int s1=SendEditor(SCI_GETSELECTIONSTART); //�������λ��
			int s2=SendEditor(SCI_GETSELECTIONEND); //�������λ��
			if(s1==s2)
				braceMatchHighlight();
			else
			{
				SendEditor(SCI_BRACEBADLIGHT,-1); //ȡ��֮ǰ����
			}

			////���Ÿ��� ���°� 2012.12.22
			//braceMatchHighlight();
		}
		if(pMsg->message == WM_MOUSEMOVE)
		{
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipExDebug.RemoveTool(&m_ScintillaWnd);
			m_ToolTip.RemoveAllTools();

			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->RemoveToolTip(&m_ScintillaWnd);
			static bool isMouseTip=false;
			wchar_t line[1024]={0}; //���浱ǰ����µĵ��� 
			POINT point=pMsg->pt;
			m_ScintillaWnd.ScreenToClient(&point);
			int pos=SendEditor(SCI_POSITIONFROMPOINT,point.x,point.y);  

            //��ֹ�༭���泬��1024���� 
			int linecur = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ�õ�ǰ�� 	
			int linepos = SendEditor(SCI_POSITIONFROMLINE,linecur);   //��ǰ����ʼλ�� 
			int lineend = SendEditor(SCI_GETLINEENDPOSITION,linecur); //��ǰ�н���λ��
			if((lineend - linepos)>1024)
				return false;

			int line_number = SendEditor(SCI_LINEFROMPOSITION,pos); 
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
				//�������������� �ϵ����ʱ,������ڱ�����ֵ
				CString csVarValue;
				if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCIsDebugValue(csNSFUN,csVarValue))
				{
					//AddToolTip(csVarValue);
					((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipExDebug.AddTool(&m_ScintillaWnd, csVarValue, NULL);
					//((CMainFrame*)AfxGetApp()->m_pMainWnd)->GiveToolTip(&m_ScintillaWnd, csVarValue, NULL);
					//return true;
				}
				int dotIndex=csNSFUN.Find(_T('.'));
				if(dotIndex!=-1)
				{
					CString namsSP=csNSFUN.Mid(0,dotIndex);
					CString nameFun=csNSFUN.Mid(dotIndex+1,csNSFUN.GetLength()-dotIndex);

					CString example = CTCRunTimeManager::g_TCAip.GetExample(namsSP,nameFun);
					if(!example.IsEmpty())
					{
						isMouseTip=true;
						//((CMainFrame*)AfxGetApp()->m_pMainWnd)->GiveToolTip(&m_ScintillaWnd, example, NULL);
						//CString testTemp;
						//testTemp=_T("<h1>")+example+_T("<\h1>");
						example.Replace(_T("|"),_T(""));
						//example=_T("<h4>")+example+_T("<\h4>");
						AddToolTip(example);
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
		//�������ESC��,�˳���ʾ
		if( pMsg->wParam == VK_ESCAPE )
		{
			hideIntellisenseWindow();
		}

		if( pMsg->wParam == VK_BACK )
		{
			//hideIntellisenseWindow();
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
					//if(m_listFaterView.m_listView.doFilter(pToShow,nStart,nEnd))
					if(m_listFaterView.m_listBox.doFilter(pToShow,nStart,nEnd))
					{
						//����������ʾ
						if(!LfetIfNote(pos))
							showIntellisenseWindow();
						//return true;
					}
					else
						hideIntellisenseWindow();
				}
				else
					hideIntellisenseWindow();
			}
			else
				hideIntellisenseWindow();

			delete [] lineBuf;
		}

		if( ( (pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_TAB) )&& m_listFaterView.m_listBox.m_isShow/*m_listFaterView.m_listView.m_isShow*/)
		{
			//����������ʾʱ,��һ�λس�,TAB
			m_listFaterView.m_listBox.SendMessage(WM_KEYDOWN,VK_RETURN);
			return true;
		}
		if(pMsg->wParam == VK_DOWN || pMsg->wParam == VK_UP)
		{
			StatubarsShowRowAndCul();
			if(m_listFaterView.m_listBox.m_isShow && pMsg->message == WM_KEYDOWN)
			{
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->RemoveToolTip(this);

				int listIndex = 0;
				if( m_listFaterView.m_listBox.GetCurSel() > 0 )
				{
					listIndex = m_listFaterView.m_listBox.GetCurSel();
				}

				if(VK_UP == pMsg->wParam)
				{
					if( listIndex >= 1 )
						listIndex -= 1;
				}
				else if(VK_DOWN == pMsg->wParam)
				{
					listIndex += 1;
				}
				m_listFaterView.m_listBox.SetCurSel(listIndex);

				ShowIntelisenseFunExample();

				((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RelayEvent(pMsg);
				return true;
			}
			else if(m_listFaterView.m_listBox.m_isShow)
			{
				return true;
			}

		}

		if( pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT )
			StatubarsShowRowAndCul();

		if(pMsg->wParam == VK_SHIFT)
			hideIntellisenseWindow();

		if(pMsg->message == WM_KEYUP &&( (pMsg->wParam == VK_LEFT)|| (pMsg->wParam == VK_UP) || (pMsg->wParam == VK_RIGHT) || (pMsg->wParam == VK_DOWN)))
		{
			//���Ÿ��� ���°� 2012.12.22
			int s1=SendEditor(SCI_GETSELECTIONSTART); //�������λ��
			int s2=SendEditor(SCI_GETSELECTIONEND); //�������λ��
			if(s1==s2)
				braceMatchHighlight();
			else
			{
				SendEditor(SCI_BRACEBADLIGHT,-1); //ȡ��֮ǰ����
			}
		}
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

	void CTCView::TCContentText(int beginPos,int endPos)
	{
		SendEditor(SCI_SETSEL,beginPos,endPos);
	}

	void CTCView::TCReplaceSelText(CString old,CString toReplace,int flag,bool fromBegin)
	{
		//����Ǵ�ͷ��ʼ�滻������ȫ���滻��
		if ( fromBegin )
		{
			TextToFind ttf;

			ttf.chrg.cpMin = 0;

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
		
		//m_allSize = SendEditor(SCI_GETLENGTH); //����ܴ�С
		m_rowCount = SendEditor(SCI_GETLINECOUNT); //���������
		if(!notFirst)
			notFirst = true;
	}


	void CTCView::OnDestroy()
	{
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_TipEx.RemoveTool(this);
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
	    if(ptr==0)
			return 0;

		while(*ptr == L' ' || *ptr == L'\t')
			ptr++;
		return ptr;
	}
	//���Ÿ��� ���°� 2012.12.22
	void CTCView::braceMatchHighlight(int Dvalue)
	{
		int ret1 = SendEditor(SCI_BRACEBADLIGHT,-1); //ȡ��֮ǰ����
		int pos = SendEditor(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ�� ����ƫ�Ʋ�ֵ
 		int linecur = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ�õ�ǰ��
 		int linestartpos = SendEditor(SCI_POSITIONFROMLINE, linecur);   //��ǰ����ʼλ��
 		int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, linecur); //��ǰ�н���λ��

		if(pos>0)
		{
			pos = pos - Dvalue;
		}

		wchar_t linepre[1024*16]={0};
		SendEditor(SCI_GETLINE,linecur,sptr_t(linepre)); //���ָ��������
		int len = wcslen(linepre);

		int leftPos = pos - linestartpos; //pos���
		int rightPos = lineendpos - pos ; //pos�Ҷ�

		int lBrace=-1; //������λ��
		int rBrace=-1; //�ҿں�λ��

		int leverBrace = 0; //���Ų���

		int type  = 0;  //�����������

		for(int i=leftPos;i<len;i++)
		{
			if((linepre[i]==L'(') && (!IsNoteOrStr(linepre,i)))
			{
				if(i!=leftPos)
				{
					type = 1; //�������
					break;
				}
				leverBrace++;
				lBrace = i;
				for(int j=lBrace+1;j<len;j++)
				{
					if((linepre[j]==L'(') && (!IsNoteOrStr(linepre,j)))
					{
						leverBrace++;
					}
					else if((linepre[j]==L')') && (!IsNoteOrStr(linepre,j)))
					{
						leverBrace--;
						if(leverBrace==0)
						{
							rBrace = j;
							break;
						}
					}
				}
			}
			else if((linepre[i]==L')') && (!IsNoteOrStr(linepre,i)))
			{
				leverBrace++;
				rBrace = i;

				for(int j=rBrace-1;j>=0;j--)
				{
					if((linepre[j]==L')') && (!IsNoteOrStr(linepre,j)))
					{
						leverBrace++;
					}
					else if((linepre[j]==L'(') && (!IsNoteOrStr(linepre,j)))
					{
						leverBrace--;
						if(leverBrace==0)
						{
							lBrace = j;
							break;
						}
					}
				}
				break;
			}
		}
		if(type)
		{
			leverBrace = 0;
			for(int i=leftPos;i>0;i--)
			{
				 if((linepre[i]==L')') && (!IsNoteOrStr(linepre,i)))
				 {
					 leverBrace--;
				 }
				 else if((linepre[i]==L'(') && (!IsNoteOrStr(linepre,i)))
				 {
					 if(leverBrace == 0)
					 {
						 leverBrace ++;
						 lBrace = i;
						 break;
					 }
					 else
					 {
						 leverBrace++;
					 }
				 }
			}

			for(int i=leftPos;i<len;i++)
			{
				if((linepre[i]==L')') && (!IsNoteOrStr(linepre,i)))
				{
					leverBrace--;
					if(leverBrace == 0)
					{
						rBrace = i;
						break;
					}
				}
				else if((linepre[i]==L'(') && (!IsNoteOrStr(linepre,i)))
				{
					leverBrace++;
				}
			}

		}
		if((lBrace >= 0) && (rBrace>lBrace))
		{
			//int ret2 = SendEditor(SCI_STYLESETBOLD,STYLE_BRACELIGHT,true); //���ô��� ��Ч
			//int start = SendEditor(SCI_BRACEMATCH,pos,0); //ƥ��֧�� ���󲻺���
			//SendEditor(SCI_STYLESETBACK,STYLE_BRACELIGHT,0xC0C0C0); //���Ÿ�����ɫ
			SendEditor(SCI_BRACEHIGHLIGHT,lBrace+linestartpos,rBrace+linestartpos); //���Ÿ���
		}
	}

	//ѡ���Ű�,GOTO������⴦�� 2012.12.21
	bool CTCView::isGoToStr(TCHAR* soucestr)
	{
		CString str = soucestr;
		str.Trim();
		int len = str.GetLength();
		int findret = str.Find(':');//��һ��ð��
		bool friStspace = false;  //���⴦��

		if(findret>0) //�ҵ�ð�ſ�ʼ����
		{
			if(!((str[0]>='A' && str[0]<='Z')||(str[0]>='a' && str[0]<='z')||(str[0]>='a' && str[0]<='z')||(str[0]>0x4E00 && str[0]<0x9FBF)||(str[0]=='_'))) //���ַ�����������,��ĸ,�»���
			{
				return false;
			}
			for(int i=1;i<len;i++)
			{
				if(!friStspace)
				{
					if(!((str[i]>='A' && str[i]<='Z')||(str[i]>='a' && str[i]<='z')||(str[i]>='a' && str[i]<='z')||(str[i]>0x4E00 && str[i]<0x9FBF)||(str[i]>='0' && str[i]<='9')||(str[i]=='_')))
					{
						if(str[i]==':')
						{
							CString tempKey=str;
							tempKey.Delete(tempKey.GetLength()-1);
							tempKey.Trim();
							if(tempKey==L"default"||tempKey==L"Ĭ��")
								return false;
							return true;
						}
						else if(str[i]==' ')
						{
							friStspace = true;
							continue;
						}
						return false;
					}
				}
				else
				{
					if(str[i]==':')
					{
						CString tempKey=str;
						tempKey.Delete(tempKey.GetLength()-1);
						tempKey.Trim();
						if(tempKey==L"default"||tempKey==L"Ĭ��")
							return false;
						return true;
					}
					else if(str[i]==' ')
					{
						continue;
					}
					else
						return false;
				}
			}
		}
		return false;
	}		

	void CTCView::TCTypesetting()
	{
		int start = SendEditor(SCI_GETSELECTIONSTART); //���ѡ�п�ʼλ��
		int end = SendEditor(SCI_GETSELECTIONEND);	    //���ѡ�н���λ��
		int line_start = SendEditor(SCI_LINEFROMPOSITION,start); //ѡ�п�ʼ��
		int line_end = SendEditor(SCI_LINEFROMPOSITION,end);     //ѡ�н�����
		wchar_t space[1024*16]={L""};   //����ո�
		CString replaceTemp;  //�����滻����
		int Lever = 0;
		bool iskeywords =false;
		for(int i = line_start; i <= line_end ; i++ )
		{
			wchar_t linepre[1024*16]={0}; //��һ������
			wchar_t linecur[1024*16]={0}; //��ǰ������
			::wmemset(space,L' ',1024*16);

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
				////ѡ���Ű�GOTO������⴦�� ���°� 2012.12.20
				if(isGoToStr(space))
				{
					wcscpy(space , TrimL(space));
				}
				////ѡ���Ű�GOTO������⴦�� ���°� 2012.12.20
				replaceTemp += space;
			}
			else
			{
				if(iskeywords&&Lever>0) //�����ؼ���
					Lever--;

				space[Lever*4] = 0;
				::wcscat(space,linecur);

				////ѡ���Ű�GOTO������⴦�� ���°� 2012.12.20
				if(isGoToStr(space))
				{
					wcscpy(space , TrimL(space));
				}
				////ѡ���Ű�GOTO������⴦�� ���°� 2012.12.20

				replaceTemp += space;
				iskeywords = false;
			}	
		}

		int linestartpos = SendEditor(SCI_POSITIONFROMLINE, line_start);   //��ǰ����ʼλ��
		int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, line_end); //��ǰ����ʼλ��

		if(linestartpos < lineendpos) //�������ʱѡ���Ű���һ��
		{
			int len = replaceTemp.GetLength();

			if(len >= 2 )
			{
				if(replaceTemp[len-2]==L'\r' && replaceTemp[len-1]==L'\n') //ȥ��β�����з�
					replaceTemp = replaceTemp.Left(len-2);	
				else if(replaceTemp[len-1]==L'\n')
					replaceTemp = replaceTemp.Left(len-1);
			}

			SendEditor(SCI_SETSEL, linestartpos, lineendpos); //�Զ���ѡ��λ��
			SendEditor(SCI_REPLACESEL, 0, (sptr_t)replaceTemp.LockBuffer());
			replaceTemp.UnlockBuffer();
		}
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
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

		if(!pFrame->m_markposL.empty() && !pFrame->m_markposLpos.empty())
		{
			if( (pFrame->m_markposL.size() > 1) && (pFrame->m_markposLpos.size() > 1) && (pFrame->m_markLorR == 2) ) //����һ�ε���,���ϵ㷴�򵼺�ȡ����
			{
				pFrame->m_markposR.push_back(pFrame->m_markposL.back());
				pFrame->m_markposRpos.push_back(pFrame->m_markposLpos.back());
				pFrame->m_markLineLenthR.push_back(pFrame->m_markLineLenthL.back()); //����µ��г���
				pFrame->m_markRpath.push_back(pFrame->m_markLpath.back());

				pFrame->m_markposL.pop_back();
				pFrame->m_markposLpos.pop_back();
				pFrame->m_markLineLenthL.pop_back();
				pFrame->m_markLpath.pop_back();
			}

			int nowline = pFrame->m_markposL.back();
			DisplayLine(nowline);
			int linestartpos = SendEditor(SCI_POSITIONFROMLINE, nowline);   //��ǰ����ʼλ��
			int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, nowline); //��ǰ�н���λ��
			int lienlength = lineendpos - linestartpos;  //�µ��г���
			int oldlinelength = pFrame->m_markLineLenthL.back(); //�ϵ��г���
			int d_valuelinepos =  lienlength - oldlinelength;   //ÿ�г��Ȳ�ֵ
			int nowpos = pFrame->m_markposLpos.back();
			wstring path;
			path = pFrame->m_markLpath.back();
			CTCView *pView=NULL;
			if(PathFileExists(path.c_str()))
				pView=(CTCView*)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(path.c_str());

			if(pView!=NULL)
			{
				int setpos = nowpos + linestartpos + d_valuelinepos +1;
				TCHAR poschar = SendEditor(SCI_GETCHARAT, setpos );

				SendEditor(SCI_GRABFOCUS);
				SendEditor(SCI_GOTOPOS,setpos);
				pFrame->m_markposR.push_back(nowline);
				pFrame->m_markposRpos.push_back(nowpos);
				pFrame->m_markLineLenthR.push_back(oldlinelength); //����µ��г���
				pFrame->m_markRpath.push_back(pFrame->m_markLpath.back());

				pFrame->m_markposL.pop_back();
				pFrame->m_markposLpos.pop_back();
				pFrame->m_markLineLenthL.pop_back();
				pFrame->m_markLpath.pop_back();
			}
			else//�ļ�������ɾ���ڵ�
			{
				for(int i=0; i<pFrame->m_markposR.size(); i++)
				{
					if((path.compare(pFrame->m_markRpath[i])==0))//ͬһ���ļ��Ŵ����ֵ
					{
						pFrame->m_markposRpos.erase(pFrame->m_markposRpos.begin() + i);
						pFrame->m_markposR.erase(pFrame->m_markposR.begin() + i);
						pFrame->m_markLineLenthR.erase(pFrame->m_markLineLenthR.begin() + i);
						pFrame->m_markRpath.erase(pFrame->m_markRpath.begin() + i);
					}
				}

				for(int i=0; i<pFrame->m_markposL.size(); i++)
				{
					if((path.compare(pFrame->m_markLpath[i])==0))//ͬһ���ļ��Ŵ����ֵ
					{
						pFrame->m_markposLpos.erase(pFrame->m_markposLpos.begin() + i);
						pFrame->m_markposL.erase(pFrame->m_markposL.begin() + i);
						pFrame->m_markLineLenthL.erase(pFrame->m_markLineLenthL.begin() + i);
						pFrame->m_markLpath.erase(pFrame->m_markLpath.begin() + i);
					}
				}
			}
			pFrame->m_markLorR = 1;
		}
	}

	void CTCView::OnUpdateEditGoback(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

		if(!pFrame->m_markposL.empty() && !pFrame->m_markposLpos.empty())
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
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

		if(!pFrame->m_markposR.empty() && !pFrame->m_markposRpos.empty())
		{
			if( (pFrame->m_markposR.size() > 1) && (pFrame->m_markposRpos.size() > 1) && (pFrame->m_markLorR == 1) ) //����һ�ε���,���ϵ㷴�򵼺�ȡ����
			{
				pFrame->m_markposL.push_back(pFrame->m_markposR.back());
				pFrame->m_markposLpos.push_back(pFrame->m_markposRpos.back());
				pFrame->m_markLineLenthL.push_back(pFrame->m_markLineLenthR.back()); //����µ��г���
				pFrame->m_markLpath.push_back(pFrame->m_markRpath.back());

				pFrame->m_markposR.pop_back();
				pFrame->m_markposRpos.pop_back();
				pFrame->m_markLineLenthR.pop_back();
				pFrame->m_markRpath.pop_back();
			}

			int nowline = pFrame->m_markposR.back();
			DisplayLine(nowline);
			int linestartpos = SendEditor(SCI_POSITIONFROMLINE, nowline);   //��ǰ����ʼλ��
			int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, nowline); //��ǰ�н���λ��
			int lienlength = lineendpos - linestartpos;  //�µ��г���
			int oldlinelength = pFrame->m_markLineLenthR.back(); //�ϵ��г���
			int d_valuelinepos =  lienlength - oldlinelength;   //ÿ�г��Ȳ�ֵ	
			int nowpos = pFrame->m_markposRpos.back();
			wstring path;
			path = pFrame->m_markRpath.back();
			CTCView *pView=NULL;
			if(PathFileExists(path.c_str()))
				pView=(CTCView*)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(path.c_str());

			if(pView!=NULL)
			{
				int setpos = nowpos + linestartpos + d_valuelinepos +1;
				TCHAR poschar = SendEditor(SCI_GETCHARAT, setpos );

				SendEditor(SCI_GRABFOCUS);
				SendEditor(SCI_GOTOPOS,setpos);
				pFrame->m_markposL.push_back(nowline);
				pFrame->m_markposLpos.push_back(nowpos);
				pFrame->m_markLineLenthL.push_back(oldlinelength); //����µ��г���
				pFrame->m_markLpath.push_back(pFrame->m_markRpath.back());

				pFrame->m_markposR.pop_back();
				pFrame->m_markposRpos.pop_back();
				pFrame->m_markLineLenthR.pop_back();
				pFrame->m_markRpath.pop_back();
			}
			else//�ļ�������ɾ���ڵ�
			{
				for(int i=0; i<pFrame->m_markposR.size(); i++)
				{
					if((path.compare(pFrame->m_markRpath[i])==0))//ͬһ���ļ��Ŵ����ֵ
					{
						pFrame->m_markposRpos.erase(pFrame->m_markposRpos.begin() + i);
						pFrame->m_markposR.erase(pFrame->m_markposR.begin() + i);
						pFrame->m_markLineLenthR.erase(pFrame->m_markLineLenthR.begin() + i);
						pFrame->m_markRpath.erase(pFrame->m_markRpath.begin() + i);
					}
				}

				for(int i=0; i<pFrame->m_markposL.size(); i++)
				{
					if((path.compare(pFrame->m_markLpath[i])==0))//ͬһ���ļ��Ŵ����ֵ
					{
						pFrame->m_markposLpos.erase(pFrame->m_markposLpos.begin() + i);
						pFrame->m_markposL.erase(pFrame->m_markposL.begin() + i);
						pFrame->m_markLineLenthL.erase(pFrame->m_markLineLenthL.begin() + i);
						pFrame->m_markLpath.erase(pFrame->m_markLpath.begin() + i);
					}
				}
			}
			pFrame->m_markLorR = 2;
		}
	}


	void CTCView::OnUpdateEditGoforward(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

		if(!pFrame->m_markposR.empty() && !pFrame->m_markposRpos.empty())
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
					int pos = m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ�� 

					//���ı���滻Ӣ�ı�� ���°� 2013.1.18
					if(CTCRunTimeManager::g_EditorSignReplace)
					{
						notify->ch = replaceSign(notify->ch);
						TCHAR chtmp[2]={0};
						chtmp[0]=notify->ch;
						SendEditor(SCI_SETSEL,pos-1,pos);
						SendEditor(SCI_REPLACESEL, 0, (sptr_t)chtmp);  
					}
				//�����Զ�����
				LastProcessedChar = notify->ch;  

								//���Ÿ��� ���°� 2012.12.22
				int s1=SendEditor(SCI_GETSELECTIONSTART); //�������λ��
				int s2=SendEditor(SCI_GETSELECTIONEND); //�������λ��
				if(notify->ch == L')')
				{
					if(s1==s2)
					{
						braceMatchHighlight(1);
					}
					else
					{
						SendEditor(SCI_BRACEBADLIGHT,-1); //ȡ��֮ǰ����
					}
				}
				else
				{
					if(s1==s2)
					{
						braceMatchHighlight();
					}
					else
					{
						SendEditor(SCI_BRACEBADLIGHT,-1); //ȡ��֮ǰ����
					}
				}

				////���Ÿ��� ���°� 2012.12.22
				//if(notify->ch == L')')
				//{
				//	braceMatchHighlight(1);
				//}
				//else
				//{
				//	braceMatchHighlight();
				//}
				//hideIntellisenseWindow();
				//�����кź��к�
				StatubarsShowRowAndCul();

					//GOTO �������⴦�� ���°� 2012.12.25
					if(notify->ch == L':')
					{
						int linecur = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ�õ�ǰ��
						int linestartpos = SendEditor(SCI_POSITIONFROMLINE, linecur);   //��ǰ����ʼλ��
						int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, linecur); //��ǰ�н���λ��
					    wchar_t linecurstr[1024*16]={0}; //��ǰ������
				    	wchar_t linecurtmp[1024*16]={0}; //��ǰ������
						SendEditor(SCI_GETLINE,linecur ,sptr_t(linecurstr)); //���ָ��������

					if(isGoToStr(linecurstr))
					{
						int nIndent = 0;
						for(int i=1;(linecur-i)>0;i++) //���ϱ���ֱ���ҵ�����GOTO����������Ϊֹ,��¼����
						{
							memset(linecurtmp,0,sizeof(linecurtmp));
							m_ScintillaWnd.SendMessage(SCI_GETLINE,linecur-i,sptr_t(linecurtmp));
							if(!isGoToStr(linecurtmp))
							{
								nIndent = SendEditor(SCI_GETLINEINDENTATION,linecur-i);
								break;
							}
						}
						wchar_t space[1024];   
						::wmemset(space,L' ',1024);
						space[nIndent] = 0;
						wcscpy(linecurstr , TrimL(linecurstr));
						wcscat(linecurstr,space);
						SendEditor(SCI_SETSEL,linestartpos,lineendpos);
						SendEditor(SCI_REPLACESEL, 0, (sptr_t)linecurstr);  
					}

				}
				//GOTO �������⴦�� ���°� 2012.12.25
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
						//if(m_listFaterView.m_listView.doFilter(pToShow,nStart,nEnd))
						if(m_listFaterView.m_listBox.doFilter(pToShow,nStart,nEnd))
						{
							//����������ʾ
							if(!LfetIfNote(pos))
								showIntellisenseWindow();
						}
						else
							hideIntellisenseWindow();
					}
					else
						hideIntellisenseWindow();
				}
				else
					hideIntellisenseWindow();

				delete [] lineBuf;

					// ������ʾ���� 
					static const wchar_t* pCallTipNextWord = NULL;//��һ������λ�� 
					static const wchar_t* pCallTipCurDesc = NULL;//��ǰ��ʾ�ĺ�����Ϣ 
					if(notify->ch == L'(') //��������������ţ���ʾ������ʾ 
					{ 	
					wchar_t word[1000]={0}; //���浱ǰ����µĵ��� 
					wchar_t wordPre[1000]={0}; //���浱ǰ����µĵ��� 
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

					CString csParam;
					//csParam.Empty();
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
// 				else if(notify->ch == L',' && m_ScintillaWnd.SendMessage(SCI_CALLTIPACTIVE) && pCallTipCurDesc) 
// 				{
// 					if(singleParam)
// 					{
// 						m_ScintillaWnd.SendMessage(SCI_CALLTIPCANCEL); 
// 						singleParam = false;
// 					}
// 					else
// 					{
// 						//������Ƕ��ţ�������һ������ 
// 						const wchar_t *pStart = pCallTipNextWord; 
// 						const wchar_t *pEnd = wcschr(pStart,L','); 
// 						// 			pStart = pCallTipNextWord; 
// 						// 			pEnd = strchr(pStart,','); 
// 						if(pEnd == NULL) pEnd = wcschr(pStart,L')'); 
// 						if(pEnd == NULL) //û����һ�����������ر���ʾ 
// 							m_ScintillaWnd.SendMessage(SCI_CALLTIPCANCEL); 
// 						else 
// 						{ 
// 							m_ScintillaWnd.SendMessage(SCI_CALLTIPSETHLT,pStart-pCallTipCurDesc, pEnd-pCallTipCurDesc); 
// 							pCallTipNextWord = pEnd+1; 
// 						}
// 					}
// 				}
				else if(notify->ch == L',' /*&& !(SendEditor(SCI_CALLTIPACTIVE))*/ )
				{
					int linecur = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ�õ�ǰ��
					int linestartpos = SendEditor(SCI_POSITIONFROMLINE, linecur);   //��ǰ����ʼλ��
					int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, linecur); //��ǰ�н���λ��
					//SendEditor(SCI_GETLINE, linecur  ,sptr_t(buff)); //���ָ��������
					int commaCount=0; //���Ÿ���
					int rightBracketCount=0;
					bool isfunbracket=false; //���������Ų�ֵ��ֵ fun1(fun2(,,),) fun2�����Ų����Ӷ�������

					for(int ii=pos; ii > linestartpos; ii--)
					{
						TCHAR tmp = SendEditor(SCI_GETCHARAT, ii );
						
						if(tmp==L')' && ii != pos) //��ú������� ii!=pos �Ǵ���functin(,)�������
						{
							rightBracketCount--;
						}
						if(tmp==L'(') //����߱�������
						{
							rightBracketCount++;

							if(rightBracketCount>0)//������Ӧ
							{
								for(int j=pos; j>ii; j--)//��ö��Ÿ���
								{	
									tmp = SendEditor(SCI_GETCHARAT, j );

									if(tmp==L')' && j != pos) //Ϊ�˷�ֹ�����������ͬ����,���������2��
										isfunbracket=true;

									if(tmp==L'(')
										isfunbracket=false;

									if(tmp==L','&& j != pos)
									{
										if(!isfunbracket)
										{
											commaCount++;
										}
									}
								}
								if(commaCount > 0)//���Ŵ���0 ������ʾ
								{								
									static CString csParam1; //�洢����
									wchar_t word[1000]={0}; //���浱ǰ����µĵ��� 
									wchar_t wordPre[1000]={0}; //���浱ǰ����µĵ��� 
									TextRange tr;    //����SCI_GETTEXTRANGE���� 

									int startpos = m_ScintillaWnd.SendMessage(SCI_WORDSTARTPOSITION,ii);//��ǰ������ʼλ�� 
									int endpos = m_ScintillaWnd.SendMessage(SCI_WORDENDPOSITION,ii);//��ǰ������ֹλ�� 

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

									int wordlen = wcslen(word) - 1;
									if(word[wordlen]!=L'(') //������һ���ַ����������Ž���ѭ��
									{
										for(int i=wordlen; i>0; i--)
										{
											if(word[i]==L'(')
											{
												word[i+1]=0; //�ض������ź��������
												break;
											}
										}
									}

									if(TCGetFunParam(wordPre,word,csParam1))
									{
										pCallTipNextWord=NULL;
										TCHAR endtmp = csParam1.GetAt(csParam1.GetLength()-2);
										if(endtmp == L'(') //����޲���ֻ��ʾ������
										{
											//�ҵ�������ô��ʾ��ʾ�� 
											pCallTipCurDesc = csParam1; //��ǰ��ʾ�ĺ�����Ϣ 
											m_ScintillaWnd.SendMessage(SCI_CALLTIPSHOW,ii,sptr_t(pCallTipCurDesc));//��ʾ�����ʾ
											//singleParam = true;
											break;
										}
										else
										{
											int paramLength = csParam1.GetLength();
											int paramCout= 1 ;//��������,1�����ŵ���2������
											for(int i=0; i <paramLength; i++)
											{
												if(csParam1[i]==L',')
													paramCout++;
											}
											if(commaCount >= paramCout) // �����������ڲ�����������ʾ,�޲����������ܴ�����
												break;

											//�ҵ�������ô��ʾ��ʾ�� 
											pCallTipCurDesc = csParam1; //��ǰ��ʾ�ĺ�����Ϣ 
											m_ScintillaWnd.SendMessage(SCI_CALLTIPSHOW,ii,sptr_t(pCallTipCurDesc));//��ʾ�����ʾ 
											const wchar_t *pStart = wcschr(pCallTipCurDesc,L'(')+1; //������һ������ 
											const wchar_t *pEnd = wcschr(pStart,L',');//�����б��Զ��ŷָ� 
											if(pEnd == NULL) pEnd = wcschr(pStart,L')');//�������һ��������������������

											m_ScintillaWnd.SendMessage(SCI_CALLTIPSETHLT, 
												pStart-pCallTipCurDesc, pEnd-pCallTipCurDesc);
											pCallTipNextWord = pEnd+1;//ָ����һ����λ��,����һ��λ��

											for(int i=0;i<commaCount;i++)//�м���������ʾλ�þ�������
											{
												//������Ƕ��ţ�������һ������ 
												const wchar_t *pStart1 = pCallTipNextWord; 
												const wchar_t *pEnd1 = wcschr(pStart1,L','); 
												if(pEnd1 == NULL) pEnd1 = wcschr(pStart1,L')'); 
												if(pEnd1 == NULL) //û����һ�����������ر���ʾ 
													m_ScintillaWnd.SendMessage(SCI_CALLTIPCANCEL); 
												else 
												{ 
													m_ScintillaWnd.SendMessage(SCI_CALLTIPSETHLT,pStart1-pCallTipCurDesc, pEnd1-pCallTipCurDesc); 
													pCallTipNextWord = pEnd1+1; 
												}
											}
											break; 
										}
									}
									else
										break;
								}
							}
						}
					}
				}

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
 						int linepos = SendEditor(SCI_POSITIONFROMLINE,linecur);   //��ǰ����ʼλ��   
 						int lineend = SendEditor(SCI_GETLINEENDPOSITION,linecur); //��ǰ�н���λ��
					    //��ֹ�༭���泬��1024���� 
						if((lineend - linepos)>1024)
							return false;

						wchar_t line[1024]={0}; 
						//m_ScintillaWnd.SendMessage(SCI_GETCURLINE,1000,sptr_t(line));
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
							   //��ֹ�༭���泬��1024���� 
								int linepos1 = SendEditor(SCI_POSITIONFROMLINE,linecur-1);   //��ǰ����ʼλ��   
								int lineend1 = SendEditor(SCI_GETLINEENDPOSITION,linecur-1); //��ǰ�н���λ��
								if((lineend1 - linepos1)>1024)
									return false;

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
								//GOTO �������⴦�� ���°� 2012.12.25
								if(isGoToStr(linepre))
								{
									nIndent = 0;
									for(int i=2;(linecur-i)>0;i++) //���ϱ���ֱ���ҵ�����GOTO����������Ϊֹ,��¼����
									{
										memset(linepre,0,sizeof(linepre));
										m_ScintillaWnd.SendMessage(SCI_GETLINE,linecur-i,sptr_t(linepre));
										if(!isGoToStr(linepre))
										{
											nIndent = SendEditor(SCI_GETLINEINDENTATION,linecur-i);
											break;
										}
									}

									wchar_t space[1024];   
									::wmemset(space,L' ',1024);
									space[nIndent] = 0;   
									CString csLine=line;
									csLine.Trim();
									::wcscat(space,csLine);
									SendEditor(SCI_SETSEL,linepos,lineend);
									SendEditor(SCI_REPLACESEL, 0, (sptr_t)space); 
								}
								//GOTO �������⴦�� ���°� 2012.12.25

								else if(pos==lineend)
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
					//���Ÿ��� ���°� 2012.12.22
					//braceMatchHighlight();
					//���Ÿ��� ���°� 2012.12.22
					int s1=SendEditor(SCI_GETSELECTIONSTART); //�������λ��
					int s2=SendEditor(SCI_GETSELECTIONEND); //�������λ��
					if(s1==s2)
						braceMatchHighlight();
					else
					{
						SendEditor(SCI_BRACEBADLIGHT,-1); //ȡ��֮ǰ����
					}
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
			break;
		case TC_FINDALLSTR:
			{
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_findInfo.clear();
				TCHAR buff[1024] = {0};
				if( FindAllContent(buff,0) )
				{
					
					((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCFindAllContent();
				}			
			}
			break;
		}

		return CView::OnNotify(wParam, lParam, pResult);
	}


void CTCView::NavigationPos()
{
	TCHAR path[MAX_PATH]={0};
	wcscpy (path,GetDocument()->GetPathName());

	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	int nowline = SendEditor(SCI_GETLINECOUNT); //���������
	int	d_valueline=nowline - m_rowCount;  //������ֵ
	m_rowCount = nowline;

	//int nowallsize = SendEditor(SCI_GETLENGTH); //ȡ�õ�ǰ��������
	//int d_valuepos = nowallsize - m_allSize;  //��������
	//m_allSize = nowallsize;

	int pos = SendEditor(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ��
	int line_number = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ���к�

	int sizeLpos = pFrame->m_markposLpos.size();
	int sizeRpos = pFrame->m_markposRpos.size();
// 	int sizeL = pFrame->m_markposL.size();
// 	int sizeR = pFrame->m_markposR.size();
// 	int sizeLengthR = pFrame->m_markLineLenthR.size();
// 	int sizeLengthL = pFrame->m_markLineLenthL.size();
// 	int sizePathR = pFrame->m_markRpath.size();
// 	int sizePathL = pFrame->m_markLpath.size();

	int linestartpos = SendEditor(SCI_POSITIONFROMLINE, line_number);   //��ǰ����ʼλ��
	int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, line_number); //��ǰ�н���λ��
	int d_valuelinepos = pos -linestartpos;    //ÿ��λ�ò�ֵ
	int lienlength = lineendpos - linestartpos; //�г���
	wstring pathtmp = path;
	TCHAR poschar = SendEditor(SCI_GETCHARAT, pos );

	if(sizeLpos > 0)
	{
		////////////////////�����Ѿ������ڵ���///////////////////////
		if(d_valueline < 0)//��ֵ����0,�Ŵ���
		{
			int line = line_number - d_valueline;

			for(int i=0; i<sizeLpos; i++)
			{
				if((pathtmp.compare(pFrame->m_markLpath[i])==0))//ͬһ���ļ��Ŵ����ֵ
				{
					if((pFrame->m_markposL[i] >= line_number) && (pFrame->m_markposL[i] < line))
					{
						pFrame->m_markposLpos.erase(pFrame->m_markposLpos.begin() + i);
						pFrame->m_markposL.erase(pFrame->m_markposL.begin() + i);
						pFrame->m_markLineLenthL.erase(pFrame->m_markLineLenthL.begin() + i);
						pFrame->m_markLpath.erase(pFrame->m_markLpath.begin() + i);
						sizeLpos -= 1;
					}
				}
			}
		}

		////////////////////����L��ֵ///////////////////////
		if(d_valueline != 0)//��ֵ����0,�Ŵ���
		{
			for(int i=0; i<sizeLpos; i++)
			{
				if((pathtmp.compare(pFrame->m_markLpath[i])==0))//ͬһ���ļ��Ŵ����ֵ
				{
					if(pFrame->m_markposL[i] > line_number)
					{
						pFrame->m_markposL[i] += d_valueline;
					}
				}
			}
		}
	}

	if((sizeRpos > 0) && (d_valueline != 0))//��ֵ����0,�Ŵ���
	{
		////////////////////�����Ѿ������ڵ���///////////////////////
		if(d_valueline < 0)//��ֵ����0,�Ŵ���
		{
			int line = line_number - d_valueline;

			for(int i=0; i<sizeRpos; i++)
			{
				if((pathtmp.compare(pFrame->m_markRpath[i])==0))//ͬһ���ļ��Ŵ����ֵ
				{
					if((pFrame->m_markposR[i] > line_number) && (pFrame->m_markposR[i] < line))
					{
						pFrame->m_markposRpos.erase(pFrame->m_markposRpos.begin() + i);
						pFrame->m_markposR.erase(pFrame->m_markposR.begin() + i);
						pFrame->m_markLineLenthR.erase(pFrame->m_markLineLenthR.begin() + i);
						pFrame->m_markRpath.erase(pFrame->m_markRpath.begin() + i);
						sizeRpos -= 1;
					}
				}
			}
		}

		////////////////////����R��ֵ///////////////////////
		for(int i=0; i<sizeRpos; i++) 
		{
			if((pathtmp.compare(pFrame->m_markRpath[i])==0))//ͬһ���ļ��Ŵ����ֵ
			{
				if(pFrame->m_markposR[i] > line_number)
				{
					pFrame->m_markposR[i] += d_valueline;
				}
			}
		}
	}

	if(sizeLpos > 0)
	{
		if((pFrame->m_markposL.back() != line_number) || (pathtmp.compare(pFrame->m_markLpath.back())!=0)) //�������ͬһ�л���ͬһ�ļ�����µ�λ��,�к�,�г���
		{
			if((poschar!=L'\r') && (poschar!=L'\n'))
			{
				pFrame->m_markposL.push_back(line_number);
				pFrame->m_markposLpos.push_back(d_valuelinepos);
				pFrame->m_markLineLenthL.push_back(lienlength);
				pFrame->m_markLpath.push_back(path);
			}

		}
		else                                //ͬһ��,����µ�λ��,�г���
		{
			if((poschar!=L'\r') && (poschar!=L'\n'))
			{
				pFrame->m_markposLpos.pop_back();
				pFrame->m_markposLpos.push_back(d_valuelinepos);
				pFrame->m_markLineLenthL.pop_back();
				pFrame->m_markLineLenthL.push_back(lienlength);
			}
		}
	}
	else
	{	
		if((poschar!=L'\r') && (poschar!=L'\n'))
		{
			pFrame->m_markposL.push_back(line_number);
			pFrame->m_markposLpos.push_back(d_valuelinepos);
			pFrame->m_markLineLenthL.push_back(lienlength);
			pFrame->m_markLpath.push_back(path);
		}
	}
	////////////////////������///////////////////////
	const int clearLen = 100;
	const int clearSize = 50;

	if(sizeLpos > clearLen)
	{
		pFrame->m_markposLpos.erase(pFrame->m_markposLpos.begin(),pFrame->m_markposLpos.end() - clearSize);
		pFrame->m_markposL.erase(pFrame->m_markposL.begin(),pFrame->m_markposL.end() - clearSize);
		pFrame->m_markLineLenthL.erase(pFrame->m_markLineLenthL.begin(),pFrame->m_markLineLenthL.end() - clearSize);
		pFrame->m_markLpath.erase(pFrame->m_markLpath.begin(),pFrame->m_markLpath.end() - clearSize);
	}
	if(sizeRpos > clearLen)
	{
		pFrame->m_markposRpos.erase(pFrame->m_markposRpos.begin(),pFrame->m_markposRpos.end() - clearSize);
		pFrame->m_markposR.erase(pFrame->m_markposR.begin(),pFrame->m_markposR.end() - clearSize);
		pFrame->m_markLineLenthR.erase(pFrame->m_markLineLenthR.begin(),pFrame->m_markLineLenthR.end() - clearSize);
		pFrame->m_markRpath.erase(pFrame->m_markRpath.begin(),pFrame->m_markRpath.end() - clearSize);
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
//��������ƥ������ ���°�
//��������ƥ������ ���°�
bool CTCView::FindAllContent(TCHAR *rFindStr,int type,int flags)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	//pFrame->m_findInfo.clear();
	TCHAR buff[1024]={0}; //��ǰ������
	TCHAR textShow[1024]={0}; //���浱ǰ����µĵ���
	TCHAR textShowTmp[1024]={0};
	CString textbuff;

	if(type == 0)
	{
		int selPosStart=SendEditor(SCI_GETSELECTIONSTART); //�������λ��
		int selPosEnd=SendEditor(SCI_GETSELECTIONEND); //�������λ��
		int d_Value=selPosEnd - selPosStart;

		if((d_Value > 0) && (d_Value < 1024)) //ѡ����ֱ��ƥ��ѡ���ִ�,����ƥ�������ڵ����ִ�
		{
			SendEditor(SCI_GETSELTEXT,NULL,LPARAM(textShowTmp)); //���ѡ������
		}
		else
		{
			int pos=SendEditor(SCI_GETCURRENTPOS); //�������λ��
			int linecur = SendEditor(SCI_LINEFROMPOSITION,pos); //ȡ�õ�ǰ�� 	
			int linepos = SendEditor(SCI_POSITIONFROMLINE,linecur);   //��ǰ����ʼλ�� 
			int lineend = SendEditor(SCI_GETLINEENDPOSITION,linecur); //��ǰ�н���λ��
			if((lineend - linepos)>1024)
				return false;

			SendEditor(SCI_GETLINE,linecur,(LPARAM)buff);//��ǰ������ʼλ�� 
			int lineStart=SendEditor(SCI_POSITIONFROMLINE,linecur);//��ǰ������ʼλ�� 
			int curPos=pos-lineStart;
			int lineLength=::wcslen(buff);
			int nShowLeng=0;
			for(int i=curPos;i<lineLength;i++)
			{
				if((XMLString::isAlpha(buff[i])||XMLString::isDigit(buff[i]) ||XMLString::isChinese(buff[i])||buff[i]==chUnderscore))
				{
					textShow[i]=buff[i];
					nShowLeng++;
				}																																																																																																																																																																																																							
				else 
					break;
			}
			for(int j=curPos;j>=0;j--)
			{
				if((XMLString::isAlpha(buff[j])||XMLString::isDigit(buff[j]) ||XMLString::isChinese(buff[j])||buff[j]==chUnderscore))
				{
					textShow[j]=buff[j];
					nShowLeng++;
				}
				else
					break;
			}

			if(nShowLeng)
			{
				int k = 0;
				for(int i = 0; i < 1024 ;i++)//ȡ�м�����
				{
					if(textShow[i]!=0)
					{
						textShowTmp[k++]=textShow[i];
					}
					if(k==(nShowLeng-1)) //�������,����,��ʡѭ������
						break;
				}
			}
		}

		textbuff = textShowTmp;
		textbuff.Trim();
		wcscpy(rFindStr,textbuff.GetBuffer(textbuff.GetLength())); //���ز��Ҵ�
		textbuff.ReleaseBuffer(textbuff.GetLength());
		//flags = SCFIND_MATCHCASE | SCFIND_WHOLEWORD;
	}
	else if(type == 1)
	{
		textbuff = rFindStr;
	}

	int sumSize = SendEditor(SCI_GETLENGTH); //����ܴ�С

	TextToFind ttf;
	ttf.chrg.cpMax = sumSize;
	ttf.lpstrText = textbuff.GetBuffer();
	textbuff.ReleaseBuffer();	

	int textbufflen = ::wcslen(textbuff);

	//SendEditor(SCI_SETSEL,rPos,rPos+::wcslen(textbuff));

	FindInfo InfoBuff;

	int rPos = 0;
	int minPos = 0;

	do{
		ttf.chrg.cpMin = minPos;
		rPos = SendEditor(SCI_FINDTEXT, flags, (LPARAM)&ttf);
		if(rPos > 0)
		{
			memset(buff,0,sizeof(buff));
			int linecur = SendEditor(SCI_LINEFROMPOSITION,rPos); //ȡ�õ�ǰ�� 	int setposLLL=0;
			InfoBuff.lineNumber = linecur;
			InfoBuff.strBeginPos = rPos;
			InfoBuff.strEndPos = rPos + textbufflen;
			InfoBuff.path = GetDocument()->GetPathName();
			SendEditor(SCI_GETLINE, linecur ,sptr_t(buff)); //���ָ��������   int setposLLL=0;
			int buffLen = wcslen(buff);
			wcscpy((InfoBuff.lindStr).GetBuffer(buffLen),buff);
			(InfoBuff.lindStr).ReleaseBuffer(buffLen);
			minPos = rPos + textbufflen;
			pFrame->m_findInfo.push_back(InfoBuff);
		}

	}while(rPos>0);

	return true;
}

//��������ƥ�����ݶ��ļ��� ���°� 2013.1.25
bool CTCView::FindAllFileContent(std::vector<wstring> inFilePath,TCHAR *inFindStr)
{
	const int TMPSIZE = 1024*8;
	TCHAR tmp[TMPSIZE]={0};
	int fileCount = inFilePath.size();   //�ļ�����
	int filePos = -2;                    //�ļ�λ��,ȥ��unicode��ʾ��
	int findStrLen =  wcslen(inFindStr);
	int line = 1;                        //�к�
	FindInfo InfoBuff;
	DWORD readCount = 0;

	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	for(int i=0; i<fileCount; i++)
	{
		readCount = 0;
		filePos = -2;
		wmemset(tmp,0,TMPSIZE);

		HANDLE hFile = CreateFile(inFilePath[i].c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		int fileLength = GetFileSize(hFile,NULL);
		int tickCount = 0;

		if(fileLength > TMPSIZE)
		{
			tickCount = fileLength/TMPSIZE;

			if(fileLength%TMPSIZE > 0)
			{
				tickCount ++;
			}
		}
		else
		{
			tickCount = 1;
		}

		for(int ii=0; ii<tickCount;ii++)
		{
			readCount = 0;
			wmemset(tmp,0,TMPSIZE);
			TCHAR *p = tmp;
			if(ReadFile(hFile,tmp,TMPSIZE,&readCount,NULL))
			{
				for(int j=0;j<readCount;j++)
				{
					filePos++;

					if((tmp[j]==L'\n'))
					{
						line ++;
					}

					if((tmp[j]==inFindStr[0]) && (tmp[j+findStrLen-1]==inFindStr[findStrLen-1]))
					{
						if(wcsncmp(p,inFindStr,findStrLen)==0)
						{
							InfoBuff.lineNumber = line;
							InfoBuff.strBeginPos = filePos;
							InfoBuff.strEndPos = filePos + findStrLen;
							InfoBuff.path = inFilePath[i].c_str();
							pFrame->m_findInfo.push_back(InfoBuff);
						}
					}
					p++;
				}
			}
			else
			{
				break;
			}
		}

		CloseHandle(hFile);
	}

	return true;
}


//����������Ϣ
void CTCView::SetEditorInfo(EditorSetInfo editSetInfo)
{
	//TCHAR *fontFace = new TCHAR[editSetInfo.fontFace.GetLength()];
	//wcscpy(fontFace,editSetInfo.fontFace.GetBuffer(editSetInfo.fontFace.GetLength()));
	SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)editSetInfo.fontFace.GetBuffer()); 
	SendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT, editSetInfo.fontSize); 
	SendEditor(SCI_STYLESETBOLD, STYLE_DEFAULT, editSetInfo.fontBold);
	SendEditor(SCI_SETTABWIDTH, editSetInfo.tabWidth);\
	editSetInfo.fontFace.ReleaseBuffer();
	//delete []fontFace;
	//int linestartpos = SendEditor(SCI_POSITIONFROMLINE, 7); //��ǰ����ʼλ��
	//int lineendpos = SendEditor(SCI_GETLINEENDPOSITION, 7); //��ǰ�н���λ��
	//SendEditor(SCI_SETSEL,linestartpos,lineendpos);
	//SendEditor(SCI_SETSELBACK,STYLE_DEFAULT,0x00ffff);

	//SendEditor(SCI_STYLESETFORE, STYLE_DEFAULT, RGB(0,0,0));
	//SendEditor(SCI_STYLESETBACK, STYLE_DEFAULT, RGB(122,122,122));
	//SendEditor(SCI_STYLESETEOLFILLED, 0);	
	//SendEditor(SCI_STYLE?SETEOLFILLED, bstyle, 1);
	//SendEditor(SCI_MARKERSETBACK, STYLE_DEFAULT,RGB(122,122,122));	
	//SendEditor(SCI_SETWHITESPACEFORE, STYLE_DEFAULT,RGB(122,122,122));	
	//�����ɫ
	//SendEditor(SCI_SETSELBACK, STYLE_DEFAULT,RGB(122,122,122));	
}


/***********************************************************************/
/*  \�������ƣ�GetEditInfo
/*	\�������ܣ���ȡ�༭�����������Ϣ
/*	\�������أ�void
/*	\����˵����
	@param     &editSetInfo	���ر༭�����������Ϣ�ṹ��
/***********************************************************************/
void CTCView::GetEditInfo(EditorSetInfo &editSetInfo)
{
	//�����С
	editSetInfo.fontSize = SendEditor(SCI_STYLEGETSIZE,STYLE_DEFAULT);
	//**********************************//
	//��������
	//��֪�����ȷ��乻����
	TCHAR fontName[48] = {0}; 
	memset(fontName,0,sizeof(fontName));
	SendEditor(SCI_STYLEGETFONT,STYLE_DEFAULT,sptr_t(fontName));
	editSetInfo.fontFace = fontName;

	//�Ƿ����
	editSetInfo.fontBold = SendEditor(SCI_STYLEGETBOLD,STYLE_DEFAULT);
	editSetInfo.tabWidth = SendEditor(SCI_GETTABWIDTH ,STYLE_DEFAULT);
}


void CTCView::AddToolTip(CString tip)
{
	// Generated by XSuperTooltipTest on 2013 January 11
	SUPER_TOOLTIP_INFO sti;

	sti.bSuperTooltip     = TRUE;
	sti.nVirtualKeyCode   = VK_F1;						// zero if none
	sti.nKeyCodeId        = 0/*<insert key code id here>*/;	// can be anything you want
	sti.nIDTool           = 0;				     // can be anything you want
	sti.nSizeX            = 1024;				//���������
	sti.pWnd              = &m_ScintillaWnd;	//������
	sti.strHeader         = tip;
	sti.strBody           = _T(" ");				//��ʾ����
	//sti.strFooter         = tip;
	sti.rgbBegin          = RGB(255, 255, 255);
	sti.rgbMid            = RGB(242, 246, 251);
	sti.rgbEnd            = RGB(202,218,239);
	sti.rgbText           = RGB(76,76,76);	
		
	m_ToolTip.SetScreenTipScheme((CXSuperTooltip::ScreenTipScheme)0);

	CString strHtml = m_ToolTip.AddTool(&sti);
}

//void CTCView::AddToolTipDebugVaule(CString tip)
//{
//	// Generated by XSuperTooltipTest on 2013 January 11
//	SUPER_TOOLTIP_INFO sti;
//
//	sti.bSuperTooltip     = FALSE;
//	sti.nVirtualKeyCode   = VK_F1;						// zero if none
//	sti.nKeyCodeId        = 0/*<insert key code id here>*/;	// can be anything you want
//	sti.nIDTool           = 0;				     // can be anything you want
//	sti.nSizeX            = 1024;				//���������
//	sti.pWnd              = &m_ScintillaWnd;	//������
//	//sti.strHeader         = tip;
//	sti.strBody           = tip;				//��ʾ����
//	//sti.strFooter         = tip;
//	sti.rgbBegin          = RGB(255, 255, 255);
//	sti.rgbMid            = RGB(242, 246, 251);
//	sti.rgbEnd            = RGB(202,218,239);
//	sti.rgbText           = RGB(76,76,76);
//
//	m_ToolTip.SetScreenTipScheme((CXSuperTooltip::ScreenTipScheme)0);
//
//	CString strHtml = m_ToolTip.AddTool(&sti);
//
//}

int CTCView::replaceSign(TCHAR ch)
{
	switch(ch)
	{
	case L'��':
		return L'!';
		break;
	case L'��':
		return L'%';
		break;
	case L'��':
		return L'(';
		break;
	case L'��':
		return L')';
		break;
	case L'��':
		return L'[';
		break;
	case L'��':
		return L']';
		break;
	case L'��':
		return L';';
		break;
	case L'��':
		return L':';
		break;
	case L'��':
	case L'��':
		return L'"';
		break;
	case L'��':
		return L'<';
		break;
	case L'��':
		return L'>';
		break;
	case L'��':
		return L',';
		break;
	case L'��':
		return L'.';
		break;
	default:
		return ch;
		break;
	}
}
//�Ƿ�ע�ͻ��ַ����� ���°� 2013.2.7
bool CTCView::IsNoteOrStr(TCHAR *str,int pos)
{
	int len = wcslen(str);
	int count = 0; //���Ÿ���
	int pos1 = -1; //���ſ�ʼλ��
	int pos2 = -1; //���Ž����λ��
	bool ret = true;

	for(int i=0; i<len; i++) //�ж��Ƿ�������
	{
		if(str[i]==L'"')
		{
			count++;
			if((count%2)==1)
				pos1 = i;
			else if((count%2)==0)
			{
				pos2 = i;
				if((pos1<pos) && (pos2>=pos))
				{
					return true;
				}
				else
				{
					count = 0;
					pos1 = -1;
					pos2 = -1;
				}
			}
		}
	}

	for(int ii=0; ii<len; ii++)
	{
		if(str[ii]==L'/') //�ж��Ƿ�ע����
		{
			if(str[ii+1]==L'/')
			{
				int notepos = ii+1;
				if(pos>notepos)
				{
					count = 0;
					pos1 = -1;
					pos2 = -1;

					for(int i=0; i<len; i++) //�ж��Ƿ�������
					{
						if(str[i]==L'"')
						{
							count++;
							if((count%2)==1)
								pos1 = i;
							else if((count%2)==0)
							{
								pos2 = i;
								if((pos1<notepos) && (pos2>=notepos))
								{
									ret = false;
									break;
								}
								else
								{
									ret = true;
									count = 0;
									pos1 = -1;
									pos2 = -1;
								}
							}
						}
					}
					if(ret)
						return ret;
				}
			}
		}
	}
	return false;
}

void CTCView::ShowIntelisenseFunExample()
	{
		///////////////////////////���λ��///////////////////////
		int pos = m_ScintillaWnd.SendMessage(SCI_GETCURRENTPOS); //ȡ�õ�ǰλ�� 
		int nStart=-1;
		int nEnd=-1;

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
			return ;

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
				CString csNSFUN = pToShow;
				int dotIndex=csNSFUN.Find(_T('.'));
				if(dotIndex!=-1)
				{
					CString nameSP=csNSFUN.Mid(0,dotIndex);
					CString nameFun=csNSFUN.Mid(dotIndex+1,csNSFUN.GetLength()-dotIndex);
					for( int size = 0; size < CTCRunTimeManager::g_TCAip.m_allApi.size(); size++ )
					{
						for( int nspa = 0; nspa <  CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp.size();  nspa++ )
						{
							CString helpSPName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].namespaceName;
							if( 0 == helpSPName.CompareNoCase(nameSP) )
							{
								for( int func = 0; func < CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun.size(); func++ )
								{
									CString csFun = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun[ func ].funcName;

									CString csSel;
									csSel.Empty();
									int cursel = m_listFaterView.m_listBox.GetCurSel();
									if( cursel < 0 )
									{
										if(lineBuf)
											delete []lineBuf;
										return;
									}
									m_listFaterView.m_listBox.GetText(cursel,csSel);
									CString temp = csSel.Left(nameFun.GetLength());
									if ( temp.CompareNoCase(nameFun) == 0  && 0 == csFun.CompareNoCase(csSel) )
									{
										CString strExample = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun[ func ].funcExample;
										strExample.Replace(_T("|"),_T(""));
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->GiveToolTip(this,strExample,NULL);
									}
								}
							}

						}
					}
				}					
			}
		}
		if(lineBuf)
			delete []lineBuf;
	}


	void CTCView::ShowIntelisenseFunExampleWithoutPos(CString nspace)
	{
			CString nameSP = nspace;
			//CString nameFun=csNSFUN.Mid(dotIndex+1,csNSFUN.GetLength()-dotIndex);
			for( int size = 0; size < CTCRunTimeManager::g_TCAip.m_allApi.size(); size++ )
			{
				for( int nspa = 0; nspa <  CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp.size();  nspa++ )
				{
					CString helpSPName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].namespaceName;
					if( 0 == helpSPName.CompareNoCase(nameSP) )
					{
						for( int func = 0; func < CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun.size(); func++ )
						{
							CString csFun = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun[ func ].funcName;

							CString csSel;
							csSel.Empty();
							int cursel = m_listFaterView.m_listBox.GetCurSel();
							if( cursel < 0 )
								return;
							m_listFaterView.m_listBox.GetText(cursel,csSel);
							//CString temp = csSel.Left(nameFun.GetLength());
							if ( /*temp.CompareNoCase(nameFun) == 0  &&*/ 0 == csFun.CompareNoCase(csSel) )
							{
								CString strExample = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun[ func ].funcExample;
								strExample.Replace(_T("|"),_T(""));
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->GiveToolTip(this,strExample,NULL);
								return;
							}
						}
					}
				}
			}
	}




	void CTCView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
	}
