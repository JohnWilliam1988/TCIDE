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
 * $Id: MatchGroupInfo.cpp 471747 2006-11-06 14:31:56Z amassari $
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <match/validators/schema/MatchGroupInfo.hpp>
#include <match/validators/common/ContentSpecNode.hpp>
#include <match/validators/schema/XSDLocator.hpp>

#include <match/internal/XTemplateSerializer.hpp>

MATCH_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  MatchGroupInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
MatchGroupInfo::MatchGroupInfo(MemoryManager* const manager)
    : fCheckElementConsistency(true)
    , fScope(Grammar::TOP_LEVEL_SCOPE)
    , fNameId(0)
    , fNamespaceId(0)
    , fContentSpec(0)
    , fElements(0)
    , fBaseGroup(0)
    , fLocator(0)
{
    fElements = new (manager) RefVectorOf<SchemaElementDecl>(4, false, manager);
}

MatchGroupInfo::MatchGroupInfo(unsigned int groupNameId,
                                 unsigned int groupNamespaceId,
                                 MemoryManager* const manager)
    : fCheckElementConsistency(true)
    , fScope(Grammar::TOP_LEVEL_SCOPE)
    , fNameId(groupNameId)
    , fNamespaceId(groupNamespaceId)
    , fContentSpec(0)
    , fElements(0)
    , fBaseGroup(0)
    , fLocator(0)
{
    fElements = new (manager) RefVectorOf<SchemaElementDecl>(4, false, manager);
}


MatchGroupInfo::~MatchGroupInfo()
{
    delete fElements;
    delete fContentSpec;
    delete fLocator;
}


// ---------------------------------------------------------------------------
//  MatchGroupInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
void MatchGroupInfo::setLocator(XSDLocator* const aLocator) {

    if (fLocator)
        delete fLocator;

    fLocator = aLocator;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(MatchGroupInfo)

void MatchGroupInfo::serialize(XSerializeEngine& serEng)
{
    if (serEng.isStoring())
    {   
        serEng<<fCheckElementConsistency;
        serEng<<fScope;
        serEng<<fNameId;
        serEng<<fNamespaceId;
        serEng<<fContentSpec;

        /***
         *
         * Serialize RefVectorOf<SchemaElementDecl>* fElements;
         *
         ***/

        XTemplateSerializer::storeObject(fElements, serEng);

        serEng<<fBaseGroup;

        //don't serialize     XSDLocator* fLocator;
    }
    else
    {
        serEng>>fCheckElementConsistency;
        serEng>>fScope;
        serEng>>fNameId;
        serEng>>fNamespaceId;
        serEng>>fContentSpec;

        /***
         * 
         * Deserialize RefVectorOf<SchemaElementDecl>*    fElements;
         *
         ***/
        XTemplateSerializer::loadObject(&fElements, 4, false, serEng);

        serEng>>fBaseGroup;

        //don't serialize     XSDLocator* fLocator;
        fLocator = 0;
    }

}

MATCH_CPP_NAMESPACE_END

/**
  * End of file MatchGroupInfo.cpp
  */


