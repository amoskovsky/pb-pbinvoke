#pragma once
#include "MethodInstance.h"

class Library;

class DLLMethodInstance: public MethodInstance
{
private:
	Library *m_library;
	string m_methodName;
	DWORD m_methodAddr;
	MethodType* m_methodType;
public:
	DLLMethodInstance(Library *library, const string &methodName, MethodType* methodType);
	virtual ~DLLMethodInstance();
	
	// iface MethodInstance
	virtual DWORD getMethodAddr();
	virtual MethodType* getMethodType();
};
