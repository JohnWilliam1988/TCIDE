// TCHtmlExample.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "TCHtmlExample.h"


// CTCHtmlExample �Ի���

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
	//ȥ��3D�߿�Ч��
	SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR | DOCHOSTUIFLAG_NO3DBORDER);
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CTCHtmlExample, CDHtmlDialog)
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDOK, &CTCHtmlExample::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CTCHtmlExample)
	//DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	//DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CTCHtmlExample ��Ϣ�������

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

//�Ƿ��ǲ�����
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
    ////////////////////////////////////����ԭ��/////////////////////////////////
    if (GetElement(_T("Function"),&pLinkElement) == S_OK && pLinkElement != NULL)
    {
        CString toInputProto;
        ///////////////////�������͹ؼ���/////////////////////////////
		CString csVar;
        CString csInt;
        CString csStr;
        CString csBool;
        CString csDouble;
		csVar.Format(_T("<span style='color:blue'>%s</span>"),L"���� ");
		csFunProto.Replace(L"���� ",csVar);
        csInt.Format(_T("<span style='color:blue'>%s</span>"),L"���� ");
        csFunProto.Replace(L"���� ",csInt);
        csStr.Format(_T("<span style='color:blue'>%s</span>"),L"�ַ��� ");
        csFunProto.Replace(L"�ַ��� ",csStr);
        csBool.Format(_T("<span style='color:blue'>%s</span>"),L"�߼��� ");
        csFunProto.Replace(L"�߼��� ",csBool);
        csDouble.Format(_T("<span style='color:blue'>%s</span>"),L"������ ");
        csFunProto.Replace(L"������ ",csDouble);
        ///////////////////Ӣ���������͹ؼ���/////////////////////////////
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

    //����˵��
    if (GetElement(_T("FunctionDec"), &pLinkElement) == S_OK && pLinkElement != NULL)
    {
        CString input;
        input.Format(_T("<a ID=FunctionDec target=_blank>%s </a> "),csFunDes);
        pLinkElement->put_outerHTML(_bstr_t(input));				
    }

    ////////////////////////��������/////////////////
    if (GetElement(_T("FunctionExample"), &pLinkElement) == S_OK && pLinkElement != NULL)
    {
        CString addin=csExample;
		CString csVar;
        CString csInt;
        CString csStr;
        CString csBool;
        CString csDouble;
		csVar.Format(_T("<span style='color:blue'>%s</span>"),L"���� ");
		addin.Replace(L"���� ",csVar);
        csInt.Format(_T("<span style='color:blue'>%s</span>"),L"���� ");
        addin.Replace(L"���� ",csInt);
        csStr.Format(_T("<span style='color:blue'>%s</span>"),L"�ַ��� ");
        addin.Replace(L"�ַ��� ",csStr);
        csBool.Format(_T("<span style='color:blue'>%s</span>"),L"�߼��� ");
        addin.Replace(L"�߼��� ",csBool);
        csDouble.Format(_T("<span style='color:blue'>%s</span>"),L"������ ");
        addin.Replace(L"������ ",csDouble);
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
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),L" �� ");
        addin.Replace(L" �� ",csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),L" �� ");
        addin.Replace(L" �� ",csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),L" true ");
        addin.Replace(L"true ",csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),L" false ");
        addin.Replace(L" false ",csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("���� "));
        addin.Replace(_T("���� "),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("return "));
        addin.Replace(_T("return "),csHelp);
        ////////////Ԥ���ؼ���/////////////////////////////////////////////
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("�ռ�"));
        addin.Replace(_T("�ռ�"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("�ռ����"));
        addin.Replace(_T("�ռ����"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("namespace"));
        addin.Replace(_T("namespace"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endnamespace"));
        addin.Replace(_T("nendamespace"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("���� "));
        addin.Replace(_T("���� "),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("���ܽ���"));
        addin.Replace(_T("���ܽ���"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("function "));
        addin.Replace(_T("function "),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endfunction"));
        addin.Replace(_T("endfunction"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("���"));
        addin.Replace(_T("���"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("�������"));
        addin.Replace(_T("�������"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("if"));
        addin.Replace(_T("if"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endif"));
        addin.Replace(_T("endif"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("����"));
        addin.Replace(_T("����"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("else"));
        addin.Replace(_T("else"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("ѭ��"));
        addin.Replace(_T("ѭ��"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("ѭ������"));
        addin.Replace(_T("ѭ������"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("while"));
        addin.Replace(_T("while"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("endwhile"));
        addin.Replace(_T("endwhile"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("����"));
        addin.Replace(_T("����"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("����"));
        addin.Replace(_T("����"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("continue"));
        addin.Replace(_T("continue"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("break"));
        addin.Replace(_T("break"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("�ظ�"));
        addin.Replace(_T("�ظ�"),csHelp);
        csHelp.Format(_T("<span style='color:blue'>%s</span>"),_T("�ظ�����"));
        addin.Replace(_T("�ظ�����"),csHelp);
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
                    //û���ҵ����з�,��ʾ�������ļ������һ��
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
        ///////////////////���ֵĸ�ʽ��////////////////////////////////

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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDHtmlDialog::OnPaint()
	CDHtmlDialog::OnPaint();
}


BOOL CTCHtmlExample::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//����F5������ˢ��
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == 116) || pMsg->message == WM_RBUTTONDOWN  || pMsg->message == WM_RBUTTONDBLCLK )
		return TRUE;
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}

//void CTCHtmlExample::OnBnClickedOk()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}
