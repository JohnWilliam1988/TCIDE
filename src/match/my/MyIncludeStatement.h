//////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���#includeԤ�������ĳ������MyIncludeStatement�����
//////////////////////////////////////////////////////////////////////////

#ifndef MYINCLUDESTATEMENT_H
#define MYINCLUDESTATEMENT_H

#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���ǰ�includeԤ����ָ�� ����� mystatement �̳���
//include Ԥ����ָ�� ��Ҫ������ ���������ļ�������ִ��Դ�ļ�ǰ����ִ�е� 
class PARSERS_EXPORT MyIncludeStatement : public MyStatement
{
public:
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //includeString const XMLCh* const          include��������·��,ע�������Ǵ�˫�������������ַ���
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
	MyIncludeStatement(const XMLCh* const    includeString
			          ,const unsigned int    id = 0
		              ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
 
    //��������:�õ�include,ע�������Ǵ�˫�������������ַ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            include�������ļ�·��
    const XMLCh* getIncludeString() const;

    //��������
	virtual ~MyIncludeStatement();

private:
    //��ʼ��
	void initialize();                

	//��������ڴ�
    void cleanUp();

	//
    //��������:����include �������ļ�·��,ע�������Ǵ�˫�������������ַ���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������       ��������                    ����
    //includeString  const XMLCh* const          include�������ļ�·�����ַ���,ע�������Ǵ�˫�������������ַ���
	void setIncludeString(const XMLCh* const includeString);

	XMLCh*            fIncludeString;                   //��¼include�����ļ���·������  "file.txt"

    /**
    * Copy constructor
    */
	MyIncludeStatement(const MyIncludeStatement&);
    MyIncludeStatement& operator=(const MyIncludeStatement&);
};

MATCH_CPP_NAMESPACE_END

#endif