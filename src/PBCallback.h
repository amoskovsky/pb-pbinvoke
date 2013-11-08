#pragma once

#include "DynCallback.h"
#include "MethodInstance.h"
#include "Runtime.h"

#include <stack>



class PBCallback : public MethodInstance, public CallbackData 
{
protected:
	MethodType *m_methodType;
	UINT m_callbackId;
	int m_inCall;
	Method *m_currentContext;
	stack<MethodContextPtr> m_contexts;
	vector<UINT> m_messageFilter;
	HHOOK m_hhk;
	int m_hookId;
	bool m_enable;
public:
	PBCallback(UINT callbackId, Method *defaultMethodContext);
	virtual ~PBCallback();
	void createCallbackContext(BYTE *argsPtr);
	void destroyCallbackContext();

	void setHookFilter(HHOOK hhk, int hookId, UINT messageFilter[], size_t messageFilterSize);
	
	// iface CallbackData
	virtual int CALLBACK callback();

	// iface MethodInstance
	virtual DWORD getMethodAddr();
	virtual MethodType* getMethodType() { return m_methodType; } 

};

