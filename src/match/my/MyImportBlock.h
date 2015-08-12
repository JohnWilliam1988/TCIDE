//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���#includeԤִ�п�ĳ�����MyImportBlock�����
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYIMPORTBLOCK_H
#define MYIMPORTBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//#importԤ����ִ�п�ĳ�����
class PARSERS_EXPORT MyImportBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������                ��������                    ����
    //importStatement         MyStatement *               #import���
    //id                      const unsigned int          ���ID
    //manager                 MemoryManager* const        �ڴ������
	MyImportBlock(MyStatement * importStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //����
	~MyImportBlock();                   

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

private:
    //��ʼ����
	void initialize();                      

    //�����
	void cleanUp();                       

	MyStatement* fImportStatement;                       //Import���

    /**
    * Copy constructor
    */
	MyImportBlock(const MyImportBlock& node);
    MyImportBlock& operator=(const MyImportBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif