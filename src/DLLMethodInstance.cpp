#include "StdAfx.h"

#include "DLLMethodInstance.h"
#include "Runtime.h"
#include "PBException.h"

DLLMethodInstance::DLLMethodInstance(Library *library, const string &methodName, MethodType* methodType)
	: m_library(library)
	, m_methodName(methodName)
	, m_methodAddr(0)
	, m_methodType(methodType)
{
	dprintf("DLLMethodInstance::DLLMethodInstance\n");
	getMethodAddr(); // throws if method does not exist 
}

DWORD DLLMethodInstance::getMethodAddr()
{
	if (m_methodAddr == 0) {
		m_methodAddr = m_library->getMethodAddr(m_methodName);
	}
	return m_methodAddr;
}

DLLMethodInstance::~DLLMethodInstance()
{
	dprintf("DLLMethodInstance::~DLLMethodInstance\n");
}

MethodType* DLLMethodInstance::getMethodType()
{
	return m_methodType;
}
