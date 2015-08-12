// TempFiles.h

#ifndef __TEMPFILES_H
#define __TEMPFILES_H

#include "MyString.h"

class CTempFiles
{
  void Clear();
public:
  UStringVector Paths;
  ~CTempFiles() { Clear(); }
};

#endif
