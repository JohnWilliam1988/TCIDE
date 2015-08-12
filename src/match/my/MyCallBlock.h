//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���Call���ִ�п�ĳ�����MyCallBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCALLBLOCK_H
#define MYCALLBLOCK_H

#include "BlockList.h"
#include "MyCallStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���������callִ�п� ����� MyCallBlock ������
//callִ�п� ��Ҫ���������ñ���API�������û��Լ������API
class PARSERS_EXPORT MyCallBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //callStatement  MyStatement *               call���
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
    MyCallBlock(MyStatement * callStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyCallBlock();   

    //��������:���غ������ñ���Ե��ַ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            �������ñ�����ַ���
    const XMLCh* getExpressionString() const;

private:
    //��ʼ����
    void initialize();                 

    //�����
    void cleanUp();                   

    MyCallStatement* fCallStatement;                      //Call���

    /**
    * Copy constructor
    */
    MyCallBlock(const MyCallBlock& block);
    MyCallBlock& operator=(const MyCallBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif