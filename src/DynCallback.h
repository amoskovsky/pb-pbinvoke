#pragma once

#include <vector>
#include <string>
#include <wtypes.h>


#define DYNCALLBACKVERSION "1.4.0"
/*
history
  
  1.4.0  
    - added method for calling __int64 functions

  1.3.0  
	- added method for calling float functions

  1.2.0  
	- added methods for calling functions dynamically

  1.1.0  
	- added calling convention arg (stdCall=true) to ctor

  1.0.1  
	- disabled optimization
	- added dumpThunk debug method

  1.0.0  
	- initial version
*/


class InvalidCallbackTemplateException {
public:
	std::string msg;
	InvalidCallbackTemplateException(const std::string &msg) {
		this->msg = msg;
	}

};

struct Callback {
	virtual int CALLBACK callback() = 0;
	virtual ~Callback() {}
};

class CallbackData : public Callback 
{
private:
	enum { 
		THIS_MARKER     = 0x45454545, 
		CALLBACK_MARKER = 0x23232323, 
	};
	std::vector<BYTE> m_thunk;
	static int CALLBACK thunk_templ(int dummy);
	void dump();
	void adjustConstant(DWORD marker, DWORD newValue);
	static int CALLBACK staticCallback(CallbackData * thisPtr, DWORD *argsPtr);
protected:
	int m_argc;
	DWORD *m_argsPtr;
public:
	virtual ~CallbackData();
	CallbackData(int argc, bool stdCall = true);
	DWORD getCallbackAddr();

	int runCallback();
	int runCallback(int arg1);
	int runCallback(int arg1, int arg2);

	std::string dumpThunk();

	// interface Callback
	virtual int CALLBACK callback();
};


DWORD runDynamicMethod(DWORD addr, DWORD *argv, int argc, bool isStdCall = true);

double runDynamicDoubleMethod(DWORD addr, DWORD *argv, int argc, bool isStdCall = true);
float runDynamicFloatMethod(DWORD addr, DWORD *argv, int argc, bool isStdCall = true);
__int64 runDynamicInt64Method(DWORD addr, DWORD *argv, int argc, bool isStdCall = true);
