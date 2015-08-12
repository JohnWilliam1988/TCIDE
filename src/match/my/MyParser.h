//////////////////////////////////////////////////////////////////////////////////////////
//�ļ������޸ļ�¼
//����      ����        ��¼��      �����Լ�Ŀ�� 
//0001      2010-06-26  Ф�㷶      ��ɿ�ű�������MyParser�����
//////////////////////////////////////////////////////////////////////////////////////////

#if !defined(MYPARSER_H)
#define MYPARSER_H

#include <match/framework/XMLErrorReporter.hpp>
#include <match/sax/InputSource.hpp>

#include "MyBlockListPool.h"
#include "MyScanner.h"
#include "MyGlobalVarList.h"

MATCH_CPP_NAMESPACE_BEGIN

//MyParser����Ҫ�������matchԴ�ļ�������
class PARSERS_EXPORT MyParser  : public XMemory
                               , public XMLErrorReporter
{ 
public:
	MyParser(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


    // -----------------------------------------------------------------------
    //  Parsing methods
    // -----------------------------------------------------------------------

	    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface.
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLErrorReporter interface. */
    //@{

    /** Handle errors reported from the parser
      *
      * This method is used to report back errors found while parsing the
      * XML file. This method is also borrowed from the SAX specification.
      * It calls the corresponding user installed Error Handler method:
      * 'fatal', 'error', 'warning' depending on the severity of the error.
      * This classification is defined by the XML specification.
      *
      * @param errCode An integer code for the error.
      * @param msgDomain A const pointer to an Unicode string representing
      *                  the message domain to use.
      * @param errType An enumeration classifying the severity of the error.
      * @param errorText A const pointer to an Unicode string representing
      *                  the text of the error message.
      * @param systemId  A const pointer to an Unicode string representing
      *                  the system id of the XML file where this error
      *                  was discovered.
      * @param publicId  A const pointer to an Unicode string representing
      *                  the public id of the XML file where this error
      *                  was discovered.
      * @param lineNum   The line number where the error occurred.
      * @param colNum    The column number where the error occurred.
      * @see ErrorHandler
      */
    virtual void error
    (
        const   unsigned int                errCode
        , const XMLCh* const                msgDomain
        , const XMLErrorReporter::ErrTypes  errType
        , const XMLCh* const                errorText
        , const XMLCh* const                systemId
        , const XMLCh* const                publicId
        , const XMLFileLoc                  lineNum
        , const XMLFileLoc                  colNum
    );

    virtual void resetErrors();

	 /** Get a pointer to the error handler
      *
      * This method returns the installed error handler. If no handler
      * has been installed, then it will be a zero pointer.
      *
      * @return The pointer to the installed error handler object.
      */
    ErrorHandler* getErrorHandler();

    /** Get a const pointer to the error handler
      *
      * This method returns the installed error handler.  If no handler
      * has been installed, then it will be a zero pointer.
      *
      * @return A const pointer to the installed error handler object.
      */
    const ErrorHandler* getErrorHandler() const;


	 /** Set the error handler
      *
      * This method allows applications to install their own error handler
      * to trap error and warning messages.
      *
      * <i>Any previously set handler is merely dropped, since the parser
      * does not own them.</i>
      *
      * @param handler  A const pointer to the user supplied error
      *                 handler.
      *
      * @see #getErrorHandler
      */
    void setErrorHandler(ErrorHandler* const handler);

    //�������صĽӿ�

    //�����ļ�
    //��������:�����ű��Ľӿ�
    //����ֵ:��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������                ��������                    ����
    //source                  const InputSource&          �ű�Դ�ļ�·���Ķ���
    void parse(const InputSource& source);                        

    //�����ļ�
    //��������:�����ű��Ľӿ�
    //����ֵ:��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������      ��������                    ����
    //systemId      const XMLCh*                �ű�Դ�ļ�·����XMLCh�ַ���
    void parse(const XMLCh* const systemId);                      

    //�����ļ�
    //��������:�����ű��Ľӿ�
    //����ֵ:��
    //��������                ����
    //void                    ��
    //����˵��:
    //��������      ��������                    ����
    //systemId      const char*                �ű�Դ�ļ�·����char�ַ���
    void parse(const char* const systemId);             

    //�����ļ�
    //��������:���ý�������,��ʾ�Ƿ��ڽű��Ľ�����
    //����ֵ:��
    //��������                ����
    //void                    ��
    void reset();                                                 

    //�õ��ڴ����������
    //��������:���ý�������
    //����ֵ:�ڴ�������
    //��������                ����
    //MemoryManager*          �ڴ�������
    MemoryManager* getMemoryManager() const;


	~MyParser();

private:
	// -----------------------------------------------------------------------
    //  Initialize/Cleanup methods
    // -----------------------------------------------------------------------
    void initialize();                                             //��ʼ��
    void cleanUp();                                                //�������

	void resetInProgress();

	bool                          fParseInProgress;       //�Ƿ��ڴ�����
    MemoryManager*                fMemoryManager;         //�ڴ����
	MyScanner*                    fScanner;               //ɨ��Դ�ļ�
    ErrorHandler*                 fErrorHandler;          //���������
    /**
    * Copy constructor
    */
    MyParser(const MyParser& pool);
    MyParser& operator=(const MyParser& toAssign);
};

inline MemoryManager* MyParser::getMemoryManager() const
{
    return fMemoryManager;
}

inline ErrorHandler* MyParser::getErrorHandler()
{
    return fErrorHandler;
}

inline const ErrorHandler* MyParser::getErrorHandler() const
{
    return fErrorHandler;
}

MATCH_CPP_NAMESPACE_END

#endif