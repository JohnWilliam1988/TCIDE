#if !defined(MYAPIPROCESS_H)
#define MYAPIPROCESS_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "AbsBlockParser.h"
#include "BaseBlock.h"
#include "MyVarListContainer.h"
#include "match/My/MyValue.h"
#include "MyDataList.h"
#include <windows.h>

MATCH_CPP_NAMESPACE_BEGIN

//����Ĭ��API�ĵ��÷���,���õ�matchDll.dll����Ŀ����Ľӿ�
typedef bool (* lpMyCommand)(int index,MyDataList * list,_variant_t * result);
typedef void (* lpMyCtrlWinCwnd)(HANDLE handle);
typedef void (* lpSetTempPath)(const XMLCh * const path);
typedef int  (* lpValidate)(const XMLCh *  nameSpace,const XMLCh * funcName, XMLCh * param);
typedef bool (* lpisNameSpaceExist)(const XMLCh * const nameSpace);
//����ʱ��dll������
class PARSERS_EXPORT MyApiProcess
{
public:
    //��������:��ʼ��DLL���й�����
    //����ֵ:��
    //��������                ����
    //void                    ��
	static void initialize();

    //��������:����DLL���й�����
    //����ֵ:��
    //��������                ����
    //void                    ��
    static void cleanUp();	

	//����һ���ռ����Ƿ����
	static bool IsNameSpExist(const XMLCh *nameSpace);

    //��api.xml�������һ��API����
	static int Find(const XMLCh *nameSpace,const XMLCh *funcname, XMLCh *param=0);
	//����EXE���
	static void TCSetExeWinCwnd(HANDLE handle);	
	//������ʱĿ¼
	static void SetTempPath(const XMLCh * const path);
	//ִ��API�ӿ�
	static void run(int index,MyDataList *argumentList,_variant_t*);
private:
    static HMODULE fHDll;
	static lpMyCommand fcmd;
	static lpMyCtrlWinCwnd fsetwinCwnd;
	static lpSetTempPath   fsetTempPath;
	static lpValidate      fValidate;
	static lpisNameSpaceExist fCheckNS;

    MyApiProcess();
};


MATCH_CPP_NAMESPACE_END

#endif
