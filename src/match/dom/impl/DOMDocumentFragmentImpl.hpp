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
 * $Id: DOMDocumentFragmentImpl.hpp 641193 2008-03-26 08:06:57Z borisk $
 */

#if !defined(MATCHC_INCLUDE_GUARD_DOMDOCUMENTFRAGMENTIMPL_HPP)
#define MATCHC_INCLUDE_GUARD_DOMDOCUMENTFRAGMENTIMPL_HPP

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <match/dom/DOM.hpp> for the entire
//  DOM API, or matchc/dom/DOM*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

#include <match/util/MatchDefs.hpp>
#include <match/dom/DOMDocumentFragment.hpp>
#include "DOMParentNode.hpp"
#include "DOMNodeImpl.hpp"

MATCH_CPP_NAMESPACE_BEGIN


class CDOM_EXPORT DOMDocumentFragmentImpl: public DOMDocumentFragment {
protected:
    DOMNodeImpl     fNode;
    DOMParentNode   fParent;

protected:
    DOMDocumentFragmentImpl(DOMDocument *);
    DOMDocumentFragmentImpl(const DOMDocumentFragmentImpl &other, bool deep);
    friend class DOMDocumentImpl;

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMDocumentFragmentImpl & operator = (const DOMDocumentFragmentImpl &);

public:
    virtual ~DOMDocumentFragmentImpl();

public:
    // Declare all of the functions from DOMNode.
    DOMNODE_FUNCTIONS;
};

MATCH_CPP_NAMESPACE_END

#endif

