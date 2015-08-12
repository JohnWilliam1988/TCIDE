#include "MyDebugValueList.h"

#include "../src/match/util/OutOfMemoryException.hpp"
#include <match/util/XMLString.hpp>

#include <match/util/XMLException.hpp>
#include <match/util/XMLExceptMsgs.hpp>
#include "MyValidException.h"
#include "MyValidator.h"
#include <wchar.h>
#include <stdio.h>

MATCH_CPP_NAMESPACE_BEGIN
//==============�����ڵ�=======================

//���캯��
//����˵��:
//��������            ��������                    ����
//namesp              const XMLCh* const          �����ռ�
//name                const XMLCh* const          ������
//value               const MyValue*              ����
//manager             MemoryManager* const        �ڴ����
MyDebugValue::MyDebugValue(const XMLCh* namesp,const XMLCh* name,const _variant_t* value)
{
    initialize();

    //���������ռ���
    setNamesp(namesp);

    //���ñ�����
    setVarName(name);

    //���ñ�������
	setVarType(MyValidator::getVarTypeString(value->vt));

    setVarValue(value);
}


    
MyDebugValue::MyDebugValue()
{
    initialize();
}


//��������
MyDebugValue::~MyDebugValue(void)
{
    cleanUp();
}

//��ʼ��
void MyDebugValue::initialize()
{
    ::memset(fNamesp,0,sizeof(XMLCh)*NAMEMAXLENGTH);
    ::memset(fVarName,0,sizeof(XMLCh)*NAMEMAXLENGTH);
    ::memset(fDatatype,0,sizeof(XMLCh)*TYPEMAXLENGTH);
    ::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
}

//���
void MyDebugValue::cleanUp()
{
    //--------�޸�,��Ϊ�ڴ��������XMLString ������,����û����XMLString ���ͷ�,�����в���ȫ����--�ӱ� 9-19
    ::memset(fNamesp,0,sizeof(XMLCh)*NAMEMAXLENGTH);
    ::memset(fVarName,0,sizeof(XMLCh)*NAMEMAXLENGTH);
    ::memset(fDatatype,0,sizeof(XMLCh)*TYPEMAXLENGTH);
    ::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
    //--------�޸�,��Ϊ�ڴ��������XMLString ������,����û����XMLString ���ͷ�,�����в���ȫ����--�ӱ� 9-19---
}


//���������ռ���
void MyDebugValue::setNamesp(const XMLCh * const namesp)
{
    ::memset(fNamesp,0,sizeof(XMLCh)*NAMEMAXLENGTH);

    if(!namesp || !(*namesp))
        return;

    ::memcpy(fNamesp,namesp,::wcslen(namesp)*sizeof(XMLCh));
}

//���ñ�����
void MyDebugValue::setVarName(const XMLCh * const name)
{
    ::memset(fVarName,0,sizeof(XMLCh)*NAMEMAXLENGTH);

    if(!name || !(*name))
        return;

    ::memcpy(fVarName,name,::wcslen(name)*sizeof(XMLCh));
}

// ���ñ�������
void MyDebugValue::setVarType(const XMLCh* type)
{
    ::memset(fDatatype,0,sizeof(XMLCh)*TYPEMAXLENGTH);

    if(!type || !(*type))
        return;
    ::memcpy(fDatatype,type,::wcslen(type)*sizeof(XMLCh));
}

//���ñ�����ֵ
void MyDebugValue::setVarValue(const _variant_t* const value)
{
    // ���ñ���ֵ
	if(value->vt==VT_BOOL)
	{
		if(value->vt)
		{
			::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
			::wcsncpy(fVarValue,L"true",VALUEMAXLENGTH);
		}
		else
		{
			::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
			::wcsncpy(fVarValue,L"false",VALUEMAXLENGTH);
		}
	}
	else if(value->vt==VT_DISPATCH)
	{
		::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
		::wsprintfW(fVarValue,L"%d",(int)value->pdispVal);
	}
	else
	{
		_variant_t dest;
		if(S_OK==VariantChangeType(&dest,value,VARIANT_NOVALUEPROP,VT_BSTR))
		{
			::memset(fVarValue,0,sizeof(XMLCh)*VALUEMAXLENGTH);
			::wcsncpy(fVarValue,dest.bstrVal,VALUEMAXLENGTH);
		}
	}
    return;
}


//��������:��ȡ�����ռ���
//����ֵ:  �����ռ���
//��������                  ����
//const XMLCh*             �����ռ���
//����˵��:
//��������      ��������                    ����
//��
const XMLCh*  MyDebugValue::getDebugNameSpace() const
{
    return fNamesp;
}

//��������:��ȡ������
//����ֵ:  ������
//��������                  ����
//const XMLCh*             ������
//����˵��:
//��������      ��������                    ����
//��
const XMLCh*  MyDebugValue::getDebugVarNames() const
{
    return fVarName;
}

//��������:��ȡ����������
//����ֵ:  ����������
//��������                  ����
//const XMLCh*          ����������
//����˵��:
//��������      ��������                    ����
//��
const XMLCh*  MyDebugValue::getValueType() const
{
    return fDatatype;
}

//��������:��ȡ������ֵ
//����ֵ:  ������ֵ
//��������                  ����
//const XMLCh*          ������ֵ
//����˵��:
//��������      ��������                    ����
//��
const XMLCh*  MyDebugValue::getDebugVarValueString() const
{
    return fVarValue;
}

//==================================================================
//  �ɶ������
//=================�����б�==================================

//���캯��
//����˵��:
//��������            ��������                    ����
//manager             MemoryManager* const        �ڴ����
MyDebugValueList::MyDebugValueList(/*MemoryManager* const  manager*/)
{
    initialize();
}

//��������
MyDebugValueList::~MyDebugValueList(void)
{
    cleanUp();
}

//��ʼ��
void MyDebugValueList::initialize()
{
    ::memset(fList,0,sizeof(MyDebugValue)*DEBUGVALUELISTMAXLENGTH);
    flength=0;
}

//���
void MyDebugValueList::cleanUp()
{
    ::memset(fList,0,sizeof(MyDebugValue)*DEBUGVALUELISTMAXLENGTH);
    flength=0;
}


//�����ڵ�����ʵ��
bool MyDebugValueList::inserNode(const XMLCh* namesp,const XMLCh* varname,const _variant_t* value)
{

    if( !namesp || !varname )
    {
        throw;
        return false;
    }

    if(flength>=DEBUGVALUELISTMAXLENGTH)
        return false;

	if(value->vt&VT_ARRAY)
	{
		return false;
	}

	//if(value->fData.f_datatype==MyValue::dt_TCArray)
	//{
	//	if(value->fData.fValue.f_tcarray)
	//	{
	//		MyValue::MyArrayValue * pArray=value->fData.fValue.f_tcarray;
	//		if(pArray->fCount==1)
	//		{
	//			int length=pArray->fLength;
	//			for(int i=0;i<length;i++)
	//			{				
	//				if(flength>=DEBUGVALUELISTMAXLENGTH)
	//					return false;

	//				//���Ե�ʱ��,��ӡ��������
	//				XMLCh helpName[32]={0};
	//				::swprintf(helpName,32,L"%s[%d]",varname,i);
	//				fList[flength].setNamesp(namesp);
	//				fList[flength].setVarName(helpName);
	//				fList[flength].setVarType(value->getDataTypeString(value->fData.fValue.f_tcarray->fType));
	//				fList[flength].setVarValue(&(value->fData.fValue.f_tcarray->fValue[i]));
	//				flength++;
	//			}
	//		}
	//		else if(pArray->fCount==2)
	//		{
	//			int c1=pArray->fArrayCount[0];
	//			int c2=pArray->fArrayCount[1];

	//			for(int i=0;i<c1;i++)
	//			{
	//				for(int j=0;j<c2;j++)
	//				{				
	//					if(flength>=DEBUGVALUELISTMAXLENGTH)
	//						return false;

	//					XMLCh helpName[32]={0};
	//					::swprintf(helpName,32,L"%s[%d][%d]",varname,i,j);
	//					fList[flength].setNamesp(namesp);
	//					fList[flength].setVarName(helpName);
	//					fList[flength].setVarType(value->getDataTypeString(value->fData.fValue.f_tcarray->fType));
	//					fList[flength].setVarValue(&(value->fData.fValue.f_tcarray->fValue[i*c2+j]));
	//					flength++;
	//				}
	//			}

	//		}
	//		else if(pArray->fCount==3)
	//		{
	//			int c1=pArray->fArrayCount[0];
	//			int c2=pArray->fArrayCount[1];
	//			int c3=pArray->fArrayCount[2];
	//			for(int i=0;i<c1;i++)
	//			{
	//				for(int j=0;j<c2;j++)
	//				{				
	//					for(int k=0;k<c3;k++)
	//					{
	//						if(flength>=DEBUGVALUELISTMAXLENGTH)
	//							return false;

	//						XMLCh helpName[32]={0};
	//						::swprintf(helpName,32,L"%s[%d][%d][%d]",varname,i,j,k);
	//						fList[flength].setNamesp(namesp);
	//						fList[flength].setVarName(helpName);
	//						fList[flength].setVarType(value->getDataTypeString(value->fData.fValue.f_tcarray->fType));
	//						fList[flength].setVarValue(&(value->fData.fValue.f_tcarray->fValue[i*c2*c3+j*c3+k]));
	//						flength++;
	//					}
	//				}
	//			}

	//		}
	//	}
	//	
	//	return true;
	//}

    fList[flength].setNamesp(namesp);
    fList[flength].setVarName(varname);
	fList[flength].setVarType(MyValidator::getVarTypeString(value->vt));
    fList[flength].setVarValue(value);
    //���ñ�������
    flength++;
    return true;
}

// �жϱ�����Ϣ�����Ƿ�Ϊ��
bool MyDebugValueList::isEmpty()
{
    return (flength==0);
}

unsigned int MyDebugValueList::getLength() const
{
    return flength;
}

//��������:��ȡ�����ռ���
//����ֵ:  �����ռ���
//��������                  ����
//const XMLCh*             �����ռ���
//����˵��:
//��������      ��������                    ����
//��
const XMLCh*  MyDebugValueList::getDebugNameSpaceByIndex(unsigned int index) const
{
    return fList[index].getDebugNameSpace();

}
//��������:��ȡ������
//����ֵ:  ������
//��������                  ����
//const XMLCh*             ������
//����˵��:
//��������      ��������                    ����
//��
const XMLCh*  MyDebugValueList::getDebugVarNamesByIndex(unsigned int index) const
{
    return fList[index].getDebugVarNames();
}
//��������:��ȡ����������
//����ֵ:  ����������
//��������                  ����
//const XMLCh*           ����������
//����˵��:
//��������      ��������                    ����
//��
const XMLCh* MyDebugValueList::getValueTypeByIndex(unsigned int index) const
{
    return fList[index].getValueType();
}

//��������:��ȡ������ֵ
//����ֵ:  ������ֵ
//��������                  ����
//const XMLCh*          ������ֵ
//����˵��:
//��������      ��������                    ����
//��
const XMLCh*  MyDebugValueList::getDebugVarValueStringByIndex(unsigned int index) const
{
    return fList[index].getDebugVarValueString();
}
//��������:���ӱ����ڵ�
//����ֵ:  void
//��������                  ����
//void
//����˵��:
//��������      ��������                    ����
//namesp        const XMLCh*               �����ռ���
//VarList       MyVarList*                 �ֲ������б�
void MyDebugValueList::catDebugValueToList(const XMLCh* namesp,MyVarList* VarList)
{
    const varNode *helpvarNode =VarList->getListHead();

    while(helpvarNode)
    {
        inserNode(namesp,helpvarNode->var->getName(),helpvarNode->var->getVarData());

        helpvarNode = helpvarNode->next;
    }
}

//��������:���ӱ����ڵ�
//����ֵ:  void
//��������                  ����
//void
//����˵��:
//��������      ��������                    ����
//namesp        const XMLCh*               �����ռ���
//VarList       MyVarListContainer*        �ֲ������б�
void MyDebugValueList::catDebugValueToList(const XMLCh* namesp,MyVarListContainer* VarList)
{
    varNode *helpvarNode =VarList->getListHead();

    while(helpvarNode)
    {
        inserNode(namesp,helpvarNode->var->getName(),helpvarNode->var->getVarData());

        helpvarNode = helpvarNode->next;
    }
}


MATCH_CPP_NAMESPACE_END
