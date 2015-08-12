//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-05-15  Ф�㷶      ���ִ�п�ĳ������BaseBlock�����
//0002      2010-7-16   �ӱ�        ����˳�Ա����fLineNum,fSourceFilePath�Լ���Ա����getLineNum(),getBlockFilePath()
////////////////////////////////////////////////////////////////////////////////////////

#ifndef BASEBLOCK_H
#define BASEBLOCK_H

#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>

#include <match/util/XMLException.hpp>
#include <match/util/XMLExceptMsgs.hpp>
#include "MyValidException.h"
#include <Windows.h>

MATCH_CPP_NAMESPACE_BEGIN

//���ǻ�������ִ�еĳ������,BaseBlock��������ִ�п�ĳ�����
//��������������:һ��while��� �� һ�� endwhile ��������һ��while��Ŀ��,
//����԰����ӿ�,������䲻�ܰ������,�����ǿ鶼��Ҫ�����ӿ�

class PARSERS_EXPORT BaseBlock : public XMemory
{
public:

    //�����͵�ö������
    enum BlockType
    {
        BLOCK_INCLUDE =0,            //include��
		BLOCK_IMPORT ,               //import��
        BLOCK_NAMESPACE,             //namespace��
        BLOCK_ENDNAMESPACE,          //endnamespace��
        BLOCK_VARIABLE,              //���������
        BLOCK_FUNCTION,              //������
        BLOCK_ENDFUNCTION,           //end������
        BLOCK_IF ,                   //if��
        BLOCK_ELSE ,                 //else��
        BLOCK_ENDIF ,                //endif��
		BLOCK_SELECT ,               //select��
		BLOCK_ENDSELECT ,            //endselect��
		BLOCK_CASE ,                 //case��
		BLOCK_DEFAULT,               //default��
        BLOCK_WHILE,                 //while��
        BLOCK_ENDWHILE,              //endwhile��
        BLOCK_BREAK,                 //ѭ���������
        BLOCK_CONTINUE,              //ѭ���������һ��
        BLOCK_REPEAT,                //repeat��
        BLOCK_ENDREPEAT,             //endrepeat��
        BLOCK_ASSIGNMENT,            //��ֵ��
        BLOCK_CALL,                  //�������ÿ�
		BLOCK_LABLE,                 //������
		BLOCK_GOTO,                  //GOTO���
        BLOCK_RETURN,                //�������ؿ�
		BLOCK_END,                   //�����Ľ�����
        BLOCK_UNKNOWN,               //δ֪
    };

    //���캯��
    //����˵��:
    //��������      ��������                    ����
    //type          const BlockType             ������
    //id            const unsigned int          ���ID
    //linenum       const unsigned int          ����
    //filepath      const XMLCh*   const        �ļ�·��
    //manager       MemoryManager* const        �ڴ������
    BaseBlock(const BlockType  type
        ,const unsigned int    id=0
        ,const unsigned int    linenum=0
        ,const XMLCh* const    filepath=NULL
        ,MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //��������:���ؿ������
    //����ֵ::
    //��������                ����
    //const BlockType         ���ؿ������
    const BlockType getBlockType() const;    

    //��������
    virtual ~BaseBlock();

    //��������:���ؿ����һ����
    //����ֵ::
    //��������                 ����
    //BlockType*         ��һ�����ָ��
    BaseBlock* getNextBlock() const;

    //��������:���ؿ����һ����
    //����ֵ::
    //��������                 ����
    //BlockType*         ��һ�����ָ��
    BaseBlock* getPreBlock() const;

    //��������:�õ��˿��id
    //����ֵ::
    //��������                 ����
    //unsigned int             IDֵ
    unsigned int getID() const;

    //���ÿ��id
    //��������:�õ��˿��id
    //����ֵ::
    //��������                 ����
    //void                     ��
    //
    //����˵��:
    //��������      ��������                    ����
    //id            unsigned int                IDֵ
    void setID(unsigned int id);

    //���ÿ����һ����
    //��������:�õ��˿��id
    //����ֵ::
    //��������                 ����
    //void                      ��
    //
    //����˵��:
    //��������      ��������                    ����
    //node          BaseBlock*                  ��ָ��
    void setNextBlock(BaseBlock* node);

    //
    //��������:���ÿ����һ�����id
    //����ֵ::
    //��������                 ����
    //��                       ��
    //
    //����˵��:
    //��������      ��������                    ����
    //node          BaseBlock*                  ��ָ��
    void setPreBlock(BaseBlock*  node);

    //��������:�õ����к�
    //����ֵ:: �кţ���������
    //��������                 ����
    //XMLFileLoc               �к�
    //
    //����˵��:��

    XMLFileLoc getBlockLineNum() const;

    //��������:�õ���������ļ�·��
    //����ֵ:: �����ڵ��ļ�·��
    //��������                 ����
    //XMLCh                    �ļ�·��
    //
    //����˵��:��   
    const XMLCh* getBlockFilePath() const;

protected:
    //�����ڴ�
    void cleanUp();

    //��ĳ�ʼ��
    void initialize();

    //��������:�����ļ�·��
    //
    //����ֵ:
    //��������                ����
    //void                    �޷�������
    //
    //����˵��:
    //��������      ��������                    ����
    //name          const XMLCh* const          �ļ�·��
    void setFilePath(const XMLCh* const name);

    /**
    * Copy constructor
    */
    BaseBlock(const BaseBlock& node);
    BaseBlock& operator=(const BaseBlock& toAssign);

    MemoryManager*    fMemoryManager;           //�ڴ����
    BlockType         fType;                    //�ڵ�����
    BaseBlock*        fNext;                    //��һ�����ָ��
    BaseBlock*        fPre;                     //��һ�����ָ��
    unsigned int      fId;                      //���ڿ�ı��,��һ���������һ��Ψһ��ʶ
    XMLFileLoc        fLineNum;                 //��ʶ�����ڵ�����
	XMLCh  	          fSourceFilePath[MAX_PATH];                  //��¼�ÿ����ڵ��ļ�·��
};

MATCH_CPP_NAMESPACE_END


#endif