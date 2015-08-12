#if !defined(MYDEBUGINFOLIST_H)
#define MYDEBUGINFOLIST_H

#include <match/util/MatchDefs.hpp>
//#include <match/util/PlatformUtils.hpp>
#include <windows.h>

MATCH_CPP_NAMESPACE_BEGIN

//===============================================MyBreakPointInfo Node =====================================

//�����Ƕϵ���Ϣ�ļ�¼,��¼��ǰ�Ķϵ���Ϣ
class PARSERS_EXPORT MyBreakPointInfo
{
public:
    MyBreakPointInfo();
	//���캯��
	//����˵��:
	//��������      ��������                    ����
	//filepath      XMLCh* const                �ļ�����·��
	//linenum       int                         �к�
	MyBreakPointInfo(const XMLCh* const filepath
                    ,int linenum
                    );

	//��������
	~MyBreakPointInfo(void);

	//��������:��ȡ�ļ���
	//����ֵ:  �ļ���
	//��������                          ����
	//XMLCh*                            �ļ���
	//����˵��:
	//��������      ��������            ����
	//��
	const XMLCh*  getDebugFileName() const;

	//��������:��ȡ�к�
	//����ֵ:   ����
	//��������                          ����
	//int                               �к�
	//����˵��:
	//��������      ��������            ����
	//��
	int getDebugLineNum() const;

    //�����ļ���·��
    void setFielName(const XMLCh * const name);

    //�����к�
    void setLineNum(XMLUInt32 lineNum);

private:
	//��ʼ��
	void initialize();
	//���
	void cleanUp();


	XMLCh             fFilename[MAX_PATH];      //�ļ�·��
	XMLUInt32         fLineNum;                 //�к�

};

//===============================================MyBPList ========================
//class PARSERS_EXPORT MyBPList
//{
//public:
//	//��������:���캯��
//	//����˵��:
//	//��������      ��������              ����
//	//manager       MemoryManager* const  �ڴ����
//	MyBPList(void);
//
//	//��������
//	~MyBPList(void);
//
//	//��������:��������м���һ��������Ϣ�ڵ�
//	//����ֵ: void
//	//��������                                  ����
//	//��
//	//����˵��:
//	//��������      ��������                    ����
//	//node          MyDebugInfo*                ������Ϣ�ڵ�
//	bool inserNode(const XMLCh* const filepath,int linenum);
//
//
//    //��������:����ָ��λ�û�ȡ�ļ���
//    //����ֵ:  �ļ���
//    //��������                          ����
//    //XMLCh*                            �ļ���
//    //����˵��:
//    //��������      ��������            ����
//    //��
//    const XMLCh*  getFileNameByIndex(unsigned int index) const;
//
//
//     //��������:����ָ��λ�û�ȡ�к�
//	//����ֵ:   ����
//	//��������                          ����
//	//int                               �к�
//	//����˵��:
//	//��������      ��������            ����
//	//��
//	int getLineNumByIndex(unsigned int index) const;
//
//
//	//��������:��ȡ�б�ͷ
//	//����ֵ: ������Ϣ�ڵ�
//	//��������                                  ����
//	//MyBreakPointInfo*                         �ϵ�ָ��
//	//����˵��:
//	//��������      ��������                    ����
//	//��
//	unsigned int getLength() const;
//
//	//��������: �ж�һ���ڵ��Ƿ����,
//	bool isExist(const XMLCh* const filepath,int linenum);
//
//	//ȥ���ڵ�
//	bool removeNode(const XMLCh* const filepath,int linenum);
//
//	// ɾ�����еĽڵ�
//	void removeAllNode();
//
//	// �ж��б��Ƿ�Ϊ��
//	bool isEmpty();
//
//private:
//	//�����ʼ��
//	void initialize();
//	//���
//	void cleanUp();
//
//	MyBreakPointInfo          fList[MATCH_MAXBREAKBPNUM];
//    unsigned int              fLeng;
//};

MATCH_CPP_NAMESPACE_END

#endif