/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-09-13  Ф�㷶      ���#import���ĳ�����MyImportStatement�����
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYIMPORTSTATEMENT_H
#define MYIMPORTSTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//�������ǰ�import��� ����� mystatement �ļ̳���
//
class PARSERS_EXPORT MyImportStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������        ��������                    ����
    //progID         const XMLCh* const          com����Ľӿ�ID
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
    MyImportStatement(const XMLCh* const    progID
		,const XMLCh* const    csNS
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������:����ProgID
    //����ֵ::
    //��������                ����
    //const XMLCh*           COM�����ID����
    const XMLCh* getProgIDString() const;

    //��������:����com����ĵĿռ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            com����ĵĿռ���
    const XMLCh* getNameSpaceString() const;

    //��������
    virtual ~MyImportStatement();

private:

    //��ʼ��
    void initialize();                 
    //��������ڴ�
    void cleanUp();

    //��������:����ProgID����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //exp           const XMLCh* const         COM�����ID����
    void setProgIDString(const XMLCh* const id);

	//��������:���ÿռ�����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //ns           const XMLCh* const          �ռ�����
    void setNameSpaceString(const XMLCh* const ns);


    XMLCh*            fProgID;                          //COM���ID��
    XMLCh*            fNamespace;                       //�ռ���

    /**
    * Copy constructor
    */
    MyImportStatement(const MyImportStatement&);
    MyImportStatement& operator=(const MyImportStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif