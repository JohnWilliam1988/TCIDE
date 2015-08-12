//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���selectִ�п�ĳ�����MySelectBlock�����
//0002      2010-05-17  Ф�㷶      ȥ��selectִ�п�Ľ������ļ�¼,��Ϊ���Ѿ���¼�ڿ�����Ľ���������
//0003      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·������������addEndSelectBlock������Ҳ������кź��ļ�·����������
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYCASEBLOCK_H
#define MYCASEBLOCK_H

#include "BlockList.h"
#include "MyCaseStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyCaseBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������           ��������                    ����
    //parentBlock        BaseBlock *                 �����ָ��
    //ifStatement        MyStatement *               if����ͷ���
    //id                 const unsigned int          ���ID
    //manager            MemoryManager* const        �ڴ������
    MyCaseBlock(BaseBlock * parentBlock
        ,MyStatement * selectStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyCaseBlock();                   

    //��������:�õ�select������Ŀ������ͷ�ڵ�
    //����ֵ::
    //��������                    ����
    //const BaseBlock*            �������ͷ�ڵ�
    const BaseBlock* GetStartBlock() const;

    //��������:��select������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                     ����
    //block         BaseBlock *                 ִ�п�ָ��
    void addBlock(BaseBlock * block);

    //��������:��select������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                     ����
    void addEndBlock();

    //��������:����select�������ִ�п������
    //����ֵ::
    //��������                       ����
    //const BlockList*               ������ָ��
    const BlockList * GetBlockList() const;

    //��������:�õ�����
    //����ֵ::
    //��������                ����
    //const BaseBlock*        ����ָ��
    const BaseBlock* GetParentBlock() const;

    //��������:����select�����������
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   ���������
    const XMLCh* getExpressionString() const;
private:
    //��ʼ����
    void initialize();                 
    //�����
    void cleanUp();                    

    bool isSetEnd;                     //��bool������ʶ���ǲ����Ѿ�������IF�������

    MyCaseStatement* fCaseStatement;  //case��ͷ

    BlockList *  fBlockList;           //�����Ŀ�����

    BaseBlock *  fParentBlock;         //��¼����

    /**
    * Copy constructor
    */
    MyCaseBlock(const MyCaseBlock& node);
    MyCaseBlock& operator=(const MyCaseBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif