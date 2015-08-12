#if !defined(MATCHC_INCLUDE_GUARD_MYVALUE_HPP)
#define MATCHC_INCLUDE_GUARD_MYVALUE_HPP

#include <match/util/PlatformUtils.hpp>
#include <match/util/ValueHashTableOf.hpp>
#include <match/framework/psvi/XSValue.hpp>

//#import "C:\\Program Files\\Common Files\\System\\ado\\msado60_Backcompat_i386.tlb" no_namespace rename("EOF","adoEOF")

MATCH_CPP_NAMESPACE_BEGIN

//class RegularExpression;

//MyValue类是用来表示match中的所有的数据,代码原型参考XSValue
//class XMLPARSER_EXPORT MyValue : public XMemory
//{
//public:
//	//这里的枚举类型一定要和XSValue的数据类型一致！！！！！
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
//			  //com支持类型
//			  dt_pVoid                = 44,
//			  dt_pComVariant          = 45,
//			  dt_TCArray              = 46,
//			  //com支持类型
//              dt_MAXCOUNT             = 64
//    };
//
//	struct XMLPARSER_EXPORT MyArrayValue
//	{
//		MyValue::DataType fType;                  //这个记录数组的类型
//		int      fCount;                          //数组的维数
//		MyValue* fValue;                          //数组的数据指针
//		int      fArrayCount[TC_MAXARRAYCOUNT];   //数组的每维的大小
//		int      fLength;                         //数组的总长度
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
//				delete []fValue;                      //这里的指针只要分配了,就是数组形式,所以这里这样处理
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
//    //////////////////////////////////自定义方法-match-7-15/////////////////////////////////////
//    /** @name Constructors */
//    //@{这里把构造函数公有化,可以实例化对象-match改动,从私有构造函数变成公共构造函数
//    /**
//    * The default constructor
//    *
//    */
//    MyValue(
//        DataType const dt = DataType::dt_MAXCOUNT
//        ,MemoryManager*  const manager = XMLPlatformUtils::fgMemoryManager
//        );
//    //@};
//    //////////////////////////////////自定义方法-match-7-15/////////////////////////////////////
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
//	//==================钟斌 2010-9-4 添加调试处理,从字符串得到类型=============
//	//dtstring 表示要匹配的字符串
//	//isBase   表示是不是只匹配基本类型,排除数组
//
//	static MyValue::DataType getDataTypeByString(const XMLCh* const dtString,bool isBase=false);
//	//==================钟斌 10-8-9 添加调试处理,从类型得到类型的字符串=============
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
//						 //TC自定义数组
//						 MyArrayValue *f_tcarray;
//						 //TC自定义数组
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
//    //////////////////////////////////////////////////////自己添加的方法,添加一个实例,可以对外返回一个实例类型////////////////////////////////
//    //static
//    //    MyValue*  getXSValueNewInstance
//    //    (DataType              datatype
//    //    ,MemoryManager*  const manager= XMLPlatformUtils::fgMemoryManager
//    //    );
//    //////////////////////////////////////////////////自己添加的方法/////////////////////////////////////////////////////////////////////
//
//
//    ///////////////////////////////////////自定义改动////////////////////////////////////////////////
//    static const MyValue::DataGroup inGroup[];//--match 前期为了编译通过,所以将这个方法变为公共方法
//    ///////////////////////////////////////自定义改动/////////////////////////////////////////////////
//
//    ////////////////////////////////////////////////自定义方法-match-7-16/////////////////////////////////////////////////////////////////////////////////
//    bool setValue(const XMLCh * const value);
//    /////////////////////////////////////////////////自定义方法/////////////////////////////////////////////////////////////////////////
//
//    ////////////////////////////////////////////////自定义方法-match-7-17/////////////////////////////////////////////////////////////////////////////////
//    bool setValue(const MyValue *const value);
//    /////////////////////////////////////////////////自定义方法/////////////////////////////////////////////////////////////////////////
//
//    ////////////////////////////////////////////////自定义方法-match-7-17/////////////////////////////////////////////////////////////////////////////////
//    void reset();
//    /////////////////////////////////////////////////自定义方法/////////////////////////////////////////////////////////////////////////
//
//    ////////////////////////////////////////////////自定义方法-match-7-17/////////////////////////////////////////////////////////////////////////////////
//    bool isEmpty();
//    /////////////////////////////////////////////////自定义方法/////////////////////////////////////////////////////////////////////////
//
//	bool isEqual(const MyValue *const value);
//
//	bool isEqual(const XMLCh * const value);
//
//private:
//    /////////////////////////////////////构造函数公有化/////////////////////////////////////////////////
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
