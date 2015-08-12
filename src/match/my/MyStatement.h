//////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ������ĳ������MyStatement�����
//////////////////////////////////////////////////////////////////////////

#ifndef MYSTATEMENT_H
#define MYSTATEMENT_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>

#include <match/util/XMLException.hpp>
#include <match/util/XMLExceptMsgs.hpp>
#include "MyValidException.h"

MATCH_CPP_NAMESPACE_BEGIN

//�����ǻ��еĳ�����,����һ���ÿ����䶼�����һ�� mystatement �ļ̳���
class PARSERS_EXPORT MyStatement : public XMemory
{
public:
	//��ʼ������
    //�������ö������
	enum StatementType
	{
		STATEMENT_INCLUDE =0,          //include�Z��
	    STATEMENT_IMPORT =0,           //import�Z��
		STATEMENT_NAMESPACE ,          //namespace�Z��
		STATEMENT_ENDNAMESPACE ,       //namesace�����Z��
		STATEMENT_IF ,                 //if�Z��
		STATEMENT_ELSE,                //else���
		STATEMENT_ENDIF,               //if�������
		STATEMENT_SELECT,              //select���
		STATEMENT_ENDSELECT,           //endselect���
		STATEMENT_CASE,                //case���
		STATEMENT_DEFAULT,             //case���
		STATEMENT_WHILE,               //while ���
		STATEMENT_BREAK,               //ѭ��break���
		STATEMENT_CONTINUE,            //ѭ��continue���
		STATEMENT_ENDWHILE,            //while �������
		STATEMENT_DO,                  //do ���
		STATEMENT_ENDDOWHILE,          //enddowhile �������
		STATEMENT_REPEAT,              //repeat�ظ��������
		STATEMENT_ENDREPEAT,           //repeat�ظ������������
		STATEMENT_FUNCTION,            //�������忪ͷ���
		STATEMENT_ENDFUNCTION,         //������β���
		STATEMENT_VARIABLE,            //�����������
		STATEMENT_ASSIGNMENT,          //��ֵ���
		STATEMENT_CALL,                //�����������
		STATEMENT_RETURN,              //�����������
		STATEMENT_UNKNOWN,             //δ֪
	};

    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //type          const StatementType         �������
    //id            const unsigned int          ���ID
    //manager       MemoryManager* const        �ڴ������
    MyStatement(const StatementType  type
        ,const unsigned int    id = 0
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


    //��������:�����������
    //����ֵ::
    //��������                ����
    //const StatementType     ������������
    const StatementType getStatementType() const;                                  

    //��������:�������ID
    //����ֵ::
    //��������                ����
    //const unsigned int      ��������ID
    const unsigned int getStatementID() const;

    //��������
    virtual ~MyStatement();

protected:
    //��ʼ��
    void initialize();                 

    //��������ڴ�
    void cleanUp();

    MemoryManager*      fMemoryManager;      //�ڴ������
    StatementType       fType;               //�ڵ�����
    unsigned int        fId;                 //���ڽڵ�����������,Ԥ��
    /**
    * Copy constructor
    */
    MyStatement(const MyStatement& node);
    MyStatement& operator=(const MyStatement& toAssign);
};

MATCH_CPP_NAMESPACE_END

#endif