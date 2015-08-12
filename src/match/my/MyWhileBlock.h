//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���whileִ�п�ĳ�����MyWhileBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·������������addEndWhileBlock������Ҳ������кź��ļ�·����������
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYWHILEBLOCK_H
#define MYWHILEBLOCK_H

#include "BlockList.h"
#include "MyWhileStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//�����ǳ���� while ִ�п�
class PARSERS_EXPORT MyWhileBlock : public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������           ��������                    ����
    //parentBlock        BaseBlock *                 �����ָ��
    //whileStatement     MyStatement *               while�������
    //id                 const unsigned int          ���ID
    //manager            MemoryManager* const        �ڴ������
	MyWhileBlock(BaseBlock * parentBlock
		        ,MyStatement * whileStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //����
	~MyWhileBlock();                   

    //��������:�����Ƿ���while������Ŀ������ͷ
    //����ֵ::
    //��������                    ����
    //const BaseBlock*            �������ͷ�ڵ�
	const BaseBlock* GetStartBlock() const;

    //��������:��while������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //block         BaseBlock *                 ִ�п�ָ��
	void addBlock(BaseBlock * block);

    //��������:���while�������,�����ڲ�ʵ����һ��while��������������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //end         MyStatement *                 while�������ָ��
	//linenum	  const unsigned int            endwhile���к�
	//filepath    const XMLCh*                  �ļ���·��
	void addEndWhileBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);

    //��������:����while�������ִ�п������
    //����ֵ::
    //��������                       ����
    //const BlockList*               ������ָ��
    const BlockList * GetBlockList() const;

    //��������:�õ�����
    //����ֵ::
    //��������                ����
    //const BaseBlock*        ����ָ��
	const BaseBlock* GetParentBlock() const;

    //��������:����while�����������
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   ���������
	const XMLCh* getExpressionString() const;

private:
    //��ʼ����
	void initialize();                 

    //�����
	void cleanUp();                    

	bool isSetEnd;                          //��bool������ʶ���ǲ����Ѿ�������While�������

	BaseBlock *  fParentBlock;              //��¼����

	MyWhileStatement* fWhileStatement;           //Function��ͷ��� 

	/*MyStatement* fEndWhileStatement;      //Function��β���*/

    BlockList *  fBlockList;                //�����Ŀ�����
 

	MyWhileBlock(const MyWhileBlock& node);
    MyWhileBlock& operator=(const MyWhileBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif