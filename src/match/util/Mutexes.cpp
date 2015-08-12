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
 * $Id: Mutexes.cpp 734985 2009-01-16 12:21:14Z johns $
 */



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <match/util/MatchDefs.hpp>
#include <match/util/PlatformUtils.hpp>
#include <match/util/Mutexes.hpp>

MATCH_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLMutex: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLMutex::XMLMutex(MemoryManager* const manager) :

    fHandle(0),
    fManager(manager)
{
    // Ask the per-platform driver to make us a mutex
    fHandle = XMLPlatformUtils::makeMutex(manager);
}


XMLMutex::~XMLMutex()
{
    if (fHandle)
    {
        XMLPlatformUtils::closeMutex(fHandle, fManager);
        fHandle = 0;
    }
}


// ---------------------------------------------------------------------------
//  XMLMutex: Lock control methods
// ---------------------------------------------------------------------------
void XMLMutex::lock()
{
    XMLPlatformUtils::lockMutex(fHandle);
}

void XMLMutex::unlock()
{
    XMLPlatformUtils::unlockMutex(fHandle);
}



// ---------------------------------------------------------------------------
//  XMLMutexLock: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLMutexLock::XMLMutexLock(XMLMutex* const toLock) :

    fToLock(toLock)
{
    fToLock->lock();
}


XMLMutexLock::~XMLMutexLock()
{
    fToLock->unlock();
}

MATCH_CPP_NAMESPACE_END