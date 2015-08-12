// TCHtmlExample.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCHtmlExample.h"


// CTCHtmlExample 对话框

IMPLEMENT_DYNCREATE(CTCHtmlExample, CDHtmlDialog)

CTCHtmlExample::CTCHtmlExample(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CTCHtmlExample::IDD, CTCHtmlExample::IDH, pParent)
{

}

CTCHtmlExample::~CTCHtmlExample()
{
}

void CTCHtmlExample::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CTCHtmlExample::OnInitDialog()
{
	//去掉3D边框效果
	SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR | DOCHOSTUIFLAG_NO3DBORDER);
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CTCHtmlExample, CDHtmlDialog)
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDOK, &CTCHtmlExample::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CTCHtmlExample)
	//DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	//DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CTCHtmlExample 消息处理程序

//HRESULT CTCHtmlExample::OnButtonOK(IHTMLElement* /*pElement*/)
//{
//	OnOK();
//	return S_OK;
//}

//HRESULT CTCHtmlExample::OnButtonCancel(IHTMLElement* /*pElement*/)
//{
//	OnCancel();
//	return S_OK;
//}
static bool isHex(wchar_t ch)
{
    if((L'a'<=ch&&ch<=L'f')||(L'A'<=ch&&ch<=L'F'))
        return true;
    return false;
}


static bool isDig(wchar_t ch)
{
    if((L'0'<=ch&&ch<=L'9'))
        return true;
    return false;
}

//是否是操作符
static bool isOper(wchar_t ch)
{
    if(L'+'==ch||ch==L'-'||ch==L'*'||ch==L','||ch==L'/'||ch==L' '||ch==L'>'||ch==L'<'||ch==L'%'||ch==L'&'||ch==L'|'||ch==L'!'||ch==L'('||ch==L')'||ch==L'='||ch==L'>'||ch==L'<')
        return true;
    return false;
}

void CTCHtmlExample::SetDecText(CString csNS,CString csFunName,CString csFunProto,CString csFunDes,CString csExample)
{

    /*if (GetElement(_T("NameSpace"), &pLinkElement) == S_OK && pLinkElement != NULL)
    {
        CString input;
        input.Format(_T("<a ID=NameSpace target=_blank class='fwb green'><b>%s</b></a>"),csNS);
        pLinkElement->put_outerHTML(_bstr_t(input));
    }
    if (GetElement(_T("FunctionName"), &pLinkElement) == S_OK && pLinkElement != NULL)
    {
        CString input;
        input.Format(_T("<a ID=FunctionName target=_blank >%s</a>"),csFunName);
        pLinkElement->put_outerHTML(_bstr_t(input));					
    }*/
    ////////////////////////////////////函数原型/////////////////////////////////
    if (GetElement(_T("Function"),&pLinkElement) == S_OK && pLinkElement != NULL)
    {
        CString toInputProto;
        ///////////////////数据类型关键字/////////////////////////////
		CString csVar;
        CString csInt;
        CString csStr;
        CString csBool;
        CString csDouble;
		csVar.Format(_T("<span style='color:blue'>%s</span>"),L"变量 ");
		csFunProto.Replace(L"变量 ",csVar);
        csInt.Format(_T("<span style='color:blue'>%s</span>"),L"整型 ");
        csFunProto.Replace(L"整型 ",csInt);
        csStr.Format(_T("<span style='color:blue'>%s</span>"),L"字符型 ");
        csFunProto.Replace(L"字符型 ",csStr);
        csBool.Format(_T("<span style='color:blue'>%s</span>"),L"逻辑型 ");
        csFunProto.Replace(L"逻辑型 ",csBool);
        csDouble.Format(_T("<span style='color:blue'>%s</span>"),L"浮点型 ");
        csFunProto.Replace(L"浮点型 ",csDouble);
        ///////////////////英文数据类型关键字/////////////////////////////
		csVar.Format(_T("<span style='color:blue'>%s</span>"),L"var ");
		csFunProto.Replace(L"var ",csVar);
        csInt.Format(_T("<span style='color:blue'>%s</span>"),L"int ");
        csFunProto.Replace(L"int ",csInt);		
        csStr.Format(_T("<span style='color:blue'>%s</span>"),L"string ");
        csFunProto.Replace(L"string ",csStr);
        csDouble.Format(_T("<span style='color:blue'>%s</span>"),L"double ");
        csFunProto.Replace(L"double ",csDouble);
        csBool.Format(_T("<span style='color:blue'>%s</span>"),L"bool ");
        csFunProto.Replace(L"bool ",csBool);
        toInputProto.Format(_T("<a ID=Function target=_blank >%s</a> "),csFunProto);		
        pLinkElement->put_outerHTML(_bstr_t(toInputProto));
    }

    //函数说明
    if (GetElement(_T("FunctionDec"), &pLinkElement) == S_OK && pLinkElement != NULL)
    {
        CString input;
        input.Format(_T("<a ID=FunctionDec target=_blank>%s </a> "),csFunDes);
        pLinkElement->put_outerHTML(_bstr_t(input));				
    }

    ////////////////////////函数举例/////////////////
    if (GetElement(_T("FunctionExample"), &pLinkElement) == S_OK && pLinkElement != NULL)
    {
        CString addin=csExample;
		CString csVar;
        CString csInt;
        CString csStr;
        CString csBool;
        CString csDouble;
		csVar.Format(_T("<span style='color:blue'>%s</span>"),L"变量 ");
		addin.Replace(L"变量 ",csVar);
        csInt.Format(_T("<span style='color:blue'>%s</span>"),L"整型 ");
        addin.Replace(L"整型 ",csInt);
        csStr.Format(_T("<span style='color:blue'>%s</span>"),L"字符型 ");
        addin.Replace(L"字符型 ",csStr);
        csBool.Format(_T("<span style='color:blue'>%s</span>"),L"逻辑型 ");
        addin.Replace(L"逻辑型 ",csBool);
        csDouble.Format(_T("<span style='color:blue'>%s</span>"),L"浮点型 ");
        addin.Replace(L"浮点型 ",csDouble);
		csVar.Format(_T("<span style='color:blue'>%s</span>"),L"var ");
		addin.Replace(L"var ",csVar);
        csInt.Format(_T("<span style='color:blue'>%s</span>"),L"int ");
        addin.Replace(L"int ",csInt);
        csStr.Format(_T("<span style='color:blue'>%s</span>"),L"string ");
        addin.Replace(L"string ",csStr);
        csDouble.Format(_T("<span style='color:blue'>%s</span>"),L"double ");
        addin.Replace(L"double ",csDouble);
        csBool.Format(_T("<span style='color:blue'>%s</span>"),L"bool ");
        addin.Replace(L"bool ",csBool);

        CString csHelp;
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),L" 真 ");
        addin.Replace(L" 真 ",csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),L" 假 ");
        addin.Replace(L" 假 ",csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),L" true ");
        addin.Replace(L"true ",csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),L" false ");
        addin.Replace(L" false ",csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("返回 "));
        addin.Replace(_T("返回 "),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("return "));
        addin.Replace(_T("return "),csHelp);
        ////////////预留关键字/////////////////////////////////////////////
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("空间"));
        addin.Replace(_T("空间"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("空间结束"));
        addin.Replace(_T("空间结束"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("namespace"));
        addin.Replace(_T("namespace"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endnamespace"));
        addin.Replace(_T("nendamespace"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("功能 "));
        addin.Replace(_T("功能 "),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("功能结束"));
        addin.Replace(_T("功能结束"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("function "));
        addin.Replace(_T("function "),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endfunction"));
        addin.Replace(_T("endfunction"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("如果"));
        addin.Replace(_T("如果"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("如果结束"));
        addin.Replace(_T("如果结束"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("if"));
        addin.Replace(_T("if"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endif"));
        addin.Replace(_T("endif"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("否则"));
        addin.Replace(_T("否则"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("else"));
        addin.Replace(_T("else"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("循环"));
        addin.Replace(_T("循环"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("循环结束"));
        addin.Replace(_T("循环结束"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("while"));
        addin.Replace(_T("while"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endwhile"));
        addin.Replace(_T("endwhile"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("继续"));
        addin.Replace(_T("继续"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("跳出"));
        addin.Replace(_T("跳出"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("continue"));
        addin.Replace(_T("continue"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("break"));
        addin.Replace(_T("break"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("重复"));
        addin.Replace(_T("重复"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("重复结束"));
        addin.Replace(_T("重复结束"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("repeat"));
        addin.Replace(_T("repeat"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endrepeat"));
        addin.Replace(_T("endrepeat"),csHelp);
        addin.Replace(_T("\r\n"),L"<br/>");
        addin.Replace(_T("\r"),L"<br/>");
        addin.Replace(_T("\n"),L"<br/>");


        
        CStringArray    strArray;
        CStringArray    commentArray;
        CStringArray	digsixteenArray;

        int	addlen=0;
        while(addlen<addin.GetLength())
        {
            if(addin[addlen]==L'/' && addin[addlen+1]==L'/')
            {
                int end;
                end=addin.Find(_T("<br/>"),addlen+1);
                if(end!=-1)
                {
                    commentArray.Add(addin.Mid(addlen,end-addlen));
                    addlen=end;
                }
                else
                {
                    //没有找到换行符,表示就是在文件的最后一行
                    commentArray.Add(addin.Mid(addlen,addin.GetLength()));
                    addlen=addin.GetLength();
                }
            }
            if( addin[addlen]=='\"')
            {
                int start,end;
                start=addlen;
                addlen++;
                while(addin[addlen]!=_T('\"'))
                {
                    if(addlen>=addin.GetLength())
                        break;

                    addlen++;
                }
                end=addlen;
                CString help;
                help = addin.Mid(start,end-start+1);
                strArray.Add(help);
            }
            if(::isDig(addin[addlen]))
            {
                if(addlen!=0)
                {
                    if(isOper(addin[addlen-1]))
                    {
                        int start,end;
                        start=addlen;

                        while(::isDig(addin[addlen])||addin[addlen]=='.')
                            addlen++;
                        end=addlen;
                        CString help=addin.Mid(start,end-start);
                        addin.Delete(start,end-start);
                        CString helpDig;
                        helpDig.Format(_T("<span style='color:Teal'>%s</span>"),help);
                        addin.Insert(start,helpDig);
                        //digsixteenArray.Add(help);
                        addlen+=::lstrlen(_T("<span style='color:Teal'></span>"));
                        //addlen+=32;
                    }
                }
            }
            if(addin[addlen]==L'#' &&(::isDig(addin[addlen+1])||::isHex(addin[addlen+1])))
            {
                if(addlen!=0)
                {
                    if(isOper(addin[addlen-1]))
                    {
                        int start,end;
                        start = addlen;;
                        addlen++;
                        while(::isDig(addin[addlen])||::isHex(addin[addlen]))
                            addlen++;
                        end=addlen;
                        CString help=addin.Mid(start,end-start);
                        addin.Delete(start,end-start);
                        CString helpDig;
                        helpDig.Format(_T("<span style='color:Teal'>%s</span>"),help);
                        addin.Insert(start,helpDig);
                        addlen+=::lstrlen(_T("<span style='color:Teal'></span>"));
                        //CString help=addin.Mid(start,end-start);
                        //digsixteenArray.Add(help);
                    }
                }
            }
            addlen++;
        }
        ///////////////////数字的格式化////////////////////////////////

        int length;
        //length=digsixteenArray.GetCount();
        //for(int i=0;i<length;i++)
        //{
        //    CString helpDig;
        //    helpDig.Format(_T("<span style='color:Teal'>%s</span>"),digsixteenArray[i]);
        //    addin.Replace(digsixteenArray[i],helpDig);
        //}
        length=strArray.GetCount();
        for(int i=0;i<length;i++)
        {
            CString helpStr;
            helpStr.Format(_T("<span style='color:red'>%s</span>"),strArray[i]);
            addin.Replace(strArray[i],helpStr);
        }
        length=commentArray.GetCount();
        for(int i=0;i<length;i++)
        {
            CString helpComment;
            helpComment.Format(_T("<span style='color:Green'>%s</span>"),commentArray[i]);
            addin.Replace(commentArray[i],helpComment);
        }
        CString csFormat;
        csFormat.Format(_T("<a ID=FunctionExample target=_blank >%s</a> "),addin);
        addin=csFormat;
        pLinkElement->put_outerHTML(_bstr_t(addin));					
    }
}


void CTCHtmlExample::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDHtmlDialog::OnPaint()
	CDHtmlDialog::OnPaint();
}


BOOL CTCHtmlExample::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//拦截F5，不让刷新
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == 116) || pMsg->message == WM_RBUTTONDOWN  || pMsg->message == WM_RBUTTONDBLCLK )
		return TRUE;
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}

//void CTCHtmlExample::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
