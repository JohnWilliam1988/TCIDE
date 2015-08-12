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
 * $Id: MatchGroupInfo.hpp 932887 2010-04-11 13:04:59Z borisk $
 */

#if !defined(MATCHC_INCLUDE_GUARD_MATCHGROUPINFO_HPP)
#define MATCHC_INCLUDE_GUARD_MATCHGROUPINFO_HPP


/**
  * The class act as a place holder to store group information.
  *
  * The class is intended for internal use.
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <match/util/RefVectorOf.hpp>
#include <match/validators/schema/SchemaElementDecl.hpp>

#include <match/internal/XSerializable.hpp>

MATCH_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Forward Declarations
// ---------------------------------------------------------------------------
class ContentSpecNode;
class XSDLocator;


class VALIDATORS_EXPORT MatchGroupInfo : public XSerializable, public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Public Constructors/Destructor
    // -----------------------------------------------------------------------
    MatchGroupInfo
    (
        unsigned int groupNameId
        , unsigned int groupNamespaceId
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    ~MatchGroupInfo();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool                     getCheckElementConsistency() const;
    unsigned int             getScope() const;
    XMLSize_t                elementCount() const;
    ContentSpecNode*         getContentSpec() const;
    SchemaElementDecl*       elementAt(const XMLSize_t index);
    const SchemaElementDecl* elementAt(const XMLSize_t index) const;
    XSDLocator*              getLocator() const;
    MatchGroupInfo*         getBaseGroup() const;
    unsigned int             getNameId() const;
    unsigned int             getNamespaceId() const;

	// -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setScope(const unsigned int other);
    void setContentSpec(ContentSpecNode* const other);
    void addElement(SchemaElementDecl* const toAdd);
    void setLocator(XSDLocator* const aLocator);
    void setBaseGroup(MatchGroupInfo* const baseGroup);
    void setCheckElementConsistency(const bool aValue);

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(MatchGroupInfo)
    MatchGroupInfo(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MatchGroupInfo(const MatchGroupInfo& elemInfo);
    MatchGroupInfo& operator= (const MatchGroupInfo& other);

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool                            fCheckElementConsistency;
    unsigned int                    fScope;
    unsigned int                    fNameId;
    unsigned int                    fNamespaceId;
    ContentSpecNode*                fContentSpec;
    RefVectorOf<SchemaElementDecl>* fElements;
    MatchGroupInfo*                fBaseGroup; // redefine by restriction
    XSDLocator*                     fLocator;
};

// ---------------------------------------------------------------------------
//  MatchGroupInfo: Getter methods
// ---------------------------------------------------------------------------
inline unsigned int MatchGroupInfo::getScope() const {

    return fScope;
}

inline XMLSize_t MatchGroupInfo::elementCount() const {

    return fElements->size();
}

inline ContentSpecNode* MatchGroupInfo::getContentSpec() const {

    return fContentSpec;
}

inline SchemaElementDecl*
MatchGroupInfo::elementAt(const XMLSize_t index) {

    return fElements->elementAt(index);
}

inline const SchemaElementDecl*
MatchGroupInfo::elementAt(const XMLSize_t index) const {

    return fElements->elementAt(index);
}

inline XSDLocator* MatchGroupInfo::getLocator() const {

    return fLocator;
}

inline MatchGroupInfo* MatchGroupInfo::getBaseGroup() const {

    return fBaseGroup;
}

inline bool MatchGroupInfo::getCheckElementConsistency() const {

    return fCheckElementConsistency;
}

inline unsigned int MatchGroupInfo::getNameId() const
{
    return fNameId;
}

inline unsigned int MatchGroupInfo::getNamespaceId() const
{
    return fNamespaceId;
}

// ---------------------------------------------------------------------------
//  MatchGroupInfo: Setter methods
// ---------------------------------------------------------------------------}
inline void MatchGroupInfo::setScope(const unsigned int other) {

    fScope = other;
}

inline void MatchGroupInfo::setContentSpec(ContentSpecNode* const other) {

    fContentSpec = other;
}

inline void MatchGroupInfo::addElement(SchemaElementDecl* const elem) {

    if (!fElements->containsElement(elem))
        fElements->addElement(elem);
}

inline void MatchGroupInfo::setBaseGroup(MatchGroupInfo* const baseGroup) {

    fBaseGroup = baseGroup;
}

inline void MatchGroupInfo::setCheckElementConsistency(const bool aValue) {

    fCheckElementConsistency = aValue;
}

MATCH_CPP_NAMESPACE_END

#endif

/**
  * End of file MatchGroupInfo.hpp
  */

