//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���ifִ�п�ĳ�����MySelectBlock�����
//0002      2010-05-17  Ф�㷶      ȥ��ifִ�п�Ľ������ļ�¼,��Ϊ���Ѿ���¼�ڿ�����Ľ���������
//0003      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·������������addEndIfBlock������Ҳ������кź��ļ�·����������
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYDEFAULTBLOCK_H
#define MYDEFAULTBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyDefaultBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������           ��������                    ����
    //parentBlock        BaseBlock *                 �����ָ��
    //ifStatement        MyStatement *               if����ͷ���
    //id                 const unsigned int          ���ID
    //manager            MemoryManager* const        �ڴ������
    MyDefaultBlock(BaseBlock * parentBlock
        ,MyStatement * selectStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyDefaultBlock();                   

    //��������:�õ�if������Ŀ������ͷ�ڵ�
    //����ֵ::
    //��������                    ����
    //const BaseBlock*            �������ͷ�ڵ�
    const BaseBlock* GetStartBlock() const;

    //��������:��if������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //block         BaseBlock *                 ִ�п�ָ��
    void addBlock(BaseBlock * block);

    //��������:��default������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                     ����
    void addEndBlock();

    //��������:����if�������ִ�п������
    //����ֵ::
    //��������                       ����
    //const BlockList*               ������ָ��
    const BlockList * GetBlockList() const;

    //��������:�õ�����
    //����ֵ::
    //��������                ����
    //const BaseBlock*        ����ָ��
    const BaseBlock* GetParentBlock() const;
private:
    //��ʼ����
    void initialize();                 
    //�����
    void cleanUp();                    

    bool isSetEnd;                     //��bool������ʶ���ǲ����Ѿ�������IF�������

    MyStatement* fSelectStatement;      //IF��ͷ

    BlockList *  fBlockList;           //�����Ŀ�����

    BaseBlock *  fParentBlock;         //��¼����

    /**
    * Copy constructor
    */
    MyDefaultBlock(const MyDefaultBlock& node);
    MyDefaultBlock& operator=(const MyDefaultBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif