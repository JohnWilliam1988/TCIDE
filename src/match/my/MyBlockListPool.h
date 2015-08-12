//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-16  Ф�㷶      ��ɿ�������MyBlockListPool�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYBLOCKLISTPOOL_H
#define MYBLOCKLISTPOOL_H

#include "BlockList.h"
#include "MyNameSpaceBlock.h"

MATCH_CPP_NAMESPACE_BEGIN


//�����صĽڵ�
struct PARSERS_EXPORT ListMgr
{
	MyNameSpaceBlock * nameSpace; //�����ռ�
	ListMgr * pre;                //��һ��
	ListMgr * next;               //��һ��

    //�ù��캯������ʼ�� 
    ListMgr()
    {
        nameSpace=NULL;
        pre=NULL;
        next=NULL;
    }
};

//�����,������������洢�������,��Ҫ���ڽ���Դ�ļ���ʱ��,���Դ�ļ����е�Դ���뷭���ִ�п�,
//Ȼ��ִ�п鶼��ϳɿ�����,Ȼ��ѿ�������������������

class PARSERS_EXPORT MyBlockListPool : public XMemory
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
    //manager       MemoryManager* const        �ڴ������
	MyBlockListPool(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//��������
	~MyBlockListPool();

	//�����ʼ��
	void initialize();

    //��������:���һ�������ռ䵽�����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������       ��������                    ����
    //toAdd          ListMgr *                   �ڵ�ָ��
	void addListToPool(ListMgr * toAdd);

	//
    //��������:���������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
	void delListPool();


    //��������:�õ������ͷ
    //����ֵ::
    //��������                ����
    //const ListMgr*          ͷָ��
	const ListMgr* getPoolHead() const;

	//
    //��������:ͨ���ռ�����������ڵ�
    //
    //����ֵ:
    //��������                ����
    //ListMgr *               �ɹ�:���ز鵽�Ľڵ�ָ��. ʧ��:����NULL
    //
    //����˵��:
    //��������      ��������                    ����
    //toFind          const XMLCh* const        �ռ���
	ListMgr * findListByNamespce(const XMLCh * const  toFind);

protected:

private:
	MemoryManager*      fMemoryManager;	            //�ڴ�����Ա
	ListMgr *              fListHead;               //�����ͷ�ڵ�
	ListMgr *              fListEnd;                //�����β�ڵ�

	//���������
	void cleanUp();	

    /**
    * Copy constructor
    */
	MyBlockListPool(const MyBlockListPool& pool);
	MyBlockListPool& operator=(const MyBlockListPool& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif 























