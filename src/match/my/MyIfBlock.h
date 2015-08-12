//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���ifִ�п�ĳ�����MyIfBlock�����
//0002      2010-05-17  Ф�㷶      ȥ��ifִ�п�Ľ������ļ�¼,��Ϊ���Ѿ���¼�ڿ������Ľ���������
//0003      2010-07-16  �ӱ�        �ڹ��캯���������˱�����кź��ļ�·������������addEndIfBlock������Ҳ�������кź��ļ�·����������
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYIFBLOCK_H
#define MYIFBLOCK_H

#include "BlockList.h"
#include "MyIfStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

class PARSERS_EXPORT MyIfBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������           ��������                    ����
    //parentBlock        BaseBlock *                 �����ָ��
    //ifStatement        MyStatement *               if����ͷ���
    //id                 const unsigned int          ���ID
    //manager            MemoryManager* const        �ڴ������
    MyIfBlock(BaseBlock * parentBlock
        ,MyStatement * ifStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyIfBlock();                   

    //��������:�õ�if������Ŀ�������ͷ�ڵ�
    //����ֵ::
    //��������                    ����
    //const BaseBlock*            ��������ͷ�ڵ�
    const BaseBlock* GetStartBlock() const;

    //��������:��if������Ŀ���������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //block         BaseBlock *                 ִ�п�ָ��
    void addBlock(BaseBlock * block);

    //��������:����if�������,�����ڲ�ʵ����һ��if����������������
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

    //��������:����if������������
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   ����������
    const XMLCh* getExpressionString() const;
private:
    //��ʼ����
    void initialize();                 
    //������
    void cleanUp();                    

    bool isSetEnd;                     //��bool������ʶ���ǲ����Ѿ�������IF�������

    MyIfStatement* fIfStatement;         //IF��ͷ

    BlockList *  fBlockList;           //�����Ŀ�����

    BaseBlock *  fParentBlock;         //��¼����

    /**
    * Copy constructor
    */
    MyIfBlock(const MyIfBlock& node);
    MyIfBlock& operator=(const MyIfBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif