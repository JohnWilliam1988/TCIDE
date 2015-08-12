//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���Call���ִ�п�ĳ�����MyCallBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYGOTOBLOCK_H
#define MYGOTOBLOCK_H

#include "BlockList.h"
#include "MyGotoStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���������callִ�п� ����� MyCallBlock ������
//callִ�п� ��Ҫ���������ñ���API�������û��Լ������API
class PARSERS_EXPORT MyGotoBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //callStatement  MyStatement *               call���
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
    MyGotoBlock(MyStatement * gotoStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyGotoBlock();   

    //��������:���غ������ñ���Ե��ַ���
    //����ֵ::
    //��������                ����
    //const XMLCh*            �������ñ�����ַ���
    const XMLCh* getGotolable() const;

private:
    //��ʼ����
    void initialize();                 

    //�����
    void cleanUp();                   

    MyGotoStatement* fGotoStatement;                      //Call���

    /**
    * Copy constructor
    */
    MyGotoBlock(const MyGotoBlock& block);
    MyGotoBlock& operator=(const MyGotoBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif