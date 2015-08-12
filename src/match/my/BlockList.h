//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ��ɿ����������BlockList�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef BLOCKLIST_H
#define BLOCKLIST_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "BaseBlock.h"

MATCH_CPP_NAMESPACE_BEGIN

//������,�������������Ҫ�������洢һϵ��ִ�п��,
class PARSERS_EXPORT BlockList : public XMemory
{
public:
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //manager       MemoryManager* const        �ڴ������
	BlockList(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//��������
	~BlockList();	

    //��������:���ؿ�����Ŀ�ͷ��ָ��
    //����ֵ::
    //��������                ����
    //const BaseBlock *       ���ؿ��ָ��
	const BaseBlock * GetTheStartBlock() const;	

    //��������:���ؿ�����Ŀ�β��ָ��
    //����ֵ::
    //��������                ����
    //const BaseBlock *       ���ؿ��ָ��
	const BaseBlock * GetTheEndBlock() const;	

	//
    //��������:���һ���鵽������ȥ
    //����ֵ::
    //��������                ����
    //void                    ��
    //
    //����˵��:
    //��������       ��������              ����
    //block          BaseBlock*            ��ָ��
	void AddBlockToList(BaseBlock*  block);

private:
	//�����ʼ��
	void initialize();

    //�����ڴ�
	void cleanUp();

	BaseBlock*        fHead;                  //����ͷ
	BaseBlock*        fEnd;                   //����β
	unsigned int      fIDIndex;               //�������
	MemoryManager*    fMemoryManager;         //�ڴ����

    /**
    * Copy constructor
    */
	BlockList(const BlockList& node);
	BlockList& operator=(const BlockList& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif