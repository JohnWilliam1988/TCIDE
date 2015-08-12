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

		//////////////////////////////////////////////////////////�Զ����쳣����/////////////////////////////////
		//my�﷨ɨ���쳣
		, my_prodef_UnfinishedProdef        = 480	         //��������Ԥ����,ֻ��#ͷ,�Ҳ���include��import�ؼ���
		, my_prodef_IncludeDefErr           = 481	         //include�������
		, my_prodef_IncludeNoFindPath       = 482	         //'{0}'�Ǵ����include���,û���ҵ��ļ�·��
		, my_prodef_NoFindIncludeSymbol     = 483	         //'{0}'�Ǵ����include���,û���ҵ��ɶԵ�\"����
		, my_prodef_ImportNoComName         = 484	         //'{0}'�Ǵ����import�������,û���ҵ��ӿ���
		, my_prodef_ImportNoNSName          = 485	         //'{0}'�Ǵ����import�������,û���ҵ��ӿڿռ�������
	    , my_NameSP_NoFindNameSP            = 486            //�Ҳ��������ռ䶨��  -�ռ�     
		, my_NameSP_NameSPDefErr            = 487            //����ռ䶨�����    -�ռ�
		, my_GolbalVar_InitialErr           = 488	         //'{0}'ȫ�ֱ����ĳ�ʼ��ֵ,ֻ���ǳ���
		, my_NameSP_NameSPUnValidity        = 489	         //{0}Ϊ���Ϸ��Ŀռ����ƻ������ظ��������ռ�
		, my_NameSP_NameSPNotEnd            = 490	         //û���ҵ������ռ��������
		, my_commo_MoreStr                  = 491	         //���ĩβ���ֶ�����ַ�{0}
		, my_commo_UndefinedStr             = 492	         //����û������ַ�{0}
		, my_Var_NoFindVarNameDef           = 493	         //û�з��ֱ���������,�������û������
		, my_ArrayType_DefErr               = 494            //������������Ͷ���  -�ռ�
		, my_Var_DefUnValidity              = 495	         //{0}���Ϸ��ı�������,�Ⱥ�����ǰ����������
		, my_Var_NameIsUnvalid              = 496	         //'{0}'Ϊ���Ϸ�����������,ֻ�����»��ߺ���ĸ��ͷ,ֻ֧������,��ĸ,�»��߽�������,���Ȳ��ܳ���32
		, my_Var_ReDefine                   = 497	         //{0}�Ѿ�������ı�����
		, my_Var_ValeIsUnvalidity           = 498	         //���Ϸ��ı���ֵ,{0}����{1}����
		, my_Var_PostionErr                 = 499			 //'{0}'���������������ڲ��Ϸ���λ��(��Ҫ�����������ռ���ߺ�����)
		, my_Func_UnDefine                  = 500	         //�Ҳ�������ͷ����
		, my_FuncRet_Unvalid                = 501	         //'{0}'�ǲ��Ϸ��ĺ�������ֵ����
		, my_FuncFormatErr                  = 502	         //{0}����ͷ��ʽ����
		, my_FuncName_Unvalidity            = 503	         //{0}���Ϸ��ĺ�������
		, my_FuncName_Redef                 = 504	         //{0}��{1}�����ռ����Ѿ����ڴ˺������ƶ���
		, my_FuncPram_DefErr                = 505	         //{0}�ظ��Ĳ���������
		, my_FuncPostion_Err                = 506	         //'{0}{1}'������������ڲ��Ϸ���λ��(ֻ�ܳ��ֵ������ռ���)
		, my_FuncEnd_NoFind                 = 507	         //�Ҳ���������β����
		, my_Ret_NoExpres                   = 508	         //�����return���,û���ҵ����ر����
		, my_RetStatmentErr                 = 509	         //�����return�����{0}
		, my_RetPostion_Err                 = 510	         //���'{0}'�����ڲ��Ϸ���λ��,Ӧ�ó����ں�����������
		, my_UndefCondition                 = 511	         //�Ҳ���'{0}'����������ʽ����
		, my_Commo_ConditionErr             = 512	         //{0}������������Դ���{1}
		, my_WhilePostion_Err               = 513	         //while�����ֵĲ��Ϸ���λ��,Ӧ�ó����ں�����������
		, my_WhileNoEnd                     = 514	         //�Ҳ��� endwhile ���
		, my_BreakPostErr                   = 515	         //Break�������ڲ��Ϸ���λ��
		, my_ContPostErr                    = 516	         //continue�������ڲ��Ϸ���λ��
		, my_ReptCondtionErr                = 517	         //{0}�������ʽ����,repeat���������Ϊ����
		, my_Rept_UnChageUI                 = 518	         //{0}����ת��Ϊ�޷�������
		, my_ReptPostion_Err                = 519	         //repeat�����ڲ��Ϸ���λ��
		, my_ReptNoEnd                      = 520	         //û���ҵ� endrepeat ���
		, my_IfPostion_Err                  = 521	         //if�������ڲ��Ϸ���λ��
		, my_If_NoEnd                       = 522	         // endif��if��ƥ��
		, my_ElsePostion_Err                = 523	         // else�������ڲ��Ϸ���λ��
		, my_AssiStatment_Err               = 524	         // ����ȷ�ĸ�ֵ���,����:{0}֮�䷢�ֿո����
		, my_Assi_NoExpress                 = 525	         // ��ֵ���û�б��ʽ
		, my_Assi_UndefVarName              = 526	         // {0}δ����ı�����
		, my_Assi_UnValidExp                = 527	         // '{0}={1}' �Ⱥ����ߵ��������Ͳ�һ��
		, my_Assi_PostionErr                = 528	         // ��ֵ��� '{0}' ��λ�ò��Ϸ�
		//, my_CallStatmentErr                = 529	         // call��δ���ֵ��õĺ������(�Ѿ��ϳ�������)
		, my_CallStatGramErr                = 529	         // ������� '{0}' �����﷨����
		, my_CallPostionErr                 = 530	         // �������������ֵ�λ�ò��Ϸ�
		, my_Comm_ConditionErr              = 531	         // ��ǰ�������ʽ'{0}'û���óɶԵ�����������,������')'(������)֮����ֶ�����ַ�
		, my_NameSP_SPBackNoSpace           = 532	         // '{0 ' �����������ռ�ؼ����������ռ�������м�û�пո�Ӧ�ü��Ͽո�
		, my_Var_BackNoSpace                = 533	         // '{0}' �����Ǳ�������ؼ�����������м�û�пո�Ӧ�ü��Ͽո�
		, my_Func_FuncBackNoSpace           = 534	         // '{0}' �����Ǻ����ؼ����뺯�����������м�û�пո�Ӧ�ü��Ͽո�
		, my_Func_FuncTypeBackNoSpace       = 535	         // '{0}' �����Ǻ������������뺯�����м�û�пո�Ӧ�ü��Ͽո�
		, my_Ret_RetStatmentBackNoSpace     = 536	         // return�ؼ�����return֮�������м�û�пո�Ӧ�ü��Ͽո�
		, my_Func_NoReturnStatment          = 537	         // {0},����{1}����Ӧ����return����
		, my_Repeat_UndefCondition          = 538	         // �Ҳ���repeat���������

		, my_Express_ErroExp                = 539	         // '{0}' ��һ����������
	
		///////δ�ӵ���Ϣ////////////////


	    ,my_SelectPostion_Err               = 540            //select�������ڴ����λ��,Ӧ�ó����ں���,����while,repeat,if,select�������
		,my_Select_NoEnd                    = 541            //û���ҵ�select�Ľ�β���endselect
	    ,my_CaseDefaultPostion_Err          = 542           //{0}�������ڴ����λ��,Ӧ�ó�����select�������
	    ,my_CaseDefault_NoEnd               = 543            //{0}���û����ȷ�Ľ���

	    ,my_For_InitiaErr                   = 544            //for��� ��ʼ����䱨�� {0}
	    ,my_For_ExpErr                      = 545            //for��� ������䱨�� {0}
	    ,my_For_StepErr                     = 546            //for��� ������䱨�� {0}

		,my_Error_Lable                     = 547            //����ı�Ƕ����ʽ {0}
		,my_Error_Goto                      = 548            //�����Goto��䶨���ʽ {0}
		,my_GotoNoSpace                     = 549	         // '{0}' ������goto��ת�ؼ��������м�û�пո�Ӧ�ü��Ͽո�
		,my_LableRedefine                   = 550	         // '{0}' ���Ѿ�����ı��
		//my�﷨ɨ���쳣
		////////////////////////////////////////////////////////�Զ����쳣/////////////////////////////////////////


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

