#ifndef MYCOMFRAME_H
#define MYCOMFRAME_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <match/framework/MemoryManager.hpp> //�ڴ������
#include <match/util/OutOfMemoryException.hpp>//�쳣��
#include <match/util/Janitor.hpp>


#include "MyApiDllList.h"
#include "MyValue.h"


MATCH_CPP_NAMESPACE_BEGIN 

//��������XML�ļ�
class XMLPARSER_EXPORT MyComFrame
{
public:
	//���캯��
	//����˵��:
	//��������      ��������                    ����
	//manager       MemoryManager* const        �ڴ������
	MyComFrame(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//��������
	~MyComFrame();

	//�����������õ�����DLL�����ָ��
	///����ֵ:  ������ͷ�ڵ�
	//��������                ����
	//const MyApiDllList *    DLL�����ָ��
	const MyApiDllList * getList() const;

	//�����������õ�����DLL�����ָ��
	///����ֵ:  ������ͷ�ڵ�
	//��������                ����
	//const MyApiDllList *    DLL�����ָ��
	 MyApiDllList * getList();


	//������������һ��dll�ڵ�,�ŵ�dll��������ȥ
	///����ֵ: ��
	//����˵��:
	//��������      ��������                    ����
	//MyApiDll      const MyApiDll *            DLL�ڵ�
	void insertDll(MyApiDll * const MyApiDll);

	//��������������DLL��·��
	///����ֵ:  DLL·��
	//��������                ����
	//const XMLCh *          DLL��·��
	//����˵��:
	//��������      ��������                    ����
	//namesp      const XMLCh * const            �����ռ�����
	//funcname    const XMLCh * const            ������������
	const XMLCh* findDllPath(const XMLCh * const namesp,const XMLCh * const funcname) const;

	//�������������Һ�������
	///����ֵ:  ����
	//��������                ����
	//const XMLCh *           ����
	//����˵��:
	//��������      ��������                    ����
	//namesp      const XMLCh * const            �����ռ�����
	//funcname    const XMLCh * const            ������������
	const XMLCh* findFunExample(const XMLCh * const namesp,const XMLCh * const funcname) const;

	//
    //��������:�жϿռ����Ƿ��Ѿ��ڱ���API�д���
    //����ֵ:������ ��ʾ����,�� ��ʾ������
    //��������                ����
    //bool                   ������ ��ʾ����,�� ��ʾ������
    //����˵��:
    //��������          ��������                    ����
    //nameSpace         const XMLCh * const         Ҫ���ҵ������ռ�
	bool isNameSpaceExist(const XMLCh * const nameSpace);

	//���
	void cleanUp(void);

protected:

	//��ʼ��
	void initialize();

	//HINSTANCE g_HDLL;  //dll �ľ��

	MyApiDllList *fList;

	MemoryManager *fMemoryManager; //�ڴ����

};

MATCH_CPP_NAMESPACE_END

#endif