//example
//
// IMyTable.h
//
#include "CppInterfaces.h"

#include <string>
#include <occi.h>

DeclareInterface(IMyTable)
   virtual int AddCol() = 0;
EndInterface