//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-07-15  Ф�㷶      ��ɱ���������MyVar�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYVAR_H
#define MYVAR_H

#include <match/util/XMemory.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyValue.h"
#include <stdio.h>
#include <wchar.h>

//������com����Ľڵ�����
struct comNode
{
    void   *              pDis;                  //����ָ��
	comNode *             next;                 //ָ����һ���ڵ�
    comNode()
    {
        pDis=0;
        next=0;
    }
};

MATCH_CPP_NAMESPACE_BEGIN

class MyComInfor;
//���ǰ�MyVar�����һ������,�洢�������ͱ����洢��ֵ
class PARSERS_EXPORT MyVar
{
public:
    //
    //��������:���캯��                
    //����˵��:
    //��������              ��������                    ����
    MyVar(const XMLCh * const  name
        );

	 MyVar(const XMLCh * const  name
        ,_variant_t *  value
		,bool isQuote=false
        );

    ~MyVar();

    //��������:ȡ�ñ�������
    //����ֵ::
    //��������                            ����
    //VARTYPE            ���ر�������
    //VARTYPE  getDataType() const;

    //��������:�õ�������
    //����ֵ::
    //��������                            ����
    //const XMLCh*                        ���ر�����
    const XMLCh*   getName() const;

    //��������:�õ�������ֵ��ָ��
    //����ֵ::
    //��������                            ����
    //const MyValue*                      �õ�����ֵ��ָ��
     _variant_t*   getVarData();


	//��������:�����������һ���ڵ�
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //var           varNode *                   �����ڵ�
	void appendCom(comNode *com);


    //��������:��¼һ��dispatch�ӿ�,�����ͷ�
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
 //   void  initialComInfor();

	////��������:����һ��MyComInfor�ӿ�
	//MyComInfor *GetComInfo();

    //��������:���ñ���ֵ
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //toSet         const MyValue* const        ��һ������ֵ��ָ��
    void  setValue(const _variant_t * const toSet);

private:
    //��������:���ñ�����
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //toSet         const XMLCh* const          ������
    void  setName(const XMLCh * const toSet);

    //��ʼ��
    void initialize();

    //��������ڴ�
    void cleanUp();

    // -----------------------------------------------------------------------
    //  data members
    // -----------------------------------------------------------------------

	//�������ʹ���
	bool                      fQuote;                    //��ʾ�Ƿ������ñ���
	_variant_t *              fQuoteData;               //���ñ���
	//�������ʹ���

	//MyComInfor *              fDispinFor;
		
	//��¼�����COM���������
	comNode *           fComHead;
	comNode *           fComEnd;

    _variant_t                fValue;               //����ֵ
    XMLCh			          fName[NAMEMAXLENGTH]; //������

    /**
    * Copy constructor
    */
    MyVar(const MyVar& node);
    MyVar& operator=(const MyVar& toAssign);
};


MATCH_CPP_NAMESPACE_END

#endif



