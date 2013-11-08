#pragma once

#pragma warning (disable:4786)
#include <vector>

#include <windows.h>

//#include "DataTypes.h"
#include "types/BaseType.h"
#include "types/MethodType.h"
#include "MethodInstance.h"

using namespace std;


class ParamStorage;
class IRuntimeExceptionRegistry;

class RuntimeException 
{
public: 
	string formatErrorMessage(const string &prefix, const string &suffix, DWORD error);
	RuntimeException(const string &msg = "", DWORD lastOsError = 0);
	RuntimeException(IRuntimeExceptionRegistry *registry, const string &msg = "", DWORD lastOsError = 0);
	inline string& getMessage() { return m_message; }
private:
	string m_message;
	void init(IRuntimeExceptionRegistry *registry, const string &msg = "", DWORD lastOsError = 0);
};

class IRuntimeExceptionRegistry{
public:
	/** Register exception */
	virtual void registerException(RuntimeException &e) = 0;
	virtual string getLastException() = 0;
	virtual void clearLastExceptionMessage() = 0;
};

class ExecutionContext: public IRuntimeExceptionRegistry 
{
public:
	ExecutionContext() {}
	virtual ~ExecutionContext();

	/**
		Converts ANSI string to Unicode.
		Allocates memory if needed. Memory is freed auto on scope exit.

		@param srcString source ANSI string with trailing zero char.
		@param strLen    length of srcString in characters.
		@param buf       preallocated buffer
	*/
	wchar_t* convertAtoW(const char *srcString, int strLen = -1, wchar_t *buf = NULL);

	/**
		Converts ANSI string length to Unicode string length.
		No memory is allocated.

		@param srcString source ANSI string with trailing zero char.
		@param strLen    length of srcString in characters (not counting zero char).

		@return length of Unicode string in characters (not counting zero char)
	*/
	int strlenAtoW(const char *srcString, int strLen = -1);

	/**
		Converts Unicode string to ANSI.
		Allocates memory if needed. Memory is freed auto on scope exit.

		@param srcString source Unicode string with trailing zero char.
		@param strLen    length of srcString in characters.
		@param buf       preallocated buffer
	*/
	char* convertWtoA(const wchar_t *srcString, int strLen = -1, char *buf = NULL);

	/**
		Converts Unicode string length to ANSI string length.
		No memory is allocated.

		@param srcString source Unicode string with trailing zero char.
		@param strLen    length of srcString in characters (not counting zero char).

		@return length of Unicode string in characters (not counting zero char)
	*/
	int strlenWtoA(const wchar_t *srcString, int strLen = -1);

	/** Register exception text */
	void registerException(RuntimeException &e);


	virtual string getLastException();
	
	virtual void clearLastExceptionMessage() { m_lastExceptionMessage = ""; }

	/**
		Allocates buffer for data, pointers to which are passed as params.
		Callers of setParam must copy data to the buffer in order to pass the pointer to it as param.
		This is needed for re-entrance.
		These buffers are freed automatically on ParamStorage scope's exit

		@param size Buffer size in bytes.

		@return Pointer to allocated buffer
	*/
	BYTE * allocateBuffer(int size, int align = 1);

	virtual void clear();

protected:
	/** Allocated param buffers */
	vector<BYTE*> m_buffers;
	
	/** Allocated param buffers sizes*/
	vector<int> m_bufferSizes;

	/** The last exception occured during calls to ExecutionContext. 
		TODO store previous exceptions */
	string m_lastExceptionMessage;

	void clearBuffers();
};

/**
	Class implementing storage of and access to params of a method .
	TODO merge to class Method

*/
class ParamStorage: public ExecutionContext {
public:
	BYTE * getParamAddr(int paramIndex);
	/**
		Ctor. 

		@param paramTypes  Array of param Types (metadata)
	*/
	//ParamStorage(vector<Type> &paramTypes, BYTE* argsPtr = NULL);
	ParamStorage(vector<NamedType> &params, BYTE* argsPtr = NULL);
	ParamStorage() : m_storageType(Type()) {}

	/**
		Dtor.
	*/
	virtual ~ParamStorage();

	
	/**
		Frees the allocated buffers and clears the params values.
		Must be called before re-using the ParamStorage instance.
	*/
	//void clearParamData();

	virtual void clear();

	size_t getArgStackSize();
	inline void setArgsPtr(BYTE *argsPtr) { m_paramDataPtr = argsPtr; }

	void dumpData(const string &text = "");
protected:
	
	/** Packed params binary values ready to be passed to a C function */
	blob m_paramDataBuf;
	BYTE* m_paramDataPtr;

private:

	/** Init */
	void init();

protected:
	/** Storage metadata */
	Type m_storageType;


	/** Param offsets in m_paramData */
	vector<BYTE*> m_paramPointers;

};

class Library: public ExecutionContext {
public:
	Library(const string &libraryName, bool delayLoad =  false);
	virtual  ~Library();
	DWORD getMethodAddr(const string &methodName, bool enableTCharSuffix = true);
	Type& getMethodType(const string &methodName);
	MethodInstance* createMethodInstance(const string& methodDecl, bool enableTCharSuffix);
	//MethodType* createMethodType(char * methodName, char *returnType, 
	//	char *paramTypes[], int paramCount, bool isStdCall = true, bool isUnicode = false);

	// IRuntimeExceptionRegistry
	/** Register exception */

private:
	void ensureIsLoaded();
	string m_libraryName;
	HMODULE m_libraryHandle;
	string m_nameSpace;
};


class Method: public ParamStorage
{
public:
	BYTE* getReturnValueAddr();
	virtual void clear();

	void invoke(MethodInstance *methodInstance);
	Method(MethodType *methodType, BYTE* argsPtr = NULL);
	virtual ~Method();
	inline MethodType *getMethodType() { return m_methodType; }
private:
	MethodType *m_methodType;
	vector<BYTE> m_returnValue;
};

typedef boost::shared_ptr<Method> MethodContextPtr;