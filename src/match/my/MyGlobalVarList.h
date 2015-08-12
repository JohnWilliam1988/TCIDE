/////////////////////////////////2009.12.16  Ф�㷶�޶�//////////////////////////

#ifndef MYGLOBALVARLIST_H
#define MYGLOBALVARLIST_H

#include"MyVar.h"
#include"MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

//������ʾȫ�ֱ����б�ڵ�
class PARSERS_EXPORT  GlobalVarNode 
{
public:
    //
    //��������:���캯��                
    //����˵��:
    //��������              ��������                    ����
    //memoryManager         MemoryManager* const        �ڴ�����Ա
	GlobalVarNode();

    //��������
	~GlobalVarNode();

    //��������:������һ��ȫ������ڵ�
    //
    //����ֵ:
    //��������                              ����
    //void                                  �޷�������
    //
    //����˵��:
    //��������      ��������                ����
    //node          GlobalVarNode * const   ȫ������ڵ�
	void setNextNode(GlobalVarNode * const node);

    //��������:�õ���һ���ڵ�
    //
    //����ֵ:
    //��������                              ����
    //GlobalVarNode *                       ȫ������ڵ�
    //
    GlobalVarNode *getNextNode() const;

    //��������:���������ռ�
    //
    //����ֵ:
    //��������                              ����
    //void                                  �޷�������
    //
    //����˵��:
    //��������      ��������                ����
    //spName        const XMLCh* const      �ռ���
	void setNameSpace(const XMLCh* const spName);

    //��������:�õ������ռ�
    //
    //����ֵ:
    //��������                              ����
    //const XMLCh *                         �õ��ռ���
	const XMLCh *getNameSpace() const;

	//�õ��ڵ�ı����б�
	MyVarList* getVarList();//const;
private:

	void initialize();
	void cleanUp();

    MyVarList             fList;                    //�����б�
	GlobalVarNode *       fNext;                    //��һ���ڵ��ָ��
	XMLCh                 fNameSpace[NAMEMAXLENGTH];//�ռ���
};

//MyGlobalVarListManager��Ҫ�������ں͹���ȫ�ֱ���
class PARSERS_EXPORT  MyGlobalVarListManager
{
public:
    //��������:���캯��                
    //����˵��:
    //��������              ��������                    ����
    //memoryManager         MemoryManager* const        �ڴ�����Ա
    MyGlobalVarListManager();

    //��������
    ~MyGlobalVarListManager();

    //��������:���һ��ȫ�ֱ����б�ڵ�
    //
    //����ֵ:
    //��������                            ����
    //void                                �޷�������
    //
    //����˵��:
    //��������      ��������              ����
    //list           GlobalVarNode *      ȫ�ֱ����б�ڵ�
	void appendNode(GlobalVarNode *list);

    //��������:ͨ���ռ������ҵ�ȫ�ֱ����б�
    //
    //����ֵ:
    //��������                            ����
    //MyVarList *                         ȫ�ֱ����б�
    //
    //����˵��:
    //��������      ��������              ����
    //nameSpace     const XMLCh * const   �ռ���
    MyVarList *findListByNameSpace(const XMLCh * nameSpace);

    //��������:�õ�ȫ�ֱ������ͷ
    //����ֵ::
    //��������                            ����
    //GlobalVarNode *                    ȫ�ֱ������ͷ
    GlobalVarNode *getListHead();

    //��������:�õ�ȫ�ֱ������β
    //����ֵ::
    //��������                            ����
    //GlobalVarNode *                    ȫ�ֱ������β
	GlobalVarNode *getListEnd();

    //��������:����һ��ȫ�ֱ�����ֵ
    //
    //����ֵ:
    //��������                            ����
    //bool                                true:���óɹ�,false:����ʧ��
    //
    //����˵��:
    //��������      ��������              ����
    //nameSpace     const XMLCh * const   �ռ���
    //varName       const XMLCh * const   ������
    //value         XMLCh *               ֵ���ַ���������ʽ
    //bool setVarValue(const XMLCh * const nameSpace,const XMLCh* const varName,XMLCh *value,VARTYPE type);

	  //����
	void cleanUp();

	//�����ʼ��
	static void initializeStaticData();
	
    //�����ʼ��
	static void cleanUpStaticData();
private:
	//�����ʼ��
	void initialize();
  
	GlobalVarNode *           fListHead;                 //ͷ�ڵ�
	GlobalVarNode *           fListEnd;                  //β�ڵ�
};

MATCH_CPP_NAMESPACE_END

#endif