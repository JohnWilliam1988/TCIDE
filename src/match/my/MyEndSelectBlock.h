//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-17  Ф�㷶      ���endif���ִ�п�ĳ�����MyEndIFBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDSELECTBLOCK_H
#define MYENDSELECTBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//����������ʶһ��IFִ�п�Ľ�����.
class PARSERS_EXPORT MyEndSelectBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������         ��������                    ����
    //endIfStatement   MyStatement *               endif���
    //id               const unsigned int          ���ID
    //manager          MemoryManager* const        �ڴ������
    MyEndSelectBlock(MyStatement * endSelectStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyEndSelectBlock();
private:
    //��ʼ����
    void initialize();                

    //�����
    void cleanUp();                   

    MyStatement* fEndSelectStatement;                     //endIf��� 

    /**
    * Copy constructor
    */
    MyEndSelectBlock(const MyEndSelectBlock& node);
    MyEndSelectBlock& operator=(const MyEndSelectBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif