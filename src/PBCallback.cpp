#include "stdafx.h"
#include "PBCallback.h"
#include "Runtime.h"
#include "Core.h"
#include "PBException.h"

PBCallback::PBCallback(UINT callbackId, Method *defaultMethodContext)
	: CallbackData(defaultMethodContext->getArgStackSize() / sizeof(DWORD), defaultMethodContext->getMethodType()->isStdCall())
	, m_callbackId(callbackId)
	, m_methodType(defaultMethodContext->getMethodType())
	, m_inCall(0)
	, m_currentContext(NULL)
	, m_hookId(-1)
	, m_hhk(NULL)
	, m_enable(true)
{
	dprintf("PBCallback::PBCallback\n");

}

PBCallback::~PBCallback()
{
	dprintf("PBCallback::~PBCallback\n");
}

void PBCallback::createCallbackContext(BYTE *argsPtr) 
{
	m_inCall ++;
	if (m_inCall > 1 || m_contexts.empty()) {
		m_contexts.push(MethodContextPtr(new Method(m_methodType, argsPtr)));
	}
	else {
		m_contexts.top()->setArgsPtr(argsPtr);
	}
	m_currentContext = m_contexts.top().get();

}
void PBCallback::destroyCallbackContext() 
{
	if (m_inCall > 1) {
		m_contexts.pop();
		m_currentContext = m_contexts.top().get();
	}
	m_inCall --;
}

void PBCallback::setHookFilter(HHOOK hhk, int hookId, UINT messageFilter[], size_t messageFilterSize)
{
	dprintf("PBCallback::setHookFilter\n");
	vector<WinHookHelper>& helpers = CoreInstance.getWinHookHelpers();
	if (hookId < 0 || hookId >= (int)helpers.size() || helpers[hookId].get() == NULL) {
		m_enable = false;
		throw RuntimeException("Message filter for this type of hooks is not implemented yet");
	}
	m_hookId = hookId;
	m_hhk = hhk;
	m_messageFilter.insert(m_messageFilter.end(), &messageFilter[0], &messageFilter[messageFilterSize]);
	m_enable = true;
}
BOOL PBCALL cb_setHookFilter(PBCallback *callback, HHOOK hhk, int hookId, UINT messageFilter[], size_t messageFilterSize)
{
	try {
		callback->setHookFilter(hhk, hookId, messageFilter, messageFilterSize);
		return true;
	}
	catch (...) {
		return false;
	}
}

//this method must NOT throw any exception
int CALLBACK PBCallback::callback() 
{
	LRESULT result;
	if (!m_enable)
		return 0;
	if (m_hookId >= 0 && m_hhk != NULL) {
		vector<WinHookHelper>& helpers = CoreInstance.getWinHookHelpers();
		if (m_hookId < (int)helpers.size() && helpers[m_hookId].get() != NULL) {
			if (helpers[m_hookId]->skip(m_hhk, (PHookProcArgs)m_argsPtr, m_messageFilter, result))
				return result;
		}
	}

	dprintf("+PBCallback::callback\n");
	static int counter = 0;
	counter ++;
	int callbackRetVal = 0; 
	Method *methodContext = NULL;
	try {
		HWND messageHandler = CoreInstance.getCallbackMessageHandler();
		if (messageHandler == NULL)
			throw RuntimeException("Callback message handler is not specified");

		createCallbackContext((BYTE*)m_argsPtr);
		dprintf("context=%p, m_argsPtr=%p\n", m_currentContext, m_argsPtr);

		dprintf("SendMessage incall=%i, counter=%i,  messageHandler=%i, messageId=%i, m_callbackId=%i\n", 
				m_inCall, counter, messageHandler, CoreInstance.getCallbackMessageId(), m_callbackId);
		result = SendMessage(messageHandler, CoreInstance.getCallbackMessageId(), m_callbackId, (LPARAM)m_currentContext); 
		dprintf("SendMessage result=%i\n", result);
		if (result == 1) {
			// executed OK
			callbackRetVal = *((int*)m_currentContext->getReturnValueAddr());
		}
		else {
			// callback id or message handler was not found 
			callbackRetVal = 0;
		}
		destroyCallbackContext();
	}
	catch (...) {
		destroyCallbackContext();
		dprintf("-PBCallback::callback with exception\n");
	}
	dprintf("-PBCallback::callback\n");
	return callbackRetVal;		
}

PBCallback* PBCALL cb_create(UINT callbackId, Method* methodContext) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		return new PBCallback(callbackId, methodContext);
	}
	catch (...) {
		return NULL;
	}
}

void PBCALL cb_destroy(PBCallback* instance) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	if (instance != NULL)
		delete instance;
}

DWORD PBCALL cb_getAddr(PBCallback* instance) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	return instance->getCallbackAddr();
}

DWORD PBCallback::getMethodAddr()
{
	return getCallbackAddr();
}
