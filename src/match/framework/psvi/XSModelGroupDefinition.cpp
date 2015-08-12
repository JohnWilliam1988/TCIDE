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
 * $Id: XSModelGroupDefinition.cpp 471747 2006-11-06 14:31:56Z amassari $
 */

#include <match/framework/psvi/XSModelGroupDefinition.hpp>
#include <match/framework/psvi/XSParticle.hpp>
#include <match/framework/psvi/XSModel.hpp>
#include <match/validators/schema/MatchGroupInfo.hpp>


MATCH_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSModelGroupDefinition: Constructors and Destructors
// ---------------------------------------------------------------------------
XSModelGroupDefinition::XSModelGroupDefinition(MatchGroupInfo* const groupInfo,
                                               XSParticle* const      groupParticle,
                                               XSAnnotation* const    annot,
                                               XSModel* const         xsModel,
                                               MemoryManager* const   manager)
    : XSObject(XSConstants::MODEL_GROUP_DEFINITION, xsModel, manager)
    , fGroupInfo(groupInfo)
    , fModelGroupParticle(groupParticle)
    , fAnnotation(annot)
{
}

XSModelGroupDefinition::~XSModelGroupDefinition()
{
    if (fModelGroupParticle) // Not owned by XSModel
        delete fModelGroupParticle;
}

// ---------------------------------------------------------------------------
//  XSModelGroupDefinition: XSModel virtual methods
// ---------------------------------------------------------------------------
const XMLCh *XSModelGroupDefinition::getName() const
{
    return fXSModel->getURIStringPool()->getValueForId(fGroupInfo->getNameId());
}

const XMLCh *XSModelGroupDefinition::getNamespace() 
{
    return fXSModel->getURIStringPool()->getValueForId(fGroupInfo->getNamespaceId());
}

XSNamespaceItem *XSModelGroupDefinition::getNamespaceItem() 
{
    return fXSModel->getNamespaceItem(getNamespace());
}

// ---------------------------------------------------------------------------
//  XSModelGroupDefinition: access methods
// ---------------------------------------------------------------------------
XSModelGroup* XSModelGroupDefinition::getModelGroup()
{
    if (fModelGroupParticle)
        return fModelGroupParticle->getModelGroupTerm();

    return 0;
}


MATCH_CPP_NAMESPACE_END

