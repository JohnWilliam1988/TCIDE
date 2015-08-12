//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-17  Ф�㷶      ���endfunction���ִ�п�ĳ�����MyEndFunctionBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYENDFUNCTIONBLOCK_H
#define MYENDFUNCTIONBLOCK_H

#include "BaseBlock.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//����ֻ������ʶһ������ִ�п�Ľ�����
class PARSERS_EXPORT MyEndFunctionBlock :  public BaseBlock
{
public:

    //���캯��
    //����˵��:
    //��������         ��������                    ����
    //endFunStatement  MyStatement *               endfunction���
    //id               const unsigned int          ���ID
    //manager          MemoryManager* const        �ڴ������
    MyEndFunctionBlock(MyStatement * endFunStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyEndFunctionBlock();
private:
    //��ʼ����
    void initialize();                 

    //�����
    void cleanUp();                   

    MyStatement* fEndFunStatement;         //endFunction��ͷ��� 

    /**
    * Copy constructor
    */
    MyEndFunctionBlock(const MyEndFunctionBlock& node);
    MyEndFunctionBlock& operator=(const MyEndFunctionBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif