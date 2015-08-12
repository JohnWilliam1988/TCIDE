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
 * $Id: DOMImplementationListImpl.hpp 671894 2008-06-26 13:29:21Z borisk $
 */

#if !defined(MATCHC_INCLUDE_GUARD_DOMIMPLEMENTATIONLISTIMPL_HPP)
#define MATCHC_INCLUDE_GUARD_DOMIMPLEMENTATIONLISTIMPL_HPP

#include <match/util/MatchDefs.hpp>
#include <match/util/RefVectorOf.hpp>
#include <match/dom/DOMImplementationList.hpp>

MATCH_CPP_NAMESPACE_BEGIN


class DOMImplementation;

class CDOM_EXPORT DOMImplementationListImpl: public DOMImplementationList
{
protected:
    RefVectorOf<DOMImplementation>   *fList;

private:
    // Unused, and unimplemented constructors, operators, etc.
    DOMImplementationListImpl(const DOMImplementationListImpl & other);
    DOMImplementationListImpl & operator = (const DOMImplementationListImpl & other);

public:
    DOMImplementationListImpl();
    void add(DOMImplementation* impl);

    virtual             ~DOMImplementationListImpl();
    virtual DOMImplementation *  item(XMLSize_t index) const;
    virtual XMLSize_t            getLength() const;
    virtual void                 release();
};

MATCH_CPP_NAMESPACE_END

#endif