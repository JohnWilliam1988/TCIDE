/////////////////////////////////2009.12.19  Ф�㷶�޶�//////////////////////////

#ifndef MYVARLIST_H
#define MYVARLIST_H

#include"MyVar.h"
#include"match/My/MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

//�����˱�������Ľڵ�����
struct varNode
{
    MyVar   *             var;                  //����ָ��
	varNode *             next;                 //ָ����һ���ڵ�
    varNode()
    {
        var=0;
        next=0;
    }
};

//���ﶨ����һ���������������,��Ҫ��������ֲ�����
class PARSERS_EXPORT MyVarList 
{
public:
    //
    //��������:���캯��                
    //����˵��:
    //��������              ��������                    ����
    MyVarList();

    //��������
	~MyVarList();

    //��������:�����������һ���ڵ�
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //var           varNode *                   �����ڵ�
	void appendVar(varNode *var);
	

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
    MyVar *findVarByName(const XMLCh * const name);

    //�ж�һ�������Ƿ����
    bool VarNameExist(const XMLCh * const name) const;

    //��������:���ر��������ͷ�ڵ�
    //����ֵ::
    //��������                            ����
    //const varNode *                     �����б��ͷ�ڵ�
    const varNode *getListHead() const;

    // 
    //��������:���ر��������β�ڵ�
    //����ֵ::
    //��������                            ����
    //const varNode *                     �����б��β�ڵ�
	const varNode *getListEnd() const;

    //��������:���ݱ�����,�޸ı�����ֵ
    //
    //����ֵ:
    //��������                ����
    //bool                    true��ʾ�޸ĳɹ�,false��ʾ�޸�ʧ��
    //
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          ������
    //toSet         const XMLCh* const          ֵ���ַ���������ʽ
    //bool setVarValueByName(const XMLCh * const name,const XMLCh * const value,VARTYPE type);

    //��������:���ݱ�����,�޸ı�����ֵ
    //
    //����ֵ:
    //��������                ����
    //bool                    true��ʾ�޸ĳɹ�,false��ʾ�޸�ʧ��
    //
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          ������
    //toSet         const MyValue* const        ֵ��MyValue���͵�ָ��
	bool setVarValueByName(const XMLCh * const name,const _variant_t * const value);

    void reset();
private:
	//�����ʼ��
	void initialize();

    //����,��������������еı������ڴ�
	void cleanUp();

	varNode *           fVarHead;              //��������ͷ�ڵ�
	varNode *           fVarEnd;               //��������β�ڵ�


    /**
    * Copy constructor
    */
    MyVarList(const MyVarList& node);
    MyVarList& operator=(const MyVarList& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif