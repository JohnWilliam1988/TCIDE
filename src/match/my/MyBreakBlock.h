//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ���break���ִ�п�ĳ�����MyBreakBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYBREAKBLOCK_H
#define MYBREAKBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���������break ִ�п� ����� BaseBlock �ļ̳���
class PARSERS_EXPORT MyBreakBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //breakState  MyStatement *                 break���
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
    MyBreakBlock(MyStatement * breakState
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyBreakBlock();
private:
    //��ʼ����
    void initialize();        

    //����
    void cleanUp();                    

    MyStatement* fBreakStatement;                           //break���

    /**
    * Copy constructor
    */
    MyBreakBlock(const MyBreakBlock& node);
    MyBreakBlock& operator=(const MyBreakBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif