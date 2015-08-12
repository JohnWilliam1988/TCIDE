/////////////////////////////////2009.12.16  Ф�㷶�޶�//////////////////////////
#if !defined(MYVALIDATOR_H)
#define MYVALIDATOR_H

#include <match/util/MatchDefs.hpp>
#include <match/framework/MemoryManager.hpp>
#include <match/util/PlatformUtils.hpp>
#include "MyAssignmentStatement.h"
#include "MyValue.h"
#include "BaseBlock.h"

MATCH_CPP_NAMESPACE_BEGIN

//������ü�������� �����������ĸ�ʽ,�Ѿ��������ǵĺϷ���(ָ������Ķ����ǲ����Ѿ�����)
//�Լ�������������Եĸ�ʽ,�ͼ������ǵĺϷ���
class PARSERS_EXPORT MyValidator
{
public :

	//д��־
	static bool writelog(const XMLCh* Name);


	//��ʽ�ļ���,һ��������䶨����
	//������������֤���������ռ��������ߺ������Ƿ�Ϊ�ؼ���
	//����ֵ:  true ���� false
    //��������                ����
    //bool                    ����ֵ��true��ʾΪ�ؼ��֣�false��ʾ�ǹؼ���
    //����˵��:
    //��������                ��������                    ����
    //Name                    const XMLCh*               ҪЧ����ַ�
	static bool validateKeyword(const XMLCh* Name);

    //��֤��������������
	//����������������������-��֤��������
	//����ֵ:   
    //��������                ����
    //����˵��:
    //��������                ��������                    ����
    //dtString            const XMLCh*  const         ҪЧ�������
	//manager             MemoryManager* const        �ڴ�����Ա
    static VARTYPE  validateConstant(const XMLCh* const dtString);

	//��֤�Ƿ���COM���Ͷ���
	//������������֤�Ƿ���COM���Ͷ���
	//����ֵ:   
    //��������                ����
    //����˵��:
    //��������                ��������                    ����
    //dtString            const XMLCh*  const         ҪЧ�������
	//manager             MemoryManager* const        �ڴ�����Ա
    static bool validateComDef(const XMLCh* const dtString,XMLCh * comID);

	//��֤�Ƿ���array���Ͷ���
	//������������֤�Ƿ���array���Ͷ���
	//����ֵ:   
    //��������                ����
    //����˵��:
    //��������                ��������                    ����
    //dtString            const XMLCh*  const         ҪЧ�������
	//manager             MemoryManager* const        �ڴ�����Ա
    static bool validateArrayDef(const XMLCh* const dtString/*,int * arraySub*/);


	//��֤�Ƿ����������Ͷ���,������ñ������������±�
	//������������֤�Ƿ���COM���Ͷ���
	//����ֵ:   
    //��������                ����
    //����˵��:
    //��������                ��������                    ����
    //dtString            const XMLCh*  const         ҪЧ�������
	//manager             MemoryManager* const        �ڴ�����Ա
    static bool validateArrayDefByExp(const BaseBlock* const parent,const XMLCh* const dtString);


	//�������������麯������ʵ�εĺϷ���
	//����ֵ:   true�� false
    //��������                ����
    //bool                true��ʾ������ʵ�κϷ���false��ʾ���Ϸ�
    //����˵��:
    //��������                ��������                    ����
	//parent              const BaseBlock* const      ������丸��
    //argumentString      const XMLCh*  const         ������ʵ��
	//funcParmList        const XMLCh*  const         �������β�
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validateCallArgumentLegality(const BaseBlock* const parent
		,const XMLCh* const argumentString
		,const XMLCh* const funcParmList
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//��������������COM���ò����ĺϷ���,ֻ�ܼ���ʽ�Ƿ���Ч
	//����ֵ:   true�� false
    //��������                ����
    //bool                true��ʾ������ʵ�κϷ���false��ʾ���Ϸ�
    //����˵��:
    //��������                ��������                    ����
	//parent              const BaseBlock* const      ������丸��
    //argumentString      const XMLCh*  const         ������ʵ��
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validateComArgumentLegality(const BaseBlock* const parent
		,const XMLCh* const argumentString
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//������������֤�������ĸ�ʽ,������,�ռ������ĸ�ʽ�Ƿ���ȷ
	//����ֵ:  true ���� false
    //��������              ����
    //bool                 ����ֵ��true��ʾΪ��ʽ��ȷ��false��ʾ����ȷ
    //����˵��:
    //��������              ��������                    ����
    //Name                 const XMLCh*  const        ҪЧ��ĺ�����,�ռ������ַ�
	//manager              MemoryManager* const        �ڴ�����Ա
	static bool validateName(const XMLCh* const Name,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	//������������֤�������ĺϷ���,����������Ƿ���� tc4.0
	//����ֵ:   true��false
    //��������             ����
    //bool                true��ʾ�������Ѿ����ڣ�false��ʾ������
    //����˵��:
    //��������             ��������                    ����
	//parent              const BaseBlock* const      �������ĸ���
    //varName             const XMLCh*  const         ҪЧ��ı�����
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validateVarNameExist(const BaseBlock* const parent,const XMLCh* const varName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//�������������麯�������ĺϷ���,��������ı������Ƿ�Ϸ� tc4.0
	//����ֵ:   true��false
    //��������                ����
    //bool                true��ʾ���������Ϸ���false��ʾ���Ϸ�
    //����˵��:
    //��������                ��������                    ����
	//parent              const BaseBlock* const      �����ĸ���
    //pramString          const XMLCh*  const         ҪЧ��ĺ�������
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validateFuncPramLegality(const BaseBlock* const parent,const XMLCh* const pramString
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

    //������������֤�������ĸ�ʽ,������,�ռ������ĸ�ʽ�Ƿ���ȷ,����������׳��쳣
    //����ֵ:  true ���� false
    //��������                ����
    //bool                    ����ֵ��true��ʾΪ��ʽ��ȷ��false��ʾ����ȷ
    //����˵��:
    //��������                ��������                    ����
    //Name                    const XMLCh*  const        ҪЧ��ĺ�����,�ռ������ַ�
    //manager                 MemoryManager* const        �ڴ�����Ա
    static bool validateNameNoException(const XMLCh* const Name);

	//�Ϸ��Եļ���,һ��������Ķ�����

	//������������֤�Ƿ��ʾһ����������,������repeat�����������ʽʹ��
	//����ֵ:  true ���� false
    //��������             ����
    //bool                ����ֵ��true��ʾΪ����������false��ʾ���ǳ�������
    //����˵��:
    //��������             ��������                    ����
    //IntegerString       const XMLCh*  const         ҪЧ����ַ�
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validateIsUnsigendInteger(const XMLCh* const IntegerString,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	//������������֤ȫ�ֱ����ĺϷ���,���������ռ�+�������Ƿ����
	//����ֵ:   true��false
    //��������             ����
    //bool                true��ʾ�������Ѿ����ڣ�false��ʾ������
    //����˵��:
    //��������             ��������                    ����
	//parent              const BaseBlock* const      �������ĸ���
    //varName             const XMLCh*  const         ҪЧ��ı�����
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validateGlobleVarNameExist(const XMLCh* const ns,const XMLCh* const varName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	static const BaseBlock* validateGlobleVarBlock(const XMLCh* const ns,const XMLCh* const varName,MemoryManager* const  manager) ;

	//�������������麯�����ĺϷ���,���麯�����Ƿ����
	//����ֵ:   true��false
    //��������                ����
    //bool                true��ʾ�������Ѿ����ڣ�false��ʾ������
    //����˵��:
    //��������                ��������                    ����
	//parent              const BaseBlock* const      �������ĸ���
    //funcName            const XMLCh*  const         ҪЧ��ĺ�����
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validateFuncNameExist(const BaseBlock* const parent,const XMLCh* const funcName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//�����������������ԵĺϷ���,�������ı��������ߺ������Ƿ�Ϸ�
	//����ֵ:   MyValue::DataType
    //��������                ����
    //MyValue::DataType   MyValue::DataType�ľ���ֵ��ʧ�ܷ��ص���MyValue::dt_MAXCOUNT
    //����˵��:
    //��������                ��������                    ����
	//parent              const BaseBlock* const      ���ʽ�ĸ���
    //funcName            const XMLCh*  const         ҪЧ��ı��ʽ
	//manager             MemoryManager* const        �ڴ�����Ա
	static VARTYPE validateExpressionLegality(const BaseBlock* const parent,const XMLCh* const expName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//��������������һ�����������б����Ƿ����һ��������
	//����ֵ:   true��false
    //��������                ����
    //bool                true��ʾ�Ѿ�������false��ʾû�а���
    //����˵��:
    //��������                ��������                    ����
	//pramString          const XMLCh* const          ���������ַ���
    //varName             const XMLCh*  const         ҪЧ��ı�����
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validatePramListContainsName(const XMLCh* const pramString,const XMLCh* const varName
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager) ;

	//�������������麯�����õĺϷ���
	//����ֵ:   MyValue::DataType
    //��������                ����
    //MyValue::DataType   MyValue::DataType�ľ���ֵ��MyValue::dt_MAXCOUNT��ʾ����������䲻�Ϸ�
    //����˵��:
    //��������                ��������                    ����
	//parent              const BaseBlock* const      ����������丸��
    //callSataement       const XMLCh*  const         ���õĺ������
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool validateCallLegality(const BaseBlock* const parent
		,const XMLCh* const callSataement
		,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	//������������֤ Break����Continue �ĺϷ���
	//����ֵ:   true��false
    //��������                ����
    //bool                true��ʾBreak����Continue�Ϸ��ԣ�false��ʾ���Ϸ�
    //����˵��:
    //��������                ��������                    ����
	//parent              const BaseBlock* const      ����������丸��
	//manager             MemoryManager* const        �ڴ�����Ա
	static bool  validateBreakOrContinue(const BaseBlock* const parent,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

	/////////////////////////////////////////////�������/////////////////////////////////////////
	//������������֤�����Ա���õĸ�ʽ�Ƿ���ȷ
	//����ֵ:  true ���� false
    //��������              ����
    //bool                 ����ֵ��true��ʾΪ��ʽ��ȷ��false��ʾ����ȷ
    //����˵��:
    //��������              ��������                    ����
    //Name                 const XMLCh*  const        ҪЧ��������Ա����
	//manager              MemoryManager* const        �ڴ�����Ա
	static bool validateArraySubCall(const BaseBlock* const parent,const XMLCh* const buff,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //������������֤format(��ʽ��)�ַ������ݵĲ����Ϸ���
    //����ֵ:   true��false
    //��������                ����
    //bool                true��ʾ���Ը�ֵ��false��ʾ�����Ը�ֵ
    //����˵��:
    //��������                ��������                    ����
    //express             const XMLCh* const          ��ʽ�������Ĳ���
    //manager             MemoryManager* const        �ڴ�����Ա
    //������Է�����
    static bool validateFormatArgument(const BaseBlock* const parent,const XMLCh* const argumentStr,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);

    //�õ����͵��ַ���
	//�����������õ����͵��ַ���
	//����ֵ:   
    //��������                ����
    //����˵��:
    //��������                ��������                    ����
    static const XMLCh * getVarTypeString(VARTYPE vt);

	//������������֤goto�ı�ǺϷ���
    //����ֵ:   true��false
    //��������            ����
    //bool                true��ʾ���Ը�ֵ��false��ʾ�����Ը�ֵ
    //����˵��:
    //��������            ��������                    ����
    //lable               const XMLCh* const          ��ʽ�������Ĳ���
    //manager             MemoryManager* const        �ڴ�����Ա
    //������Է�����
    static bool validateGoto(const BaseBlock* const parent,const XMLCh* const lable,MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);


private:
	MyValidator();
};

MATCH_CPP_NAMESPACE_END

#endif
