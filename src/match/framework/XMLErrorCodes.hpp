// This file is generated, don't edit it!!

#if !defined(MATCHC_INCLUDE_GUARD_ERRHEADER_XMLErrs)
#define MATCHC_INCLUDE_GUARD_ERRHEADER_XMLErrs

#include <match/framework/XMLErrorReporter.hpp>
#include <match/util/MatchDefs.hpp>
#include <match/dom/DOMError.hpp>

MATCH_CPP_NAMESPACE_BEGIN

class XMLErrs
{
public :
    enum Codes
    {
        NoError                            = 0
        , W_LowBounds                        = 1
        , NotationAlreadyExists              = 2
        , AttListAlreadyExists               = 3
        , ContradictoryEncoding              = 4
        , UndeclaredElemInCM                 = 5
        , UndeclaredElemInAttList            = 6
        , XMLException_Warning               = 7
        , XIncludeResourceErrorWarning       = 8
        , XIncludeCannotOpenFile             = 9
        , XIncludeIncludeFailedResourceError   = 10
        , W_HighBounds                       = 11
        , E_LowBounds                        = 12
        , FeatureUnsupported                 = 13
        , TopLevelNoNameComplexType          = 14
        , TopLevelNoNameAttribute            = 15
        , NoNameRefAttribute                 = 16
        , NoNameRefElement                   = 17
        , NoNameRefGroup                     = 18
        , NoNameRefAttGroup                  = 19
        , AnonComplexTypeWithName            = 20
        , AnonSimpleTypeWithName             = 21
        , InvalidElementContent              = 22
        , SimpleTypeContentError             = 23
        , ExpectedSimpleTypeInList           = 24
        , ListUnionRestrictionError          = 25
        , SimpleTypeDerivationByListError    = 26
        , ExpectedSimpleTypeInRestriction    = 27
        , DuplicateFacet                     = 28
        , ExpectedSimpleTypeInUnion          = 29
        , EmptySimpleTypeContent             = 30
        , InvalidSimpleContent               = 31
        , UnspecifiedBase                    = 32
        , InvalidComplexContent              = 33
        , SchemaElementContentError          = 34
        , ContentError                       = 35
        , UnknownSimpleType                  = 36
        , UnknownComplexType                 = 37
        , UnresolvedPrefix                   = 38
        , RefElementNotFound                 = 39
        , TypeNotFound                       = 40
        , TopLevelAttributeNotFound          = 41
        , InvalidChildInComplexType          = 42
        , BaseTypeNotFound                   = 43
        , DatatypeValidatorCreationError     = 44
        , InvalidChildFollowingSimpleContent   = 45
        , InvalidChildFollowingConplexContent   = 46
        , AttributeDefaultFixedValue         = 47
        , NotOptionalDefaultAttValue         = 48
        , DuplicateAttribute                 = 49
        , AttributeWithTypeAndSimpleType     = 50
        , AttributeSimpleTypeNotFound        = 51
        , ElementWithFixedAndDefault         = 52
        , InvalidDeclarationName             = 53
        , ElementWithTypeAndAnonType         = 54
        , NotSimpleOrMixedElement            = 55
        , DisallowedSimpleTypeExtension      = 56
        , InvalidSimpleContentBase           = 57
        , InvalidComplexTypeBase             = 58
        , InvalidChildInSimpleContent        = 59
        , InvalidChildInComplexContent       = 60
        , AnnotationError                    = 61
        , DisallowedBaseDerivation           = 62
        , InvalidBlockValue                  = 63
        , InvalidFinalValue                  = 64
        , InvalidSubstitutionGroupElement    = 65
        , SubstitutionGroupTypeMismatch      = 66
        , DuplicateElementDeclaration        = 67
        , InvalidAttValue                    = 68
        , AttributeRefContentError           = 69
        , DuplicateRefAttribute              = 70
        , ForbiddenDerivationByRestriction   = 71
        , ForbiddenDerivationByExtension     = 72
        , BaseNotComplexType                 = 73
        , ImportNamespaceDifference          = 74
        , DeclarationNoSchemaLocation        = 75
        , IncludeNamespaceDifference         = 76
        , OnlyAnnotationExpected             = 77
        , InvalidAttributeContent            = 78
        , AttributeRequiredGlobal            = 79
        , AttributeRequiredLocal             = 80
        , AttributeDisallowedGlobal          = 81
        , AttributeDisallowedLocal           = 82
        , InvalidMin2MaxOccurs               = 83
        , AnyAttributeContentError           = 84
        , NoNameGlobalElement                = 85
        , NoCircularDefinition               = 86
        , DuplicateGlobalType                = 87
        , DuplicateGlobalDeclaration         = 88
        , WS_CollapseExpected                = 89
        , Import_1_1                         = 90
        , Import_1_2                         = 91
        , ElemIDValueConstraint              = 92
        , NoNotationType                     = 93
        , EmptiableMixedContent              = 94
        , EmptyComplexRestrictionDerivation   = 95
        , MixedOrElementOnly                 = 96
        , InvalidContentRestriction          = 97
        , ForbiddenDerivation                = 98
        , AtomicItemType                     = 99
        , GroupContentError                  = 100
        , AttGroupContentError               = 101
        , MinMaxOnGroupChild                 = 102
        , DeclarationNotFound                = 103
        , AllContentLimited                  = 104
        , BadMinMaxAllCT                     = 105
        , BadMinMaxAllElem                   = 106
        , DuplicateAttInDerivation           = 107
        , NotExpressibleWildCardIntersection   = 108
        , BadAttDerivation_1                 = 109
        , BadAttDerivation_2                 = 110
        , BadAttDerivation_3                 = 111
        , BadAttDerivation_4                 = 112
        , BadAttDerivation_5                 = 113
        , BadAttDerivation_6                 = 114
        , BadAttDerivation_7                 = 115
        , BadAttDerivation_8                 = 116
        , BadAttDerivation_9                 = 117
        , AllContentError                    = 118
        , RedefineNamespaceDifference        = 119
        , Redefine_InvalidSimpleType         = 120
        , Redefine_InvalidSimpleTypeBase     = 121
        , Redefine_InvalidComplexType        = 122
        , Redefine_InvalidComplexTypeBase    = 123
        , Redefine_InvalidGroupMinMax        = 124
        , Redefine_DeclarationNotFound       = 125
        , Redefine_GroupRefCount             = 126
        , Redefine_AttGroupRefCount          = 127
        , Redefine_InvalidChild              = 128
        , Notation_DeclNotFound              = 129
        , IC_DuplicateDecl                   = 130
        , IC_BadContent                      = 131
        , IC_KeyRefReferNotFound             = 132
        , IC_KeyRefCardinality               = 133
        , IC_XPathExprMissing                = 134
        , AttUseCorrect                      = 135
        , AttDeclPropCorrect3                = 136
        , AttDeclPropCorrect5                = 137
        , AttGrpPropCorrect3                 = 138
        , InvalidTargetNSValue               = 139
        , XMLException_Error                 = 140
        , InvalidRedefine                    = 141
        , InvalidNSReference                 = 142
        , NotAllContent                      = 143
        , InvalidAnnotationContent           = 144
        , InvalidFacetName                   = 145
        , InvalidXMLSchemaRoot               = 146
        , CircularSubsGroup                  = 147
        , ELTSchemaNS                        = 148
        , InvalidAttTNS                      = 149
        , NSDeclInvalid                      = 150
        , DOMLevel1Node                      = 151
        , DuplicateAnyAttribute              = 152
        , AnyAttributeBeforeAttribute        = 153
        , E_HighBounds                       = 154
        , F_LowBounds                        = 155
        , EntityExpansionLimitExceeded       = 156
        , ExpectedCommentOrCDATA             = 157
        , ExpectedAttrName                   = 158
        , ExpectedNotationName               = 159
        , NoRepInMixed                       = 160
        , ExpectedDefAttrDecl                = 161
        , ExpectedEqSign                     = 162
        , ExpectedElementName                = 163
        , CommentsMustStartWith              = 164
        , InvalidDocumentStructure           = 165
        , ExpectedDeclString                 = 166
        , BadXMLVersion                      = 167
        , UnsupportedXMLVersion              = 168
        , UnterminatedXMLDecl                = 169
        , BadXMLEncoding                     = 170
        , BadStandalone                      = 171
        , UnterminatedComment                = 172
        , PINameExpected                     = 173
        , UnterminatedPI                     = 174
        , InvalidCharacter                   = 175
        , UnterminatedStartTag               = 176
        , ExpectedAttrValue                  = 177
        , UnterminatedEndTag                 = 178
        , ExpectedAttributeType              = 179
        , ExpectedEndOfTagX                  = 180
        , ExpectedMarkup                     = 181
        , NotValidAfterContent               = 182
        , ExpectedComment                    = 183
        , ExpectedCommentOrPI                = 184
        , ExpectedWhitespace                 = 185
        , NoRootElemInDOCTYPE                = 186
        , ExpectedQuotedString               = 187
        , ExpectedPublicId                   = 188
        , InvalidPublicIdChar                = 189
        , UnterminatedDOCTYPE                = 190
        , InvalidCharacterInIntSubset        = 191
        , UnexpectedWhitespace               = 192
        , InvalidCharacterInAttrValue        = 193
        , ExpectedMarkupDecl                 = 194
        , TextDeclNotLegalHere               = 195
        , ConditionalSectInIntSubset         = 196
        , ExpectedPEName                     = 197
        , UnterminatedEntityDecl             = 198
        , InvalidCharacterRef                = 199
        , UnterminatedCharRef                = 200
        , ExpectedEntityRefName              = 201
        , EntityNotFound                     = 202
        , NoUnparsedEntityRefs               = 203
        , UnterminatedEntityRef              = 204
        , RecursiveEntity                    = 205
        , PartialMarkupInEntity              = 206
        , UnterminatedElementDecl            = 207
        , ExpectedContentSpecExpr            = 208
        , ExpectedAsterisk                   = 209
        , UnterminatedContentModel           = 210
        , ExpectedSystemOrPublicId           = 211
        , UnterminatedNotationDecl           = 212
        , ExpectedSeqChoiceLeaf              = 213
        , ExpectedChoiceOrCloseParen         = 214
        , ExpectedSeqOrCloseParen            = 215
        , ExpectedEnumValue                  = 216
        , ExpectedEnumSepOrParen             = 217
        , UnterminatedEntityLiteral          = 218
        , MoreEndThanStartTags               = 219
        , ExpectedOpenParen                  = 220
        , AttrAlreadyUsedInSTag              = 221
        , BracketInAttrValue                 = 222
        , Expected2ndSurrogateChar           = 223
        , ExpectedEndOfConditional           = 224
        , ExpectedIncOrIgn                   = 225
        , ExpectedINCLUDEBracket             = 226
        , UnexpectedEOE                      = 227
        , PEPropogated                       = 228
        , ExtraCloseSquare                   = 229
        , PERefInMarkupInIntSubset           = 230
        , EntityPropogated                   = 231
        , ExpectedNumericalCharRef           = 232
        , ExpectedOpenSquareBracket          = 233
        , BadSequenceInCharData              = 234
        , IllegalSequenceInComment           = 235
        , UnterminatedCDATASection           = 236
        , ExpectedNDATA                      = 237
        , NDATANotValidForPE                 = 238
        , HexRadixMustBeLowerCase            = 239
        , DeclStringRep                      = 240
        , DeclStringsInWrongOrder            = 241
        , NoExtRefsInAttValue                = 242
        , XMLDeclMustBeLowerCase             = 243
        , ExpectedEntityValue                = 244
        , BadDigitForRadix                   = 245
        , EndedWithTagsOnStack               = 246
        , NestedCDATA                        = 247
        , UnknownPrefix                      = 248
        , PartialTagMarkupError              = 249
        , EmptyMainEntity                    = 250
        , CDATAOutsideOfContent              = 251
        , Unexpected2ndSurrogateChar         = 252
        , NoPIStartsWithXML                  = 253
        , XMLDeclMustBeFirst                 = 254
        , XMLVersionRequired                 = 255
        , StandaloneNotLegal                 = 256
        , EncodingRequired                   = 257
        , ColonNotLegalWithNS                = 258
        , XMLException_Fatal                 = 259
        , BadSchemaLocation                  = 260
        , SchemaScanFatalError               = 261
        , IllegalRefInStandalone             = 262
        , PEBetweenDecl                      = 263
        , NoEmptyStrNamespace                = 264
        , NoUseOfxmlnsAsPrefix               = 265
        , NoUseOfxmlnsURI                    = 266
        , PrefixXMLNotMatchXMLURI            = 267
        , XMLURINotMatchXMLPrefix            = 268
        , NoXMLNSAsElementPrefix             = 269
        , CT_SimpleTypeChildRequired         = 270
        , InvalidRootElemInDOCTYPE           = 271
        , InvalidElementName                 = 272
        , InvalidAttrName                    = 273
        , InvalidEntityRefName               = 274
        , DuplicateDocTypeDecl               = 275
        , XIncludeOrphanFallback             = 276
        , XIncludeNoHref                     = 277
        , XIncludeXPointerNotSupported       = 278
        , XIncludeInvalidParseVal            = 279
        , XIncludeMultipleFallbackElems      = 280
        , XIncludeIncludeFailedNoFallback    = 281
        , XIncludeCircularInclusionLoop      = 282
        , XIncludeCircularInclusionDocIncludesSelf   = 283
        , XIncludeDisallowedChild            = 284
        , XIncludeConflictingNotation        = 285
        , XIncludeConflictingEntity          = 286
        , F_HighBounds                       = 287

		//////////////////////////////////////////////////////////自定义异常代码/////////////////////////////////
		//my语法扫描异常
		, my_prodef_UnfinishedProdef        = 480	         //不完整的预定义,只有#头,找不到include和import关键字
		, my_prodef_IncludeDefErr           = 481	         //include定义错误
		, my_prodef_IncludeNoFindPath       = 482	         //'{0}'是错误的include语句,没有找到文件路径
		, my_prodef_NoFindIncludeSymbol     = 483	         //'{0}'是错误的include语句,没有找到成对的\"符号
		, my_prodef_ImportNoComName         = 484	         //'{0}'是错误的import导入语句,没有找到接口名
		, my_prodef_ImportNoNSName          = 485	         //'{0}'是错误的import导入语句,没有找到接口空间名定义
	    , my_NameSP_NoFindNameSP            = 486            //找不到命名空间定义  -空间     
		, my_NameSP_NameSPDefErr            = 487            //命令空间定义错误    -空间
		, my_GolbalVar_InitialErr           = 488	         //'{0}'全局变量的初始化值,只能是常量
		, my_NameSP_NameSPUnValidity        = 489	         //{0}为不合法的空间名称或者是重复的命名空间
		, my_NameSP_NameSPNotEnd            = 490	         //没有找到命名空间结束定义
		, my_commo_MoreStr                  = 491	         //语句末尾出现多余的字符{0}
		, my_commo_UndefinedStr             = 492	         //发现没定义的字符{0}
		, my_Var_NoFindVarNameDef           = 493	         //没有发现变量名定义,定义后面没有内容
		, my_ArrayType_DefErr               = 494            //错误的数组类型定义  -空间
		, my_Var_DefUnValidity              = 495	         //{0}不合法的变量定义,等号在最前面或者最后面
		, my_Var_NameIsUnvalid              = 496	         //'{0}'为不合法的命名定义,只有以下划线和字母开头,只支持数字,字母,下划线进行命名,长度不能超过32
		, my_Var_ReDefine                   = 497	         //{0}已经定义过的变量名
		, my_Var_ValeIsUnvalidity           = 498	         //不合法的变量值,{0}不是{1}类型
		, my_Var_PostionErr                 = 499			 //'{0}'变量定义语句出现在不合法的位置(需要出现在命名空间或者函数中)
		, my_Func_UnDefine                  = 500	         //找不到函数头定义
		, my_FuncRet_Unvalid                = 501	         //'{0}'是不合法的函数返回值类型
		, my_FuncFormatErr                  = 502	         //{0}函数头格式错误
		, my_FuncName_Unvalidity            = 503	         //{0}不合法的函数名称
		, my_FuncName_Redef                 = 504	         //{0}在{1}命名空间中已经存在此函数名称定义
		, my_FuncPram_DefErr                = 505	         //{0}重复的参数名定义
		, my_FuncPostion_Err                = 506	         //'{0}{1}'函数定义出现在不合法的位置(只能出现的命名空间中)
		, my_FuncEnd_NoFind                 = 507	         //找不到函数结尾定义
		, my_Ret_NoExpres                   = 508	         //错误的return语句,没有找到返回表达试
		, my_RetStatmentErr                 = 509	         //错误的return表达试{0}
		, my_RetPostion_Err                 = 510	         //语句'{0}'出现在不合法的位置,应该出现在函数定义里面
		, my_UndefCondition                 = 511	         //找不到'{0}'语句条件表达式定义
		, my_Commo_ConditionErr             = 512	         //{0}语句的条件表达试错误{1}
		, my_WhilePostion_Err               = 513	         //while语句出现的不合法的位置,应该出现在函数定义里面
		, my_WhileNoEnd                     = 514	         //找不到 endwhile 语句
		, my_BreakPostErr                   = 515	         //Break语句出现在不合法的位置
		, my_ContPostErr                    = 516	         //continue语句出现在不合法的位置
		, my_ReptCondtionErr                = 517	         //{0}条件表达式错误,repeat条件表达试为整形
		, my_Rept_UnChageUI                 = 518	         //{0}不能转换为无符号整数
		, my_ReptPostion_Err                = 519	         //repeat出现在不合法的位置
		, my_ReptNoEnd                      = 520	         //没有找到 endrepeat 语句
		, my_IfPostion_Err                  = 521	         //if语句出现在不合法的位置
		, my_If_NoEnd                       = 522	         // endif与if不匹配
		, my_ElsePostion_Err                = 523	         // else语句出现在不合法的位置
		, my_AssiStatment_Err               = 524	         // 不正确的赋值语句,变量:{0}之间发现空格符号
		, my_Assi_NoExpress                 = 525	         // 赋值语句没有表达式
		, my_Assi_UndefVarName              = 526	         // {0}未定义的变量名
		, my_Assi_UnValidExp                = 527	         // '{0}={1}' 等号两边的数据类型不一致
		, my_Assi_PostionErr                = 528	         // 赋值语句 '{0}' 的位置不合法
		//, my_CallStatmentErr                = 529	         // call后未发现调用的函数语句(已经废除不用了)
		, my_CallStatGramErr                = 529	         // 函数语句 '{0}' 发现语法错误
		, my_CallPostionErr                 = 530	         // 函数调用语句出现的位置不合法
		, my_Comm_ConditionErr              = 531	         // 当前条件表达式'{0}'没有用成对的括号括起来,或者是')'(闭括号)之后出现多余的字符
		, my_NameSP_SPBackNoSpace           = 532	         // '{0 ' 好像是命名空间关键字与命名空间变量名中间没有空格，应该加上空格
		, my_Var_BackNoSpace                = 533	         // '{0}' 好像是变量定义关键字与变量名中间没有空格，应该加上空格
		, my_Func_FuncBackNoSpace           = 534	         // '{0}' 好像是函数关键字与函数返回类型中间没有空格，应该加上空格
		, my_Func_FuncTypeBackNoSpace       = 535	         // '{0}' 好像是函数返回类型与函数名中间没有空格，应该加上空格
		, my_Ret_RetStatmentBackNoSpace     = 536	         // return关键字与return之后的语句中间没有空格，应该加上空格
		, my_Func_NoReturnStatment          = 537	         // {0},函数{1}里面应该有return语句的
		, my_Repeat_UndefCondition          = 538	         // 找不到repeat条件表达试

		, my_Express_ErroExp                = 539	         // '{0}' 是一个错误的语句
	
		///////未加的消息////////////////


	    ,my_SelectPostion_Err               = 540            //select语句出现在错误的位置,应该出现在函数,或者while,repeat,if,select语句里面
		,my_Select_NoEnd                    = 541            //没有找到select的结尾语句endselect
	    ,my_CaseDefaultPostion_Err          = 542           //{0}语句出现在错误的位置,应该出现在select语句里面
	    ,my_CaseDefault_NoEnd               = 543            //{0}语句没有正确的结束

	    ,my_For_InitiaErr                   = 544            //for语句 初始化语句报错 {0}
	    ,my_For_ExpErr                      = 545            //for语句 条件语句报错 {0}
	    ,my_For_StepErr                     = 546            //for语句 步长语句报错 {0}

		,my_Error_Lable                     = 547            //错误的标记定义格式 {0}
		,my_Error_Goto                      = 548            //错误的Goto语句定义格式 {0}
		,my_GotoNoSpace                     = 549	         // '{0}' 好像是goto跳转关键字与标记中间没有空格，应该加上空格
		,my_LableRedefine                   = 550	         // '{0}' 是已经定义的标记
		//my语法扫描异常
		////////////////////////////////////////////////////////自定义异常/////////////////////////////////////////


    };

    static bool isFatal(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds));
    }

    static bool isWarning(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds));
    }

    static bool isError(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds));
    }

    static XMLErrorReporter::ErrTypes errorType(const XMLErrs::Codes toCheck)
    {
        if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))
            return XMLErrorReporter::ErrType_Warning;
        else if ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds))
            return XMLErrorReporter::ErrType_Fatal;
        else if ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds))
            return XMLErrorReporter::ErrType_Error;
        return XMLErrorReporter::ErrTypes_Unknown;
    }
    static DOMError::ErrorSeverity  DOMErrorType(const XMLErrs::Codes toCheck)
    {
        if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))
            return DOMError::DOM_SEVERITY_WARNING;
        else if ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds))
            return DOMError::DOM_SEVERITY_FATAL_ERROR;
        else return DOMError::DOM_SEVERITY_ERROR;
    }

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLErrs();
};

MATCH_CPP_NAMESPACE_END

#endif

