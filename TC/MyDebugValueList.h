#if !defined(MYDEBUGVALUELIST_H)
#define MYDEBUGVALUELIST_H

#include "MyVarListContainer.h"
#include "MyValue.h"

MATCH_CPP_NAMESPACE_BEGIN

// =============���Ա����ڵ�=================
class PARSERS_EXPORT MyDebugValue
{
public:
	//���캯��
	//����˵��:
	//��������            ��������                    ����
	//namesp              const XMLCh* const          �����ռ�
	//name                const XMLCh* const          ������
	//value               const MyValue*              ����
	//manager             MemoryManager* const        �ڴ����
	MyDebugValue(const XMLCh* namesp,const XMLCh* name,const _variant_t* value/*,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager*/);

    MyDebugValue();

	//��������
	~MyDebugValue(void);

	//��������:��ȡ�����ռ���
	//����ֵ:  �����ռ���
	//��������                  ����
	//const XMLCh*             �����ռ���
	//����˵��:
	//��������      ��������                    ����
	//��
	const XMLCh*  getDebugNameSpace() const;

	//��������:��ȡ������
	//����ֵ:  ������
	//��������                  ����
	//const XMLCh*             ������
	//����˵��:
	//��������      ��������                    ����
	//��
	const XMLCh*  getDebugVarNames() const;	

	//��������:��ȡ����������
	//����ֵ:  ����������
	//��������                  ����
	//const XMLCh*           ����������
	//����˵��:
	//��������      ��������                    ����
	//��
	const XMLCh* getValueType() const;

	//��������:��ȡ������ֵ
	//����ֵ:  ������ֵ
	//��������                  ����
	//const XMLCh*          ������ֵ
	//����˵��:
	//��������      ��������                    ����
	//��
	const XMLCh*  getDebugVarValueString() const;

	
	//��������:�����¸������ڵ�
	//����ֵ:  void
	//��������                  ����
	//void
	//����˵��:
	//��������      ��������                    ����
	//node         MyDebugValue* const         �����ڵ�
	void  setNextNode(MyDebugValue* const node);

    //���������ռ���
	void setNamesp(const XMLCh * const namesp);

	//���ñ�����
	void setVarName(const XMLCh * const name);

	//���ñ�������
	void setVarType(const XMLCh*  type);

	//���ñ�����ֵ
	void setVarValue(const _variant_t* const value);
private:

	//��ʼ��
	void initialize();
	//���
	void cleanUp();

	XMLCh               fNamesp[NAMEMAXLENGTH];
	XMLCh               fVarName[NAMEMAXLENGTH];
	XMLCh               fDatatype[TYPEMAXLENGTH];
	XMLCh               fVarValue[VALUEMAXLENGTH];
};

//==============���Ա�������=======================

class PARSERS_EXPORT MyDebugValueList
{
public:

	//���캯��
	//����˵��:
	//��������            ��������                    ����
	//manager             MemoryManager* const        �ڴ����
	MyDebugValueList(/*MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager*/);

	//��������
	~MyDebugValueList(void);

	//��������:���ӱ����ڵ�
	//����ֵ:  void
	//��������                  ����
	//void
	//����˵��:
	//��������      ��������                    ����
	//namesp        const XMLCh*               �����ռ���
	//VarList       MyVarList*                 �ֲ������б�
	void catDebugValueToList(const XMLCh* namesp,MyVarList* VarList);

	//��������:���ӱ����ڵ�
	//����ֵ:  void
	//��������                  ����
	//void
	//����˵��:
	//��������      ��������                    ����
	//namesp        const XMLCh*               �����ռ���
	//VarList       MyVarListContainer*        �ֲ������б�
	void catDebugValueToList(const XMLCh* namesp,MyVarListContainer* VarList);

    unsigned int getLength() const;

	//��ձ�����Ϣ����
	void cleanUp();

	// �жϱ�����Ϣ�����Ƿ�Ϊ��
	bool isEmpty();

    	//��������:��ȡ�����ռ���
	//����ֵ:  �����ռ���
	//��������                  ����
	//const XMLCh*             �����ռ���
	//����˵��:
	//��������      ��������                    ����
	//��
	const XMLCh*  getDebugNameSpaceByIndex(unsigned int) const;

	//��������:��ȡ������
	//����ֵ:  ������
	//��������                  ����
	//const XMLCh*             ������
	//����˵��:
	//��������      ��������                    ����
	//��
	const XMLCh*  getDebugVarNamesByIndex(unsigned int) const;	

	//��������:��ȡ����������
	//����ֵ:  ����������
	//��������                  ����
	//const XMLCh*           ����������
	//����˵��:
	//��������      ��������                    ����
	//��
	const XMLCh* getValueTypeByIndex(unsigned int) const;

	//��������:��ȡ������ֵ
	//����ֵ:  ������ֵ
	//��������                  ����
	//const XMLCh*          ������ֵ
	//����˵��:
	//��������      ��������                    ����
	//��
	const XMLCh*  getDebugVarValueStringByIndex(unsigned int) const;

private:

	//��ʼ��
	void initialize();
	

	//�����ڵ�����ʵ��
	bool inserNode(const XMLCh* namesp,const XMLCh* varname,const _variant_t* value);
	
	MyDebugValue         fList[DEBUGVALUELISTMAXLENGTH];
	unsigned int         flength;
};

MATCH_CPP_NAMESPACE_END

#endif