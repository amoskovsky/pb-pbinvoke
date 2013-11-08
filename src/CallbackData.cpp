
#include "DynCallback.h"

using namespace std;

CallbackData::CallbackData(int argc, bool stdCall)
{
	m_argc = argc;
	char *fnBuf = (char *)&thunk_templ;
	//printf("fnBuf=%p\n", fnBuf);
	if (fnBuf[0] == '\xE9') {
		// debug thunk - need to follow jumps to find real fn body
		int offset = *(int*)&fnBuf[1];
		//printf("jmp %X\n", offset);
		fnBuf += offset + 5 /* 1 + 4 bytes for jmp OFFSET */;
	}
	char *fnBufEnd = (char *)memchr(fnBuf, '\xC2' /*RET xxx*/, 2048 /*max template size */); 
	if (fnBufEnd == NULL) {
		throw InvalidCallbackTemplateException("end of function not found");
	}
	size_t fnSize = fnBufEnd - fnBuf + 3;
	//printf("fnSize=%i\n", fnSize);
	
	m_thunk.resize(fnSize);
	memcpy(&m_thunk[0], fnBuf, m_thunk.size());

	
	//dump();

	// correct RET xxx (last 3 bytes of fn body, C2 04 00)
	if (m_argc == 0 && !stdCall /* cdecl, caller must clear stack*/) {
		m_thunk[m_thunk.size() - 3] = (BYTE)0xC3; // RET 0
	}
	else {
		*((unsigned short*)&m_thunk[m_thunk.size() - 2]) = (unsigned short)m_argc * 4; // RET XXX,  XXX > 0
	}
	
	// correct 'this' reference
	adjustConstant(THIS_MARKER, (DWORD)this);
	
	// correct real callback reference
	adjustConstant(CALLBACK_MARKER, (DWORD)&staticCallback);

	DWORD oldProtect;
	BOOL ret = VirtualProtect(&m_thunk[0], m_thunk.size(), PAGE_EXECUTE_READWRITE, &oldProtect);

	//dump();

}

CallbackData::~CallbackData()
{

}

int CALLBACK CallbackData::thunk_templ(int dummy) 
{
	DWORD *argsPtr = (DWORD *) &dummy;
	int (CALLBACK *ptr)(DWORD, DWORD *) = (int (CALLBACK*)(DWORD, DWORD *)) CALLBACK_MARKER;
	return ptr(THIS_MARKER, argsPtr);
}
	
DWORD CallbackData::getCallbackAddr() 
{
	return (DWORD)&m_thunk[0];
}

int CallbackData::runCallback() 
{
	int (CALLBACK *ptr)() = (int (CALLBACK *)()) getCallbackAddr();
	int save1, save2;
	__asm {
		mov save1, esp;
	}
	int res = ptr();
	__asm {
		mov save2, esp;
	}
	printf("save1=%p, save2=%p\n", save1, save2);
	return res;
}
int CallbackData::runCallback(int arg1) 
{
	int (CALLBACK *ptr)(int) = (int (CALLBACK *)(int)) getCallbackAddr();
	int save1, save2;
	__asm {
		mov save1, esp;
	}
	int res = ptr(arg1);
	__asm {
		mov save2, esp;
	}
	printf("save1=%p, save2=%p\n", save1, save2);
	return res;
}

int CallbackData::runCallback(int arg1, int arg2) 
{
	int (CALLBACK *ptr)(int, int) = (int (CALLBACK *)(int, int)) getCallbackAddr();
	int save1, save2;
	__asm {
		mov save1, esp;
	}
	int res = ptr(arg1, arg2);
	__asm {
		mov save2, esp;
	}
	printf("save1=%p, save2=%p\n", save1, save2);
	return res;
}



void CallbackData::dump() 
{
	for (int i = 0; i < m_thunk.size(); i ++) {
		printf("%02X ", (BYTE)m_thunk[i]);
	}
	printf("\n");
}

std::string CallbackData::dumpThunk()
{
	vector<char> buf;
	buf.resize(m_thunk.size() * 3 );
	for (int i = 0; i < m_thunk.size(); i ++) {
		sprintf(&buf[i * 3], "%02X ", (BYTE)m_thunk[i]);
	}
	buf.push_back('\0');
	return &buf[0];
}


void CallbackData::adjustConstant(DWORD marker, DWORD newValue) 
{
	for (int i = 0; i < m_thunk.size() - 4; i ++) {
		if (*(DWORD*)&m_thunk[i] == marker) {
			*(DWORD*)&m_thunk[i] = newValue;
			return;
		}
	}
	throw InvalidCallbackTemplateException("marker not found");
}

int CALLBACK CallbackData::staticCallback(CallbackData * thisPtr, DWORD *argsPtr) 
{
	thisPtr->m_argsPtr = argsPtr;
	return thisPtr->callback();
}

int CALLBACK CallbackData::callback() 
{
	return 1;
}


typedef DWORD (*DWORDMethod)();
DWORD runDynamicMethod(DWORD addr, DWORD *argv, int argc, bool isStdCall) {
	DWORDMethod method = (DWORDMethod)addr;
	for (int i = argc - 1; i >= 0; i --) {
		DWORD arg = argv[i];
		__asm push arg;
	}
	DWORD ret = method();
	if (!isStdCall) {
		int argsSize = argc * sizeof(DWORD);
		__asm  add esp, argsSize;
	}
	return ret;
}

typedef double (*DoubleMethod)(DWORD, DWORD *, int, bool);
double runDynamicDoubleMethod(DWORD addr, DWORD *argv, int argc, bool isStdCall) {
	DoubleMethod method = (DoubleMethod) &runDynamicMethod;
	return method(addr, argv, argc, isStdCall);
}


typedef float (*FloatMethod)(DWORD, DWORD *, int, bool);
float runDynamicFloatMethod(DWORD addr, DWORD *argv, int argc, bool isStdCall) {
	FloatMethod method = (FloatMethod) &runDynamicMethod;
	return method(addr, argv, argc, isStdCall);
}

typedef __int64 (*Int64Method)(DWORD, DWORD *, int, bool);
__int64 runDynamicInt64Method(DWORD addr, DWORD *argv, int argc, bool isStdCall) {
	Int64Method method = (Int64Method) &runDynamicMethod;
	return method(addr, argv, argc, isStdCall);
}