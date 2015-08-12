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
 * $Id: MatchAttGroupInfo.hpp 932887 2010-04-11 13:04:59Z borisk $
 */

#if !defined(MATCHC_INCLUDE_GUARD_MATCHATTGROUPINFO_HPP)
#define MATCHC_INCLUDE_GUARD_MATCHATTGROUPINFO_HPP


/**
  * The class act as a place holder to store attributeGroup information.
  *
  * The class is intended for internal use.
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <match/util/RefVectorOf.hpp>
#include <match/validators/schema/SchemaAttDef.hpp>

#include <match/internal/XSerializable.hpp>

MATCH_CPP_NAMESPACE_BEGIN

class VALIDATORS_EXPORT MatchAttGroupInfo : public XSerializable, public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Public Constructors/Destructor
    // -----------------------------------------------------------------------
    MatchAttGroupInfo
    (
        unsigned int           attGroupNameId
        , unsigned int         attGroupNamespaceId
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    ~MatchAttGroupInfo();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool                containsTypeWithId() const;
    XMLSize_t           attributeCount() const;
    XMLSize_t           anyAttributeCount() const;
    unsigned int        getNameId() const;
    unsigned int        getNamespaceId() const;
    SchemaAttDef*       attributeAt(const XMLSize_t index);
    const SchemaAttDef* attributeAt(const XMLSize_t index) const;
    SchemaAttDef*       anyAttributeAt(const XMLSize_t index);
    const SchemaAttDef* anyAttributeAt(const XMLSize_t index) const;
    SchemaAttDef*       getCompleteWildCard() const;
    const SchemaAttDef* getAttDef(const XMLCh* const baseName,
                                  const int uriId) const;

	// -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setTypeWithId(const bool other);
    void addAttDef(SchemaAttDef* const toAdd, const bool toClone = false);
    void addAnyAttDef(SchemaAttDef* const toAdd, const bool toClone = false);
    void setCompleteWildCard(SchemaAttDef* const toSet);

	// -----------------------------------------------------------------------
    //  Query methods
    // -----------------------------------------------------------------------
    bool containsAttribute(const XMLCh* const name, const unsigned int uri);

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(MatchAttGroupInfo)
    MatchAttGroupInfo(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MatchAttGroupInfo(const MatchAttGroupInfo& elemInfo);
    MatchAttGroupInfo& operator= (const MatchAttGroupInfo& other);

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool                       fTypeWithId;
    unsigned int               fNameId;
    unsigned int               fNamespaceId;
    RefVectorOf<SchemaAttDef>* fAttributes;
    RefVectorOf<SchemaAttDef>* fAnyAttributes;
    SchemaAttDef*              fCompleteWildCard;
    MemoryManager*             fMemoryManager;
};

// ---------------------------------------------------------------------------
//  MatchAttGroupInfo: Getter methods
// ---------------------------------------------------------------------------
inline bool MatchAttGroupInfo::containsTypeWithId() const {

    return fTypeWithId;
}

inline XMLSize_t MatchAttGroupInfo::attributeCount() const {

    if (fAttributes) {
        return fAttributes->size();
    }

    return 0;
}

inline XMLSize_t MatchAttGroupInfo::anyAttributeCount() const {

    if (fAnyAttributes) {
        return fAnyAttributes->size();
    }

    return 0;
}

inline unsigned int MatchAttGroupInfo::getNameId() const
{
    return fNameId;
}

inline unsigned int MatchAttGroupInfo::getNamespaceId() const
{
    return fNamespaceId;
}

inline SchemaAttDef*
MatchAttGroupInfo::attributeAt(const XMLSize_t index) {

    if (fAttributes) {
        return fAttributes->elementAt(index);
    }

    return 0;
}

inline const SchemaAttDef*
MatchAttGroupInfo::attributeAt(const XMLSize_t index) const {

    if (fAttributes) {
        return fAttributes->elementAt(index);
    }

    return 0;
}

inline SchemaAttDef*
MatchAttGroupInfo::anyAttributeAt(const XMLSize_t index) {

    if (fAnyAttributes) {
        return fAnyAttributes->elementAt(index);
    }

    return 0;
}

inline const SchemaAttDef*
MatchAttGroupInfo::anyAttributeAt(const XMLSize_t index) const {

    if (fAnyAttributes) {
        return fAnyAttributes->elementAt(index);
    }

    return 0;
}

inline SchemaAttDef*
MatchAttGroupInfo::getCompleteWildCard() const {

    return fCompleteWildCard;
}

// ---------------------------------------------------------------------------
//  MatchAttGroupInfo: Setter methods
// ---------------------------------------------------------------------------
inline void MatchAttGroupInfo::setTypeWithId(const bool other) {

    fTypeWithId = other;
}

inline void MatchAttGroupInfo::addAttDef(SchemaAttDef* const toAdd,
                                             const bool toClone) {

    if (!fAttributes) {
        fAttributes = new (fMemoryManager) RefVectorOf<SchemaAttDef>(4, true, fMemoryManager);
    }

    if (toClone) {
        SchemaAttDef* clonedAttDef = new (fMemoryManager) SchemaAttDef(toAdd);

        if (!clonedAttDef->getBaseAttDecl())
            clonedAttDef->setBaseAttDecl(toAdd);

        fAttributes->addElement(clonedAttDef);
    }
    else {
        fAttributes->addElement(toAdd);
    }
}

inline void MatchAttGroupInfo::addAnyAttDef(SchemaAttDef* const toAdd,
                                             const bool toClone) {

    if (!fAnyAttributes) {
        fAnyAttributes = new (fMemoryManager) RefVectorOf<SchemaAttDef>(2, true, fMemoryManager);
    }

    if (toClone) {
        SchemaAttDef* clonedAttDef = new (fMemoryManager) SchemaAttDef(toAdd);

        if (!clonedAttDef->getBaseAttDecl())
            clonedAttDef->setBaseAttDecl(toAdd);

        fAnyAttributes->addElement(clonedAttDef);
    }
    else {
        fAnyAttributes->addElement(toAdd);
    }
}

inline void
MatchAttGroupInfo::setCompleteWildCard(SchemaAttDef* const toSet) {

    if (fCompleteWildCard) {
        delete fCompleteWildCard;
    }

    fCompleteWildCard = toSet;
}

MATCH_CPP_NAMESPACE_END

#endif

/**
  * End of file MatchAttGroupInfo.hpp
  */

