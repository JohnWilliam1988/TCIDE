//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���namespaceִ�п�ĳ�����MyNameSpaceBlock�����
//0002      2010-07-16  �ӱ�        �ڹ��캯��������˱�����кź��ļ�·������������addEndNameSpaceBlock������Ҳ������кź��ļ�·����������
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYNAMESPACEBLOCK_H
#define MYNAMESPACEBLOCK_H

#include "BlockList.h"
#include "MyNameSpaceStatement.h"

MATCH_CPP_NAMESPACE_BEGIN

//namespace�����ռ������
class PARSERS_EXPORT MyNameSpaceBlock :  public BaseBlock
{
public:
    //���캯��
    //����˵��:
    //��������         ��������                    ����
    //NSStatement      MyStatement *               namespace���
    //id               const unsigned int          ���ID
    //manager          MemoryManager* const        �ڴ������
    MyNameSpaceBlock(MyStatement * NSStatement
        ,const unsigned int    id = 0
		,const unsigned int    linenum=0
		,const XMLCh*          filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������
    ~MyNameSpaceBlock();                   

    //��������:�õ�namespace������Ŀ������ͷ�ڵ�
    //����ֵ::
    //��������                    ����
    //const BaseBlock*            �������ͷ�ڵ�
    const BaseBlock* GetStartBlock() const;

    //��������:��namespace������Ŀ��������һ����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //block         BaseBlock *                 ִ�п�ָ��
    void addBlock(BaseBlock * block);

    //��������:���namespace�������,�����ڲ�ʵ����һ��namespace��������������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷���
    //
    //����˵��:
    //��������      ��������                    ����
    //end         MyStatement *                 namespace�������ָ��
	//linenum	  const unsigned int            endnamespace���к�
	//filepath    const XMLCh*                  �ļ���·��
    void addEndNameSpaceBlock(MyStatement * end,const unsigned int linenum,const XMLCh* filepath);

    //��������:�õ������ռ�����
    //����ֵ::
    //��������                       ����
    //const XMLCh*                   ���������
    const XMLCh* getSapceNameString() const;

    //��������:����namespace�������ִ�п������
    //����ֵ::
    //��������                       ����
    //const BlockList*               ������ָ��
    const BlockList * GetBlockList() const;
private:
    //��ʼ����
    void initialize();                           

    //�����
    void cleanUp();                              

    bool isSetEnd;                                //��bool������ʶ���ǲ����Ѿ�������namespace�������
    MyNameSpaceStatement* fNameSpaceStatement;             //namespace��ͷ
    /*MyStatement* fEndNameSpaceStatement;        //namespace��β*/
    BlockList *  fBlockList;                      //�����Ŀ�����

    /**
    * Copy constructor
    */
    MyNameSpaceBlock(const MyNameSpaceBlock& node);
    MyNameSpaceBlock& operator=(const MyNameSpaceBlock& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif

