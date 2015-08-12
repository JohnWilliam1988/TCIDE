//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���includeԤ���������DependBlockList�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef DEPENDBLOCKLIST_H
#define DEPENDBLOCKLIST_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include "BaseBlock.h"
#include "windows.h"

MATCH_CPP_NAMESPACE_BEGIN

const unsigned int MAXHEADFILELENGTH=256;

//DependBlockListֻ��װincludeBlock��,��Ҫ��������¼����Ŀ�������ļ�
class PARSERS_EXPORT DependBlockList : public XMemory
{
public:
    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //manager       MemoryManager* const        �ڴ������
	DependBlockList(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

	//��������
	~DependBlockList();	

    //��������:���ؿ�����Ŀ�ͷ��ָ��
    //����ֵ::
    //��������                ����
    //const BaseBlock *       ���ؿ��ָ��
	const BaseBlock * GetTheStartBlock() const;	

	//
    //��������:���һ��include�鵽������ȥ
    //����ֵ::
    //��������                ����
    //void                    ��
    //
    //����˵��:
    //��������       ��������              ����
    //block          BaseBlock*            ��ָ��
	void addBlockToList(BaseBlock*  block);


	//
    //��������:���һ��ͷ�ļ���������ȥ
    //����ֵ::
    //��������                ����
    //void                    ��
    //
    //����˵��:
    //��������       ��������              ����
    //file           XMLCh* include       �ļ�·��
	void addIncludeBlockToList(const XMLCh * const file);


	//
    //��������:���һ��com����ĵ���鵽������ȥ
    //����ֵ::
    //��������                ����
    //void                    ��
    //
    //����˵��:
    //��������       ��������              ����
    //progID        XMLCh* include       prog��ID
	//ns            XMLCh* include       ����Ŀռ���
	void addImportBlockToList(const XMLCh * const progID,const XMLCh * const ns,const XMLCh * const file);

	//
    //��������:���һ��include�鵽������ȥ
    //����ֵ::
    //��������                ����
    //bool                    ���ʾ:�Ѵ���,�ٱ�ʾ������
    //
    //����˵��:
    //��������       ��������              ����
    //include          XMLCh* include      �������ļ�·��
	bool isExistIncludeByPath(const XMLCh * const include);

	//
    //��������:�ж�һ��import�Ƿ����
    //����ֵ::
    //��������                ����
    //bool                    ���ʾ:�Ѵ���,�ٱ�ʾ������
    //
    //����˵��:
    //��������       ��������              ����
    //import          XMLCh*              com����
	bool isExistImport(const XMLCh * const id,const XMLCh * const ns);


	//��������:���һ��ͷ�ļ�����������ȥ
    //����ֵ::
    //��������                ����
    //void                    ��
    //
    //����˵��:
    //��������       ��������              ����
    //scannerFile   const XMLCh * const   ��ָ��
	//bool addFileToArray(const XMLCh * const scannerFile);

	//
    //��������:���һ��include�鵽������ȥ
    //����ֵ::
    //��������                ����
    //bool                    ���ʾ:�Ѵ���,�ٱ�ʾ������
    //
    //����˵��:
    //��������       ��������              ����
    //include          XMLCh* include      �������ļ�·��
	//bool isExistScannerFile(const XMLCh * const include);

	//
    //��������:�ӿռ����õ�com id
	//����ֵ:�ҵ��Ŀ�
	//��������                ����
	//bool                   ������ ��ʾID����,���ؼ�L ��ʾID������
	//����˵��:
	//��������          ��������                    ����
	//nameSpace        const XMLCh * const         �����ռ�
	//comID             XMLCh *                    comID
    bool getComIDByNs(const XMLCh * const nameSpace,XMLCh* comID);


	 //�����ڴ�
	void cleanUp();

private:

	//�����ʼ��
	void initialize();

   

	BaseBlock*        fHead;                  //��ͷ
	BaseBlock*        fEnd;                   //��β
	unsigned int      fIDIndex;               //��Ľڵ���
	MemoryManager*    fMemoryManager;         //�ڴ����

	//unsigned int      fScannerFileLength;      //��Ľڵ���
	//XMLCh             fHeadFileList[MAXHEADFILELENGTH][MAX_PATH];

    /**
    * Copy constructor
    */
	DependBlockList(const DependBlockList& node);
	DependBlockList& operator=(const DependBlockList& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif