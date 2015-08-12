#if !defined(MATCHC_INCLUDE_GUARD_MYVALUE_HPP)
#define MATCHC_INCLUDE_GUARD_MYVALUE_HPP

#include <match/util/PlatformUtils.hpp>
#include <match/util/ValueHashTableOf.hpp>
#include <match/framework/psvi/XSValue.hpp>

//#import "C:\\Program Files\\Common Files\\System\\ado\\msado60_Backcompat_i386.tlb" no_namespace rename("EOF","adoEOF")

MATCH_CPP_NAMESPACE_BEGIN

//class RegularExpression;

//MyValue����������ʾmatch�е����е�����,����ԭ�Ͳο�XSValue
//class XMLPARSER_EXPORT MyValue : public XMemory
//{
//public:
//	//�����ö������һ��Ҫ��XSValue����������һ�£���������
//    enum DataType {
//              dt_string               = 0,
//              dt_boolean              = 1,
//              dt_decimal              = 2,
//              dt_float                = 3,
//              dt_double               = 4,
//              dt_duration             = 5,
//              dt_dateTime             = 6,
//              dt_time                 = 7,
//              dt_date                 = 8,
//              dt_gYearMonth           = 9,
//              dt_gYear                = 10,
//              dt_gMonthDay            = 11,
//              dt_gDay                 = 12,
//              dt_gMonth               = 13,
//              dt_hexBinary            = 14,
//              dt_base64Binary         = 15,
//              dt_anyURI               = 16,
//              dt_QName                = 17,
//              dt_NOTATION             = 18,
//              dt_normalizedString     = 19,
//              dt_token                = 20,
//              dt_language             = 21,
//              dt_NMTOKEN              = 22,
//              dt_NMTOKENS             = 23,
//              dt_Name                 = 24,
//              dt_NCName               = 25,
//              dt_ID                   = 26,
//              dt_IDREF                = 27,
//              dt_IDREFS               = 28,
//              dt_ENTITY               = 29,
//              dt_ENTITIES             = 30,
//              dt_integer              = 31,
//              dt_nonPositiveInteger   = 32,
//              dt_negativeInteger      = 33,
//              dt_long                 = 34,
//              dt_int                  = 35,
//              dt_short                = 36,
//              dt_byte                 = 37,
//              dt_nonNegativeInteger   = 38,
//              dt_unsignedLong         = 39,
//              dt_unsignedInt          = 40,
//              dt_unsignedShort        = 41,
//              dt_unsignedByte         = 42,
//              dt_positiveInteger      = 43,
//			  //com֧������
//			  dt_pVoid                = 44,
//			  dt_pComVariant          = 45,
//			  dt_TCArray              = 46,
//			  //com֧������
//              dt_MAXCOUNT             = 64
//    };
//
//	struct XMLPARSER_EXPORT MyArrayValue
//	{
//		MyValue::DataType fType;                  //�����¼���������
//		int      fCount;                          //�����ά��
//		MyValue* fValue;                          //���������ָ��
//		int      fArrayCount[TC_MAXARRAYCOUNT];   //�����ÿά�Ĵ�С
//		int      fLength;                         //������ܳ���
//		MyArrayValue()
//		{
//			fType=MyValue::dt_MAXCOUNT;
//			fValue=NULL;
//			::memset(fArrayCount,0,sizeof(int)*TC_MAXARRAYCOUNT);
//			fCount=0;
//			fLength=0;
//		}
//		~MyArrayValue()
//		{
//			if(fValue)
//				delete []fValue;                      //�����ָ��ֻҪ������,����������ʽ,����������������
//			fType=MyValue::dt_MAXCOUNT;
//			fCount=0;
//			fLength=0;
//		}
//	};
//
//    enum DataGroup {
//            dg_numerics,
//            dg_datetimes,
//            dg_strings
//    };
//
//	void initialArray(DataType type,int arrayCount);
//
//	void redefArrayCount(int c1,int c2=-1,int c3=-1);
//
//    //////////////////////////////////�Զ��巽��-match-7-15/////////////////////////////////////
//    /** @name Constructors */
//    //@{����ѹ��캯�����л�,����ʵ��������-match�Ķ�,��˽�й��캯����ɹ������캯��
//    /**
//    * The default constructor
//    *
//    */
//    MyValue(
//        DataType const dt = DataType::dt_MAXCOUNT
//        ,MemoryManager*  const manager = XMLPlatformUtils::fgMemoryManager
//        );
//    //@};
//    //////////////////////////////////�Զ��巽��-match-7-15/////////////////////////////////////
//
//    //  Constructors and Destructor
//    // -----------------------------------------------------------------------
//    /** @name Destructor */
//    //@{
//    ~MyValue();
//    //@}
//
//    //---------------------------------
//    /** @name Externalization methods */
//    //@{
//
//    /**
//      * Validate a given string of the data type specified
//      *
//      * @param  content    data to be validated
//      * @param  datatype   schema datatype
//      * @param  status     validation status which is set upon validation fails
//      * @param  version    xml version
//      * @param  manager    memory manager provided
//      */
//    //static
//    //bool     validate
//    //         (
//    //            const XMLCh*          const content
//    //          ,       DataType              datatype
//    //          ,       Status&               status
//    //          ,       XMLVersion            version    = ver_10
//    //          ,       MemoryManager*  const manager    = XMLPlatformUtils::fgMemoryManager
//    //         );
//
//    /**
//      * Get the canonical representation for a given string of the
//      * data type specified
//      *
//      * @param  content    raw data
//      * @param  datatype   schema datatype
//      * @param  status     validation status which is set upon validation fails
//      * @param  version    xml version
//      * @param  toValidate to validate the content before generate canonical representation
//      * @param  manager    memory manager provided
//      */
//    //static
//    //XMLCh*   getCanonicalRepresentation
//    //         (
//    //            const XMLCh*          const content
//    //          ,       DataType              datatype
//    //          ,       Status&               status
//    //          ,       XMLVersion            version    = ver_10
//    //          ,       bool                  toValidate = true
//    //          ,       MemoryManager*  const manager    = XMLPlatformUtils::fgMemoryManager
//    //         );
//
//    /**
//      * Get the actual value, in the form of MyValue, for a given string of the
//      * data type specified
//      *
//      * Client application is responsible for the deletion of the MyValue returned.
//      *
//      * @param  content    raw data
//      * @param  datatype   schema datatype
//      * @param  status     validation status which is set upon validation fails
//      * @param  version    xml version
//      * @param  toValidate to validate the content before generate actual value
//      * @param  manager    memory manager provided
//      */
//    //static
//    //MyValue* getActualValue
//    //         (
//    //            const XMLCh*          const content
//    //          ,       DataType              datatype
//    //          ,       Status&               status
//    //          ,       XMLVersion            version    = ver_10
//    //          ,       bool                  toValidate = true
//    //          ,       MemoryManager*  const manager    = XMLPlatformUtils::fgMemoryManager
//    //         );
//
//    //static
//    //DataType  getDataType  (const XMLCh* const dtString);
//
//	//==================�ӱ� 2010-9-4 ��ӵ��Դ���,���ַ����õ�����=============
//	//dtstring ��ʾҪƥ����ַ���
//	//isBase   ��ʾ�ǲ���ֻƥ���������,�ų�����
//
//	static MyValue::DataType getDataTypeByString(const XMLCh* const dtString,bool isBase=false);
//	//==================�ӱ� 10-8-9 ��ӵ��Դ���,�����͵õ����͵��ַ���=============
//	static const XMLCh*  getDataTypeString (DataType type,bool isChinese=false);
//
//    //@}
//
//    //----------------------------------
//    /** public data */
//
//    struct XSValue_Data {
//        DataType f_datatype;
//        union {
//                         bool      f_bool;
//                         char      f_char;
//                unsigned char      f_uchar;
//                         short     f_short;
//                unsigned short     f_ushort;
//                         int       f_int;
//                unsigned int       f_uint;
//                         long      f_long;
//                unsigned long      f_ulong;
//                         float     f_float;
//                         double    f_double;
//                         XMLCh*    f_strVal;
//                         XMLByte*  f_byteVal;
//						 //TC�Զ�������
//						 MyArrayValue *f_tcarray;
//						 //TC�Զ�������
//                //struct decimal {
//                //                double           f_dvalue;
//                //} f_decimal;
//
//                //struct datetime {
//                //                int       f_year;
//                //                int       f_month;
//                //                int       f_day;
//                //                int       f_hour;
//                //                int       f_min;
//                //                int       f_second;
//                //                double    f_milisec;
//
//                //} f_datetime;
//
//                //struct doubletype {
//                //                double          f_double;
//                //                DoubleFloatType f_doubleEnum;
//                //} f_doubleType;
//
//                //struct floattype {
//                //                float           f_float;
//                //                DoubleFloatType f_floatEnum;
//                //} f_floatType;
//        } fValue;
//
//    } fData;
//
//    //////////////////////////////////////////////////////�Լ���ӵķ���,���һ��ʵ��,���Զ��ⷵ��һ��ʵ������////////////////////////////////
//    //static
//    //    MyValue*  getXSValueNewInstance
//    //    (DataType              datatype
//    //    ,MemoryManager*  const manager= XMLPlatformUtils::fgMemoryManager
//    //    );
//    //////////////////////////////////////////////////�Լ���ӵķ���/////////////////////////////////////////////////////////////////////
//
//
//    ///////////////////////////////////////�Զ���Ķ�////////////////////////////////////////////////
//    static const MyValue::DataGroup inGroup[];//--match ǰ��Ϊ�˱���ͨ��,���Խ����������Ϊ��������
//    ///////////////////////////////////////�Զ���Ķ�/////////////////////////////////////////////////
//
//    ////////////////////////////////////////////////�Զ��巽��-match-7-16/////////////////////////////////////////////////////////////////////////////////
//    bool setValue(const XMLCh * const value);
//    /////////////////////////////////////////////////�Զ��巽��/////////////////////////////////////////////////////////////////////////
//
//    ////////////////////////////////////////////////�Զ��巽��-match-7-17/////////////////////////////////////////////////////////////////////////////////
//    bool setValue(const MyValue *const value);
//    /////////////////////////////////////////////////�Զ��巽��/////////////////////////////////////////////////////////////////////////
//
//    ////////////////////////////////////////////////�Զ��巽��-match-7-17/////////////////////////////////////////////////////////////////////////////////
//    void reset();
//    /////////////////////////////////////////////////�Զ��巽��/////////////////////////////////////////////////////////////////////////
//
//    ////////////////////////////////////////////////�Զ��巽��-match-7-17/////////////////////////////////////////////////////////////////////////////////
//    bool isEmpty();
//    /////////////////////////////////////////////////�Զ��巽��/////////////////////////////////////////////////////////////////////////
//
//	bool isEqual(const MyValue *const value);
//
//	bool isEqual(const XMLCh * const value);
//
//private:
//    /////////////////////////////////////���캯�����л�/////////////////////////////////////////////////
//
//    // -----------------------------------------------------------------------
//    //  Unimplemented constructors and operators
//    // -----------------------------------------------------------------------
//    MyValue(const MyValue&);
//    MyValue & operator=(const MyValue &);
//    MemoryManager*      fMemoryManager;
//};

MATCH_CPP_NAMESPACE_END

#endif
