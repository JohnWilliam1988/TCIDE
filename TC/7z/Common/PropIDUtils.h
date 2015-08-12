// PropIDUtils.h

#ifndef __PROPID_UTILS_H
#define __PROPID_UTILS_H

#include "MyString.h"
#include "Types.h"

void ConvertUInt32ToHex(UInt32 value, wchar_t *s);
UString ConvertPropertyToString(const PROPVARIANT &propVariant, PROPID propID, bool full = true);

#endif
