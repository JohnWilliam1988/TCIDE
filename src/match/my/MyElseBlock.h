//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-17  Ф�㷶      ���else���ִ�п�ĳ�����MyElseBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·����������
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYELSEBLOCK_H
#define MYELSEBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//���������ǳ������ else ִ�п�
class PARSERS_EXPORT MyElseBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������       ��������                    ����
    //parentBlock    BaseBlock *                 �����ָ��
    //elseStatement  MyStatement *               else���
    //id             const unsigned int          ���ID
    //manager        MemoryManager* const        �ڴ������
    MyElseBlock(BaseBlock * parentBlock
        ,MyStatement * elseStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyElseBlock();

    //��������:�õ�else������Ŀ������ͷ�ڵ�
    //����ֵ::
    //��������                    ����
    //const BaseBlock*            �������ͷ�ڵ�
    const BaseBlock* GetStartBlock() const;

    //��������:��else������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //block         BaseBlock *                 ִ�п�ָ��
    void addBlock(BaseBlock * block);

    //��������:���else�������,�����ڲ�ʵ����һ��else��������������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //end         MyStatement *                 if�������ָ��
    //linenum	  const unsigned int            endif���к�
    //filepath    const XMLCh*                  �ļ���·��
    void addEndIfBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);

    //��������:����else�������ִ�п������
    //����ֵ::
    //��������                       ����
    //const BlockList*               ������ָ��
    const BlockList * GetBlockList() const;

    //��������:�õ�����
    //����ֵ::
    //��������                ����
    //const BaseBlock*        ����ָ��
    const BaseBlock* GetParentBlock() const;

    //��������:�����Ѿ�����endif����,������Ϊ��else���,����ֻ���Ƿ��Ѿ����˽�β,����Ҫ��¼������
    //����ֵ::
    //��������                ����
    //void                    ��
    void setIfEnd();

private:
    //��ʼ����
    void initialize();                 

    //�����
    void cleanUp();                   

    bool isSetEnd;                      //��bool������ʶ���ǲ����Ѿ�������IF�������

    MyStatement* fElseStatement;         //else���

    BlockList *  fBlockList;             //�����Ŀ�����

    BaseBlock *  fParentBlock;           //��¼����

    /**
    * Copy constructor
    */
    MyElseBlock(const MyElseBlock& node);
    MyElseBlock& operator=(const MyElseBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif