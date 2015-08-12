#if !defined(ABSBLOCKPARSER_H)
#define ABSBLOCKPARSER_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <match/framework/XMLErrorReporter.hpp>
#include <match/sax/ErrorHandler.hpp>
#include "BaseBlock.h"
#include "MyError.h"
#include <match/util/XMLUniDefs.hpp>
//�ӱ� 10-8-9 ������Թ�����Ҫ�ı����б��ͷ�ļ�
#include"MyVarListContainer.h"
//�ӱ� 10-8-9 ������Թ�����Ҫ�ı����б��ͷ�ļ�

MATCH_CPP_NAMESPACE_BEGIN

//ִ�п�ĳ�����
class PARSERS_EXPORT AbsBlockParser
{
public:
	//���캯��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	//Block         const BaseBlock *           ģ���ָ��
    //manager       MemoryManager* const        �ڴ������
	AbsBlockParser(const XMLCh* const    nameSpace
		,const BaseBlock *  Block
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
	
    //����
    ~AbsBlockParser();

	//�����������õ������ռ�
	//����ֵ:   XMLCh*
    //��������                ����
    //const XMLCh*            �����ռ���
    //����˵��:
    //��
	const XMLCh* getSpaceName() const;
protected:

	//��ʼ��
	void initialize();                              

	//�������
	void cleanUp();

	//�������������������ռ���
	//����ֵ:   ��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������      ��������                    ����
	//nameSpace     const XMLCh* const          �����ռ���
	void setSpaceName(const XMLCh* const nameSpace);

	//
    //�����������õ������Ŀ�ָ��
	//����ֵ:   ������Ŀ�ָ��
    //��������                ����
    //BaseBlock*              ������Ŀ�ָ��
    //����˵��:
    //��
	BaseBlock* getBlock() const;

//���԰汾���������ķ���
#ifdef TC_DEBUG
    //�ӱ� 10-8-9 �����ж�״̬�ķ���
    //���������� �жϵ���״̬��
    void debug(const XMLCh* const namesp, MyVarListContainer* VarList);
    //�ӱ� 10-8-9 �����ж�״̬�ķ���
#endif

	bool getIsStop() const;
	bool isGoto(const BaseBlock* startBlock,/*MyVarList * const VarList,MyVarListContainer* VarListCon,*/const BaseBlock** gotoBlock) const;

	MemoryManager*              fMemoryManager;	 	//�ڴ�����Ա
	BaseBlock *                 fBlock;             //�����Ŀ�,�ڴ治���Լ�����             
	XMLCh                       fNameSpace[NAMEMAXLENGTH];         //���ڵ������ռ� 

	//CCF �������캯��
	AbsBlockParser(const AbsBlockParser& pool);
	AbsBlockParser& operator=(const AbsBlockParser& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif
