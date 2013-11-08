#pragma once

#include "../Runtime.h"
#include "../types/StructType.h"

class ParamStorage;

//TODO remove this class, value objects' func-ty is implemented in ExecutionContext
class Structure : public ParamStorage  
{
public:
	Structure(StructType *structType);
	virtual ~Structure();

};

