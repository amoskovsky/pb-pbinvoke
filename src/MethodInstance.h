
#pragma once

#include "types/MethodType.h"

class Method;
class MethodInstance
{
public:
	virtual ~MethodInstance() {}
	virtual DWORD getMethodAddr() = 0;
	virtual MethodType* getMethodType() = 0;

	//virtual void invoke(Method *methodContext);
};
