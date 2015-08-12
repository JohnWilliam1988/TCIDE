//////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���#includeԤ�������ĳ������MyIncludeStatement�����
//////////////////////////////////////////////////////////////////////////

#ifndef MYNAMESPACESTATEMENT_H
#define MYNAMESPACESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN
//�������ǰ� namespace ��� ����� mystatement �̳���
//namespace �����Ҫ���������������ռ��
class PARSERS_EXPORT MyNameSpaceStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //spaceName     const XMLCh* const          �ռ���
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
	MyNameSpaceStatement(const XMLCh* const    spaceName
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
	//�õ������ռ���
    //��������:�õ������ռ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            �ռ���
	const XMLCh* getSpaceName() const;

    //��������
	virtual ~MyNameSpaceStatement();

private:
    //��ʼ��
	void initialize();                 

	//��������ڴ�
    void cleanUp();

//
    //��������:���ÿռ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������       ��������                    ����
    //includeString  const XMLCh* const          �ռ���
	void setSpaceName(const XMLCh* const spaceName);

	
	XMLCh            fNameSpace[NAMEMAXLENGTH];                  //��¼�ռ�����

    /**
    * Copy constructor
    */
	MyNameSpaceStatement(const MyNameSpaceStatement&);
    MyNameSpaceStatement& operator=(const MyNameSpaceStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif