/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: DOM.hpp 527149 2007-04-10 14:56:39Z amassari $
 */

#if !defined(MATCHC_INCLUDE_GUARD_DOM_HPP)
#define MATCHC_INCLUDE_GUARD_DOM_HPP

//
//  This is the primary header file for inclusion in application
//  programs using the C++ XML Document Object Model API.
//

#include <match/dom/DOMAttr.hpp>
#include <match/dom/DOMCDATASection.hpp>
#include <match/dom/DOMCharacterData.hpp>
#include <match/dom/DOMComment.hpp>
#include <match/dom/DOMDocument.hpp>
#include <match/dom/DOMDocumentFragment.hpp>
#include <match/dom/DOMDocumentType.hpp>
#include <match/dom/DOMElement.hpp>
#include <match/dom/DOMEntity.hpp>
#include <match/dom/DOMEntityReference.hpp>
#include <match/dom/DOMException.hpp>
#include <match/dom/DOMImplementation.hpp>
#include <match/dom/DOMNamedNodeMap.hpp>
#include <match/dom/DOMNode.hpp>
#include <match/dom/DOMNodeList.hpp>
#include <match/dom/DOMNotation.hpp>
#include <match/dom/DOMProcessingInstruction.hpp>
#include <match/dom/DOMText.hpp>

// Introduced in DOM Level 2
#include <match/dom/DOMDocumentRange.hpp>
#include <match/dom/DOMDocumentTraversal.hpp>
#include <match/dom/DOMNodeFilter.hpp>
#include <match/dom/DOMNodeIterator.hpp>
#include <match/dom/DOMRange.hpp>
#include <match/dom/DOMRangeException.hpp>
#include <match/dom/DOMTreeWalker.hpp>

// Introduced in DOM Level 3
#include <match/dom/DOMLSParser.hpp>
#include <match/dom/DOMLSParserFilter.hpp>
#include <match/dom/DOMConfiguration.hpp>
#include <match/dom/DOMLSResourceResolver.hpp>
#include <match/dom/DOMError.hpp>
#include <match/dom/DOMErrorHandler.hpp>
#include <match/dom/DOMImplementationLS.hpp>
#include <match/dom/DOMImplementationList.hpp>
#include <match/dom/DOMImplementationRegistry.hpp>
#include <match/dom/DOMImplementationSource.hpp>
#include <match/dom/DOMLSInput.hpp>
#include <match/dom/DOMLSOutput.hpp>
#include <match/dom/DOMLocator.hpp>
#include <match/dom/DOMPSVITypeInfo.hpp>
#include <match/dom/DOMStringList.hpp>
#include <match/dom/DOMTypeInfo.hpp>
#include <match/dom/DOMUserDataHandler.hpp>
#include <match/dom/DOMLSSerializer.hpp>
#include <match/dom/DOMLSSerializerFilter.hpp>

#include <match/dom/DOMXPathEvaluator.hpp>
#include <match/dom/DOMXPathNSResolver.hpp>
#include <match/dom/DOMXPathException.hpp>
#include <match/dom/DOMXPathExpression.hpp>
#include <match/dom/DOMXPathResult.hpp>
#include <match/dom/DOMXPathNamespace.hpp>


#endif
