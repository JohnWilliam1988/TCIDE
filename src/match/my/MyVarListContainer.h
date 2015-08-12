/////////////////////////////////2009.12.19  Ф�㷶�޶�//////////////////////////

#ifndef MYVARLISTCONTAINER_H
#define MYVARLISTCONTAINER_H

#include "MyVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

//�����б��һ������,����������������ڴ�,ֻ�����������б���ӿ鴫�ݵ�ʱ��
//�ѱ����б�����װ��һ��,Ȼ�󴫸��ӿ�,�����Ͳ���Ӱ�츸���еı����б�Ľṹ
class PARSERS_EXPORT MyVarListContainer
{
public:
    //
    //��������:���캯��                
    //����˵��:
    //��������              ��������                    ����
    //memoryManager         MemoryManager* const        �ڴ�����Ա
	MyVarListContainer();

    //��������
	~MyVarListContainer();
	
    //�ڸ���һ�������� �����в�ѡ һ������
    //��������:�����������һ���ڵ�
    //
    //����ֵ:
    //��������                ����
    //MyVar *                 ����ָ��
    //
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh * const         ������
	MyVar *findVarByName(const XMLCh *const name);


	//�ڸ���һ����������һ�������±��������Ա�ľ��
    //��������:�����������һ���ڵ�
    //
    //����ֵ:
    //��������                ����
    //MyVar *                 ����ָ��
    //
    //����˵��:
    //��������      ��������                    ����
    //name         const XMLCh * const         ������
	//int          ������±�                   ...
    //MyVar *findArraySubVarByName(const XMLCh * const name,int c1,int c2=-1,int c3=-1);

    //�ж�һ�������Ƿ����
    bool VarNameExist(const XMLCh *const name) const;

    // 
    //��������:���ر��������ͷ�ڵ�
    //����ֵ::
    //��������                            ����
    //const varNode *                     �����б��ͷ�ڵ�
	varNode *getListHead() const;

    // 
    //��������:���ر��������β�ڵ�
    //����ֵ::
    //��������                            ����
    //const varNode *                     �����б��β�ڵ�
	varNode *getListEnd();

    //��������:����һ������������������ȥ
    //
    //����ֵ:
    //��������                ����
    //void                    ��
    //
    //����˵��:
    //��������      ��������                    ����
    //list          const MyVarList * const     �����б�ָ��
	void catVarList(const MyVarList * const list);

    //��������:��һ�������������ӵ�������������ȥ
    //
    //����ֵ:
    //��������                ����
    //void                    ��
    //
    //����˵��:
    //��������      ��������                             ����
    //list          const MyVarListContainer * const     �����б�����ָ��
	void catVarList(const MyVarListContainer * const list);

    //��������:���ݱ�����,�޸ı�����ֵ
    //
    //����ֵ:
    //��������                ����
    //bool                    true��ʾ�޸ĳɹ�,false��ʾ�޸�ʧ��
    //
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          ������
    //value         const XMLCh* const          ֵ���ַ���������ʽ
    //bool setVarValueByName(const XMLCh * const  name, const  XMLCh * const  value,VARTYPE type);

    //��������:���ݱ�����,�޸ı�����ֵ
    //
    //����ֵ:
    //��������                ����
    //bool                    true��ʾ�޸ĳɹ�,false��ʾ�޸�ʧ��
    //
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          ������
    //value         const MyValue* const        ֵ��MyValue���͵�ָ��
    bool setVarValueByName(const XMLCh * const  name,const  _variant_t * const  value);

    //��������:�����������һ������
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //var           varNode *                   �����ڵ�
	void appendVar(varNode *var);

private:

	//�����ʼ��
	void initialize();

    //����
	void cleanUp();

	varNode *           fVarHead;                      //�����б�ͷ�ڵ�
	varNode *           fVarEnd;                       //�����б�β�ڵ�

    /**
    * Copy constructor
    */
    MyVarListContainer(const MyVarListContainer& node);
    MyVarListContainer& operator=(const MyVarListContainer& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif