//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���functionִ�п�ĳ�����MyFunctionBlock�����
//0002      2010-05-17  Ф�㷶      ȥ����function�������ļ�¼,��Ϊ���Ѿ���¼�ڿ������������
//0003      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·������������addEndFunctionBlock������Ҳ������кź��ļ�·����������
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYFUNCTIONBLOCK_H
#define MYFUNCTIONBLOCK_H

#include "BlockList.h"
#include "MyFunctionHeadStatement.h"

#include "MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

//���ǰѺ���ͷ�Ķ��� ����� MyFunctionBlock
class PARSERS_EXPORT MyFunctionBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������           ��������                    ����
    //parentBlock        BaseBlock *                 �����ָ��
    //functionStatement  MyStatement *               function����ͷ���
    //id                 const unsigned int          ���ID
    //manager            MemoryManager* const        �ڴ������
	MyFunctionBlock(BaseBlock *  parentBlock
	         ,MyStatement * functionStatement
		     ,const unsigned int    id = 0
			 ,const unsigned int    linenum=0
			 ,const XMLCh*          filepath=NULL
		     ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
	~MyFunctionBlock();                   

    //��������:�õ�����������Ŀ������ͷ�ڵ�
    //����ֵ::
    //��������                    ����
    //const BaseBlock*            �������ͷ�ڵ�
	const BaseBlock* GetStartBlock() const;

    //��������:���������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //block         BaseBlock *                  ִ�п�ָ��
    void addBlock(BaseBlock * block);


    //��������:��Ӻ����������,�����ڲ�ʵ����һ��������������������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //end         MyStatement *                 �����������ָ��
	//linenum	  const unsigned int            endfunction���к�
	//filepath    const XMLCh*                  �ļ���·��
	void addEndFunctionBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);          

    //��������:���غ�����
    //����ֵ::
    //��������                ����
    //const XMLCh*            �������ַ���
	const XMLCh* getFunctionNameString() const;

	//��������:���غ����Ĳ����б�
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   �õ������Ĳ����б�
	const XMLCh* getFunctionPramString() const;


	//��������:���غ����������ִ�п������
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

	bool isSetEnd;                               //��bool������ʶ���ǲ����Ѿ�������Function������

	BaseBlock *  fParentBlock;                   //��¼����

	MyFunctionHeadStatement* fFunctionStatement; //Function��ͷ

	BlockList *  fBlockList;                     //�����Ŀ�����

    /**
    * Copy constructor
    */
	MyFunctionBlock(const MyFunctionBlock& node);
    MyFunctionBlock& operator=(const MyFunctionBlock& toAssign);
};

MATCH_CPP_NAMESPACE_END



#endif