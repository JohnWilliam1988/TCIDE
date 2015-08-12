//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���Call���ִ�п�ĳ�����MyCallBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYLABLEBLOCK_H
#define MYLABLEBLOCK_H

#include "BlockList.h"
#include "MyLableStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���������callִ�п� ����� MyCallBlock ������
//callִ�п� ��Ҫ���������ñ���API�������û��Լ������API
class PARSERS_EXPORT MyLableBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //callStatement  MyStatement *               call���
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
    MyLableBlock(MyStatement * lableStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyLableBlock();   

    //��������:���غ������ñ���Ե��ַ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            �������ñ�����ַ���
    const XMLCh* getLable() const;

private:
    //��ʼ����
    void initialize();                 

    //�����
    void cleanUp();                   

    MyLableStatement* fLableStatement;                      //Call���

    /**
    * Copy constructor
    */
    MyLableBlock(const MyLableBlock& block);
    MyLableBlock& operator=(const MyLableBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif