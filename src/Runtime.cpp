#include "stdafx.h"
#include "Runtime.h"
#include "Core.h"
#include "types/StructType.h"

#include "DLLMethodInstance.h"
#include "DynCallback.h"
#include "Compiler.h"

#include "PBException.h"


Method::Method(MethodType *methodType, BYTE* argsPtr) 
	: ParamStorage(methodType->getParams(), argsPtr)
	, m_methodType(methodType)
{
	int retSize = methodType->getReturnType()->getSizeOf();
	if (retSize < sizeof(DWORD))
		retSize = sizeof(DWORD);
	m_returnValue.resize(retSize);
	dprintf("m_returnValue.resize=%i\n", retSize);
}

Method::~Method()
{

}


//ParamStorage::ParamStorage(vector<Type> &paramTypes, BYTE* argsPtr)
//	: m_storageType(new StructType(paramTypes, 0/*pack*/, 0/*explicitAlign*/, false/*isUnion*/, true/*isArgStack*/))
//	, m_paramDataPtr(argsPtr)
//{
//	init();
//}

ParamStorage::ParamStorage(vector<NamedType> &params, BYTE* argsPtr)
	: m_storageType(new StructType(params, 0/*pack*/, 0/*explicitAlign*/, false/*isUnion*/, true/*isArgStack*/))
	, m_paramDataPtr(argsPtr)
{
	init();
}

ParamStorage::~ParamStorage()
{
}

ExecutionContext::~ExecutionContext()
{
	clearBuffers();
}

ExecutionContext * PBCALL ctx_create() 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		return new ExecutionContext();
	}
	catch (...) {
		return NULL;
	}
}

void PBCALL ctx_destroy(ExecutionContext *context) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	if (context != NULL)
		delete context;
}

BYTE * ExecutionContext::allocateBuffer(int size, int align)
{
	dprintf("+ExecutionContext::allocateBuffer size=%i, align=%i\n", size, align);
	BYTE *buf = new BYTE[size + align - 1];
	m_buffers.push_back(buf);
	m_bufferSizes.push_back(size + align - 1);
	DWORD alignedBuf = (DWORD)buf;
	alignedBuf = align * (int)((alignedBuf + align - 1) / align);
	dprintf(" allocated addr=%p aligned addr=%p\n", buf, alignedBuf);
	dprintf("-ExecutionContext::allocateBuffer\n");
	return (BYTE*)alignedBuf;
}

void ParamStorage::init()
{
	dprintf("+ParamStorage::init\n");
	if (m_paramDataPtr == NULL) {
		m_paramDataBuf.clear();
		dprintf("m_storageType->getSizeOf()=%i\n", m_storageType->getSizeOf());
		m_paramDataBuf.resize(m_storageType->getSizeOf() + 4 /* for next line no fail if (void)*/, 0); // fill param data with 0
		m_paramDataPtr = &m_paramDataBuf[0]; 
	}
	dprintf("-ParamStorage::init getTypeSignature=%s\n", m_storageType->getTypeSignature().c_str());
}



void Method::invoke(MethodInstance *methodInstance)
{
	dprintf("+Method::invoke m_storageType->getSizeOf()=%i\n", m_storageType->getSizeOf());

	m_methodType->getReturnType()->executeMethod(
		this,
		methodInstance->getMethodAddr(),
		&m_returnValue[0],
		m_paramDataPtr,
		m_storageType->getSizeOf(),
		m_methodType->isStdCall()
	);

	dprintf("-Method::invoke\n");
}

BOOL PBCALL me_invoke(Method *methodContext, MethodInstance *methodInstance) 
{
	dprintf("+me_invoke methodContext=%p, methodInstance=%p\n", methodContext, methodInstance);
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		methodContext->invoke(methodInstance);
		dprintf("-me_invoke\n");
		return true;
	}
	catch (...) {
		dprintf("-me_invoke Exception\n");
		return false;
	}
}

DWORD Library::getMethodAddr(const string &methodName, bool enableTCharSuffix)
{
	ensureIsLoaded();
	FARPROC addr = GetProcAddress(m_libraryHandle, methodName.c_str());
	if (enableTCharSuffix && addr == NULL && methodName.size() > 0) {
		string suffix = CoreInstance.isUnicode() ? "W" : "A";
		string methodNameS;
		if (methodName[methodName.size() - 1] == suffix[0]) {
			methodNameS = methodName.substr(0, methodName.size() - 1);
		}
		else {
			methodNameS = methodName + suffix;
		}
		addr = GetProcAddress(m_libraryHandle, methodNameS.c_str());
	}
	if (addr == NULL)
		throw RuntimeException(this, "Method " + methodName + " could not be found in library " + m_libraryName);
	return (DWORD)addr;
}

Type& Library::getMethodType(const string &methodName)
{
	Type& type = CoreInstance.findName(m_nameSpace, methodName);
	return type;
}

BOOL PBCALL ctx_clear(ExecutionContext *context) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		if (context != NULL)
			context->clear();
		return true;
	}
	catch (...) {
		return false;
	}
}



wchar_t* ExecutionContext::convertAtoW(const char *srcString, int strLen, wchar_t *buf)
{
	dprintf("+ParamStorage::convertAtoW srcString=%s, strLen=%i, buf=%p\n", srcString, strLen, buf);
	if (srcString == NULL)
		return NULL;
	if (strLen < 0)
		strLen = strlen(srcString);

	int newLen = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		srcString,
		strLen + 1 , // incl 0
		NULL, 
		0 // return buf size in wchar's
	);
	dprintf(" MultiByteToWideChar(1) ret=%i\n", newLen);
	wchar_t* tgtString = buf == NULL
		? (wchar_t*) allocateBuffer(newLen * sizeof(wchar_t), __alignof(wchar_t))
		: buf;
	dprintf(" tgtString=%p, newLen=%i\n", tgtString, newLen);
	int ret = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		srcString,
		strLen + 1, // incl 0
		tgtString, 
		newLen
	);
	dprintf(" MultiByteToWideChar(2) ret=%i, tgtString=%S\n", ret, tgtString);
	dprintf("-ParamStorage::convertAtoW\n");
	return tgtString;
}
int ExecutionContext::strlenAtoW(const char *srcString, int strLen)
{
	if (srcString == NULL)
		return 0;
	if (strLen < 0)
		strLen = strlen(srcString);

	int newLen = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		srcString,
		strLen , // not incl 0
		NULL, 
		0 // return buf size in wchar's
	);
	return newLen;
}

char* ExecutionContext::convertWtoA(const wchar_t *srcString, int strLen, char *buf)
{
	if (srcString == NULL)
		return NULL;
	if (strLen < 0)
		strLen = wcslen(srcString);

	int newLen = WideCharToMultiByte(
		CP_ACP,
		0,
		srcString,
		strLen + 1 , // incl 0
		NULL, 
		0, // return buf size in char's
		NULL, // def char
		NULL  // &lb_UsedDefChar

	);
	char* tgtString = buf == NULL
		? (char*) allocateBuffer(newLen * sizeof(char), __alignof(char))
		: buf;
	int ret = WideCharToMultiByte(
		CP_ACP,
		0,
		srcString,
		strLen + 1, // incl 0
		tgtString, 
		newLen,
		NULL, // def char
		NULL  // &lb_UsedDefChar
	);
	return tgtString;
}

int ExecutionContext::strlenWtoA(const wchar_t *srcString, int strLen)
{
	dprintf("+ParamStorage::strlenWtoA srcString='%S', strLen=%i\n", srcString, strLen);
	if (srcString == NULL)
		return 0;
	if (strLen < 0)
		strLen = wcslen(srcString);
	dprintf(" strLen=%i\n", strLen);

	int newLen = WideCharToMultiByte(
		CP_ACP,
		0,
		srcString,
		strLen , // not incl 0
		NULL, 
		0, // return buf size in char's
		NULL, // def char
		NULL  // &lb_UsedDefChar

	);

	dprintf("-ParamStorage::convertLenWtoA  newLen=%i\n", newLen);
	return newLen;
}


Library::Library(const string &libraryName, bool delayLoad)
	: m_libraryName(libraryName)
	, m_libraryHandle(NULL)
{
	if (!delayLoad)
		ensureIsLoaded();
}

Library * PBCALL lib_create(char *libraryName, bool delayLoad, BYTE *unicodeMarker) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		ParamStorage ps;
		if (IsUnicode(unicodeMarker)) {
			libraryName = ps.convertWtoA((wchar_t*)libraryName);
		}
		return new Library(libraryName, delayLoad);
	}
	catch (...) {
		return NULL;
	}
}
void PBCALL lib_destroy(Library * library) 
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	if (library != NULL) {
		delete library;
	}
}

void Library::ensureIsLoaded()
{
	if (m_libraryHandle != NULL)
		return;
	DWORD flags = 0;
	m_libraryHandle = LoadLibraryEx(m_libraryName.c_str(), NULL, flags);
	if (m_libraryHandle == NULL)
		throw RuntimeException(this, "Library " + m_libraryName + " could not be loaded", GetLastError());
}

Library::~Library()
{
	if (m_libraryHandle != NULL) {
		FreeLibrary(m_libraryHandle);
	}
}



RuntimeException::RuntimeException(IRuntimeExceptionRegistry *registry, const string &msg, DWORD lastOsError)
{
	init(registry, msg, lastOsError);
}
RuntimeException::RuntimeException(const string &msg, DWORD lastOsError)
{
	init(&Core::getInstance() /*registry*/, msg, lastOsError);
}

void RuntimeException::init(IRuntimeExceptionRegistry *registry, const string &msg, DWORD lastOsError)
{
	m_message = msg;
	if (m_message == "")
		m_message = "Unspecified Runtime Exception";
	m_message += formatErrorMessage(", LastError: ", "", lastOsError);

	if (registry == NULL)
		registry = &CoreInstance;
	
	registry->registerException(*this);

	dprintf("****** RuntimeException: %s ******\n", m_message.c_str());

}

string RuntimeException::formatErrorMessage(const string &prefix, const string &suffix, DWORD error)
{
	if (error == 0)
		return "";

	char numBuf[20];
	string errorCodeText = itoa(error, numBuf, 20);

	char *textBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		0, // Default language
		(LPTSTR) &textBuf,
		0,
		NULL 
	);
	
	string messageText = textBuf;
	LocalFree(textBuf);


	return prefix + "[" + errorCodeText + "] " + messageText + suffix;
}
 



void ExecutionContext::registerException(RuntimeException &e)
{
	//dprintf("registerException %p, text='%s'\n", this, e.getMessage().c_str());
	if (&PBExceptionStorageInstance != (PBExceptionStorage*)this)
		PBExceptionStorageInstance.registerException(e);

	m_lastExceptionMessage = e.getMessage();
}

string* PBCALL er_getLastException(IRuntimeExceptionRegistry *er)
{
	if (er == NULL)
		return new string(PBExceptionStorageInstance.getLastException());
	else
		return new string(er->getLastException());
}

string dumpBuf(BYTE *data, int size)
{
	const char *digitFmt = "%02X ";
	int digitLen = 3;
	int i;
	vector<char> hex;
	hex.resize(size * digitLen );
	for (i = 0; i < size; i ++) {
		sprintf(&hex[i * digitLen], digitFmt, data[i], data[i] == 0 ? '.' : data[i]);
	}
	hex.push_back('\0');

	vector<char> ch;
	ch.resize(size);
	for (i = 0; i < size; i ++) {
		ch[i] = data[i] < ' ' ? '.' : data[i];
	}
	ch.push_back('\0');


	return string(&hex[0]) + " " + string(&ch[0]);
}

size_t ParamStorage::getArgStackSize()
{
	return m_storageType->getSizeOf();
}

void ParamStorage::dumpData(const string &text)
{
#ifdef _DEBUG_MSG
	dprintf("\n******** dumpData: %s\n", text.c_str());
	dprintf("Params: %s\n", dumpBuf(m_paramDataPtr, m_storageType->getSizeOf()).c_str());
	dprintf("Buffers: %i\n", m_buffers.size());
	for (size_t i = 0; i < m_buffers.size(); i ++) {
		dprintf("  [%i]: %s\n", i, dumpBuf(m_buffers[i], m_bufferSizes[i]).c_str());
	}

	dprintf("********\n");
#endif
}

void ExecutionContext::clearBuffers()
{
	for (int i = m_buffers.size() - 1; i >= 0; i --) {
		delete [] m_buffers[i];
	}
	m_buffers.clear();
	m_bufferSizes.clear();
	
}

void ExecutionContext::clear()
{
	clearBuffers();
	m_lastExceptionMessage = "";
}


void ParamStorage::clear()
{
	ExecutionContext::clear();
	memset(m_paramDataPtr, m_storageType->getSizeOf(), 0);
}

void Method::clear()
{
	ParamStorage::clear();
	memset(&m_returnValue[0], m_returnValue.size(), 0);
}

BYTE * ParamStorage::getParamAddr(int paramIndex)
{
	return &m_paramDataPtr[m_storageType->getItemOffset(paramIndex)];
}

BYTE* Method::getReturnValueAddr()
{
	return &m_returnValue[0];
}


MethodInstance* Library::createMethodInstance(const string& methodDecl, bool enableTCharSuffix)
{
	// try 1st if method name was passed 
	string methodName = methodDecl;
	Type methodType = CoreInstance.findName(m_nameSpace, methodName, false);
	if (enableTCharSuffix && methodType.get() == NULL) {
		methodName += ( CoreInstance.isUnicode() ?  "W" : "A");
		methodType = CoreInstance.findName(m_nameSpace, methodName, false);
	}
	if (methodType.get() == NULL) {
		// no method with methodDecl name
		// compile declaration
		Compiler compiler(m_nameSpace, enableTCharSuffix);
		methodName = compiler.compileAsC(methodDecl.c_str());
		if (methodName == "") 
			throw RuntimeException(this, "Declaration does not contain function prototypes: '" + methodDecl + "'");
		methodType = CoreInstance.findName(m_nameSpace, methodName);
	}
	if (methodType->getPBTypeClass() != TC_METHOD_TYPE)
		throw RuntimeException(this, "Name '" + methodName + "' is not a function prototype'");
	dprintf("methodType=%p, methodType->getParams().size()=%i\n", methodType.get(), ((MethodType*)methodType.get())->getParams().size());	
	return new DLLMethodInstance(this, methodName, (MethodType*)methodType.get());
}



MethodInstance* PBCALL lib_createMethodInstance(Library * library, const char *methodDecl, bool enableTCharSuffix, BYTE *unicodeMarker)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		ParamStorage ps;
		if (IsUnicode(unicodeMarker)) {
			methodDecl = ps.convertWtoA((wchar_t*)methodDecl);
		}
		return library->createMethodInstance(methodDecl, enableTCharSuffix);
	}
	catch (...) {
		return NULL;
	}
}


void PBCALL s_destroy(string* s) 
{
	if (s != NULL)
		delete s;
}

int PBCALL s_size(string& s, bool expectUnicode) 
{
	if (expectUnicode) {
		ExecutionContext cx;
		return cx.strlenAtoW(s.c_str());
	}
	else {
		//dprintf("s_size size=%i\n", s.size());
		return s.size();
	}
}

void PBCALL s_get(string& s, BYTE* retStr, int retStrMaxLen, bool expectUnicode) 
{
	if (expectUnicode) {
		ExecutionContext cx;
		wchar_t* ws = cx.convertAtoW(s.c_str());
		int len = wcslen(ws);
		if (len > retStrMaxLen)
			len = retStrMaxLen;
		memcpy(retStr, ws, len * sizeof(wchar_t));
	}
	else {
		//dprintf("s_get size=%i, retStrMaxLen=%i, s='%s'\n", s.size(), retStrMaxLen, s.c_str());
		int len = strlen(s.c_str());
		if (len > retStrMaxLen)
			len = retStrMaxLen;
		memcpy(retStr, s.c_str(), len * sizeof(char));
	}
}


string ExecutionContext::getLastException() 
{ 
	//dprintf("getLastException %p, text='%s'\n", this, m_lastExceptionMessage.c_str());
	if (m_lastExceptionMessage == "" && &CoreInstance != (Core*)this)
		return CoreInstance.getLastException();
	string tmp = m_lastExceptionMessage; 
	m_lastExceptionMessage = ""; 
	return tmp; 
}; 
