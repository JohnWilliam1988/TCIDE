//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-07-15  Ф�㷶      ���ʵ�γ�����MyDataNode�����
////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYDATALIST_H
#define MYDATALIST_H
#include"MyValue.h"
MATCH_CPP_NAMESPACE_BEGIN
//������MyDataNode ������ʾһ��ʵ�νڵ�,�ں������õ�ʱ��ֵ��ʱ����
class PARSERS_EXPORT MyDataNode 
{
public:
    //
    //��������:���캯��                
    //����˵��:
    //��������              ��������                    ����
    //type                  const MyValue::DataType     ��������
    //value                 const XMLCh * const         ����ֵ���ַ���������ʽ
    MyDataNode();

    //��������
    ~MyDataNode();


    //��������:���õ������Ǵ��������ʱ��,ֱ�Ӽ�¼����ָ��ͺ�,�������ô���
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //data          const MyValue* const        MyValueֵ��ָ��
    void setQuoteData(_variant_t* data);


    //��������:����ʵ��ֵ
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //toSet         const XMLCh* const          ֵ���ַ���������ʽ
    void  setValue(const XMLCh * const toSet,VARTYPE type);

    //
    // 
    //��������:�õ�ʵ�ε�ֵ��ָ��
    //����ֵ::
    //��������                            ����
    //const MyValue*                      ʵ�ε�ֵ��ָ��
    /*const*/ _variant_t* getData() /*const*/;

    //��������:������һ���ڵ�
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //node          MyDataNode * const          ��һ���ڵ��ָ��
    void setNextNode(MyDataNode * const node);

    // 
    //��������:�õ���һ���ڵ�
    //����ֵ::
    //��������                            ����
    //MyDataNode *                        ��һ���ڵ��ָ��
    MyDataNode *getNextNode() const;
private:
    //��ʼ��
    void initialize();
    //����
    void cleanUp();

	//�������ʹ���
	bool                      fQuote;                    //��ʾ�Ƿ������ñ���
	_variant_t *              fQuoteData;               //���ñ���
	//�������ʹ���

    _variant_t                fData;                     //ʵ��ָ��
    MyDataNode *              fNext;                     //�¸��ڵ�

    /**
    * Copy constructor
    */
    MyDataNode(const MyDataNode& node);
    MyDataNode& operator=(const MyDataNode& toAssign);
};

//�������Ҫ���ڴ���ʵ���б�Ĵ���,����ʱ������ʵ�α���
class PARSERS_EXPORT MyDataList
{
public:
    //��������:���캯��                
    //����˵��:
    //��������              ��������                    ����
    //memoryManager         MemoryManager* const        �ڴ�����Ա
    MyDataList(bool needDel=true);

    //��������
    ~MyDataList();

    //��������:��ʵ���б��м���һ��ʵ��
    //
    //����ֵ:
    //��������                            ����
    //void                                �޷�������
    //
    //����˵��:
    //��������      ��������              ����
    //var           MyDataNode *          ʵ��ָ��
    void appendNode(MyDataNode *var);

	//��������:��һ���������뵽�����ڶ�����������ȥ,�����������WIN32ʱ,��������λ��ʹ�õ�
    //
    //����ֵ:
    //��������                            ����
    //void                                �޷�������
    //
    //����˵��:
    //��������      ��������              ����
    //var           MyDataNode *          ʵ��ָ��
    void insertNodeByNode(MyDataNode *forwardNode,MyDataNode *var);


    //��������:�õ�ʵ���������ͷ
    //����ֵ::
    //��������                            ����
    //MyDataNode *                        ʵ��ͷ��ָ��
    MyDataNode *getListHead();

    //��������:�õ�ʵ���������β
    //����ֵ::
    //��������                            ����
    //MyDataNode *                        ʵ��ͷ��ָ��
    MyDataNode *getListEnd();

    //���ʵ������
    void clear();
private:
    //�����ʼ��
    void initialize();

    //�ڴ�����
    void cleanUp();

    MyDataNode *           fNodeHead;                 //ʵ������ͷ
    MyDataNode *           fNodeEnd;                  //ʵ������β
	bool                   fNeedDel;

    /**
    * Copy constructor
    */
    MyDataList(const MyDataList& node);
    MyDataList& operator=(const MyDataList& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif