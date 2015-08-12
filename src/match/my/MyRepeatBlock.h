//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���repeatִ�п�ĳ�����MyRepeatBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·������������addEndRepeatBlock������Ҳ������кź��ļ�·����������
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYREPEATBLOCK_H
#define MYREPEATBLOCK_H

#include "BlockList.h"
#include "MyStatement.h"
#include "MyRepeatStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//repeatִ�п�ĳ���
class PARSERS_EXPORT MyRepeatBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������           ��������                    ����
    //parentBlock        BaseBlock *                 �����ָ��
    //repeatStatement    MyStatement *               repeat����ͷ���
    //id                 const unsigned int          ���ID
    //manager            MemoryManager* const        �ڴ������
    MyRepeatBlock(BaseBlock * parentBlock
        ,MyStatement * repeatStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyRepeatBlock();  

    //��������:�õ�repeat������Ŀ������ͷ�ڵ�
    //����ֵ::
    //��������                    ����
    //const BaseBlock*            �������ͷ�ڵ�
    const BaseBlock* GetStartBlock() const;

    //��������:��repeat������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //block         BaseBlock *                 ִ�п�ָ��
    void addBlock(BaseBlock * block);

    //��������:���repeat�������,�����ڲ�ʵ����һ��repeat��������������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //end         MyStatement *                 repeat�������ָ��
	//linenum	  const unsigned int            endrepeat���к�
	//filepath    const XMLCh*                  �ļ���·��
    void addEndRepeatBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);

    //��������:����repeat�����������
    //����ֵ::
    //��������                       ����
    //const BlockList*               ������ָ��
    const BlockList * GetBlockList() const;

    //��������:�õ�����
    //����ֵ::
    //��������                ����
    //const BaseBlock*        ����ָ��
    const BaseBlock* GetParentBlock() const;

    //
    //��������:����repeat���ѭ���Ĵ���(һ���Ǹ�������)
    //����ֵ::
    //��������                       ����
    //unsigned int                   repeatѭ���Ĵ���
    //unsigned int getRepeatNum() const;
	bool isContant();

	unsigned int getContantNum();

	const XMLCh* getVarName();

	const XMLCh* getInitValue();

	const XMLCh* getExp();

	const XMLCh* getStep();

private:
    //��ʼ����
    void initialize();                 

    //�����
    void cleanUp();                   

    bool isSetEnd;                     //��bool������ʶ���ǲ����Ѿ�������repeat�������

    BaseBlock *       fParentBlock;    //��¼����

    MyRepeatStatement* fRepeatStatement;         //repeat���

    /*MyStatement* fEndRepeatStatement;      //endrepeat���*/

    BlockList *  fBlockList;            //�����Ŀ�����

    /**
    * Copy constructor
    */
    MyRepeatBlock(const MyRepeatBlock& node);
    MyRepeatBlock& operator=(const MyRepeatBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif