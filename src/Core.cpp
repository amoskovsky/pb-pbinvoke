#include "stdafx.h"

#include "Core.h"
#include "Runtime.h"
#include "types/DataTypes.h"
#include "PBException.h"
#include "Compiler.h"
#include <time.h>

Core * Core::m_instance = NULL;

void Core::resetState() 
{ 
    dprintf("Core::resetState\n");
    if (m_instance != NULL) {
        delete m_instance;
        m_instance = NULL;
    } 
}


Core::Core()
    : m_pack(8)
    , m_callbackMessageHandler(NULL)
    , m_callbackMessageId(PBM_CUSTOM01)
    , m_maxInternalTypeIndex(0)
    , m_isTCharInitialized(false)
    , m_defCallingConvention(true)
{
    // allocate 0-th index so it would not be used by any type
    m_types.push_back(Type());

    initInternalTypes();
    initInternalNames();
    initWinHookHelpers();
}

Core::~Core()
{

}

Core& Core::getInstance() 
{
    if (m_instance == NULL) {
        m_instance = new Core();
    }
    return *m_instance;
}

Core* PBCALL g_getCoreInstance() 
{
    return &CoreInstance;
}

void PBCALL core_setDefCallingConvention(bool isStdCall)
{ 
    CoreInstance.setDefCallingConvention(isStdCall);
}


void Core::initWinAPI()
{
    Compiler cc;
    //dprintf("initWinAPI 1=%i\n", time(NULL));
    cc.compileAsC(
        "typedef tchar_t TCHAR, *TSTR, *LPTSTR, *LPCTSTR;"
        "typedef wchar_t WCHAR, *WSTR, *LPWSTR, *LPCWSTR;"
        "typedef char CHAR, *STR, *LPSTR, *LPCSTR, BYTE;"
        "typedef bool BOOL;"
        "typedef __thandle HWND, HINSTANCE;"
        "typedef int *LPBOOL, INT, UINT, *UINT_PTR, DWORD, *LPDWORD,"
                    "HANDLE, LRESULT, LONG, ULONG, WPARAM, LPARAM, HMODULE, HHOOK, LPSECURITY_ATTRIBUTES;"
        "typedef short WORD, *LPWORD, SHORT, USHORT;"
        "typedef void VOID, *PVOID, *LPVOID, *LPCVOID;"
        "typedef float FLOAT;"
        );
    //dprintf("initWinAPI 2=%i\n", time(NULL));
}


void Core::compile(const string &nameSpace, const string& source)
{
    Compiler cc(nameSpace);
    cc.compileAsC(source.c_str());
} 

BOOL PBCALL core_compile(const char* nameSpace, const char* source, DataCharset charset)
{
    PBExceptionStorageInstance.clearLastExceptionMessage();
    try {
        ParamStorage ps;
        if (charset == DC_UNICODE) {
            nameSpace = ps.convertWtoA((wchar_t*)nameSpace);
            source = ps.convertWtoA((wchar_t*)source);
        }
        CoreInstance.compile(nameSpace,  source);
        return true;
    }
    catch(...) {
        return false;
    }
}
BOOL PBCALL core_setKey(const char* key, DataCharset charset)
{
    PBExceptionStorageInstance.clearLastExceptionMessage();
    try {
        ParamStorage ps;
        if (charset == DC_UNICODE) {
            key = ps.convertWtoA((wchar_t*)key);
        }
        return true;
    }
    catch(...) {
        return false;
    }
}
BOOL PBCALL core_setAppHandle(DWORD handle)
{
    PBExceptionStorageInstance.clearLastExceptionMessage();
    try {
        
        return true;
    }
    catch(...) {
        return false;
    }
}
inline void Core::registerType(Type &typeRef, int &typeIndex)
{
    // avoid dup types
    string sig = typeRef->getTypeSignature();
    typeIndex = m_typeMap[sig];
    if (typeIndex == 0 /* see Core ctor */) {
        typeIndex = m_types.size();
        m_typeMap[sig] = typeIndex;
        m_types.push_back(typeRef);
    }
    if (typeIndex >= 7) {
        int i; i = 1;
    }
    dprintf("registerType sig='%s', index=%i, this=%p\n", sig.c_str(), typeIndex, m_types[typeIndex].get());
}

void Core::initTChar() 
{
    if (m_isTCharInitialized)
        return;
    dprintf("+Core::initTChar\n");
    if (m_isUnicode) {
        defineType("", "tchar_t", Type(new TWCharType()));
    }
    else {
        defineType("", "tchar_t", Type(new TACharType()));
    }

    initWinAPI();
    m_isTCharInitialized = true;
    dprintf("-Core::initTChar\n");
}

Type& Core::registerType(Type &typeRef)
{
    int index;
    registerType(typeRef, index);
    return m_types[index];
}



Type& Core::defineType(const string &nameSpace, const string& typeName, Type &typeRef)
{
    dprintf("+defineType %s, this=%p\n", typeName.c_str(), typeRef.get());
    string name = nameSpace + "::" + typeName;
    int index = m_typeDefs[name];
    if (index != 0 && index <= m_maxInternalTypeIndex ) {
        throw RuntimeException("Cannot redefine internal type '" + typeName + "'");
    }
    else if (index > m_maxInternalTypeIndex) {
        if (!typeRef->isSame(m_types[index].get()))
            throw RuntimeException("Cannot redefine type/function '" + typeName + "' with different type/function");
    }
    registerType(typeRef, index);
    m_typeDefs[name] = index;
    dprintf("-defineType %s=%s\n", name.c_str(), m_types[index]->getTypeSignature().c_str());
    return m_types[index];
}

Type& Core::findType(const string &nameSpace, const string& typeName, bool throwNotFound)
{
    // find in namespace
    int index = m_typeDefs[nameSpace + "::" + typeName];
    if (index != 0)
        return m_types[index];

    if (nameSpace != "") {
        // find in global namespace
        index = m_typeDefs["::" + typeName];
        if (index != 0) 
            return m_types[index];
    }

    if (throwNotFound)
        throw RuntimeException("Type '" + typeName + "' not found");

    static Type empty;
    return empty;
}

Type& Core::findName(const string &nameSpace, const string& name, bool throwNotFound)
{
    // find in namespace
    int index = m_names[nameSpace + "::" + name];
    if (index != 0)
        return m_types[index];

    if (nameSpace != "") {
        // find in global namespace
        index = m_names["::" + name];
        if (index != 0) 
            return m_types[index];
    }

    if (throwNotFound)
        throw RuntimeException("Name '" + name + "' not found");

    static Type empty;
    return empty;
}

Type& Core::defineName(const string &nameSpace, const string& name, Type &typeRef)
{
    string fullName = nameSpace + "::" + name;
    int index = m_names[fullName];
    if (index > 0 ) {
        if (!typeRef->isSame(m_types[index].get()))
            throw RuntimeException("Cannot redefine name '" + name + "' with different type");
    }
    registerType(typeRef, index);
    m_names[fullName] = index;
    dprintf("defineName %s=%s\n", fullName.c_str(), m_types[index]->getTypeSignature().c_str());
    return m_types[index];
}




int Core::getPack()
{
    return m_pack;
}

int PBCALL core_getPack() {
    PBExceptionStorageInstance.clearLastExceptionMessage();
    return CoreInstance.getPack();
}

void Core::setPack(int pack)
{
    m_pack = pack;
    dprintf("Core::setPack %i\n", pack);
}

BOOL PBCALL core_setPack(int pack) {
    PBExceptionStorageInstance.clearLastExceptionMessage();
    try {
        CoreInstance.setPack(pack);
        return true;
    }
    catch(...) {
        return false;
    }
}
void Core::pushPack(int pack)
{
    m_packStack.push(m_pack);
    setPack(pack);
}

BOOL PBCALL core_pushPack(int pack) {
    PBExceptionStorageInstance.clearLastExceptionMessage();
    try {
        CoreInstance.pushPack(pack);
        return true;
    }
    catch(...) {
        return false;
    }
}

void Core::popPack()
{
    if (m_packStack.empty())
        throw RuntimeException(this, "Pack stack is empty");
    int oldPack = m_packStack.top();
    m_packStack.pop();
    setPack(oldPack);
}

BOOL PBCALL core_popPack() {
    PBExceptionStorageInstance.clearLastExceptionMessage();
    try {
        CoreInstance.popPack();
        return true;
    }
    catch(...) {
        return false;
    }
}

void Core::setCallbackMessageHandler(HWND callbackMessageHandler)
{
    PBExceptionStorageInstance.clearLastExceptionMessage();
    m_callbackMessageHandler = callbackMessageHandler;
}

void PBCALL core_setCallbackMessageHandler(HWND callbackMessageHandler)
{
    CoreInstance.setCallbackMessageHandler(callbackMessageHandler);
}

void Core::setCallbackMessageId(UINT callbackMessageId)
{
    PBExceptionStorageInstance.clearLastExceptionMessage();
    m_callbackMessageId = callbackMessageId;
}

void PBCALL core_setCallbackMessageId(UINT callbackMessageId)
{
    CoreInstance.setCallbackMessageId(callbackMessageId);
}


    //static Type staticAStringType(new AStringType());
    //static Type staticWStringType(new WStringType());
    //static Type staticBoolType(new BoolType());
    //static Type staticIntType(new IntType());
    //static Type staticDoubleType(new DoubleType());
    //static Type staticFloatType(new FloatType());


void Core::initInternalTypes(void)
{
    defineType("", "int", Type(new IntType()));
    defineType("", "char", Type(new ACharType()));
    defineType("", "wchar_t", Type(new WCharType()));
    defineType("", "bool", Type(new BoolType()));
    defineType("", "double", Type(new DoubleType()));
    defineType("", "float", Type(new FloatType()));
    defineType("", "void", Type(new VoidType()));
    defineType("", "short", Type(new ShortType()));
    defineType("", "__int64", Type(new Int64Type()));
    defineType("", "__thandle", Type(new THandleType()));
    // insert here defineType("", "...", type);
    m_maxInternalTypeIndex = m_types.size() - 1;
}

void Core::initInternalNames(void)
{
    // insert here defineName("...", type);
}


struct WinHookHelper_CALLWNDPROCRET: public IWinHookHelper {
    virtual UINT& getMessage(PHookProcArgs args) { return ((PCWPRETSTRUCT)args->lParam)->message;}
};
struct WinHookHelper_CALLWNDPROC: public IWinHookHelper {
    virtual UINT& getMessage(PHookProcArgs args) { return ((PCWPSTRUCT)args->lParam)->message;}
};
struct WinHookHelper_GETMESSAGE: public IWinHookHelper {
    virtual UINT& getMessage(PHookProcArgs args) { return ((PMSG)args->lParam)->message;}
};
//struct WinHookHelper_SHELL: public IWinHookHelper {
//  virtual UINT& getMessage(PHookProcArgs args) { return (()args->lParam)->message;}
//};



#define INSTALL_HELPER(h) { \
    if (m_winHookHelpers.size() < WH_ ## h + 1) m_winHookHelpers.resize(WH_ ## h + 1);\
    m_winHookHelpers[WH_ ## h] = WinHookHelper(new WinHookHelper_ ## h());\
}
void Core::initWinHookHelpers()
{
    INSTALL_HELPER(CALLWNDPROCRET);
    INSTALL_HELPER(CALLWNDPROC);
    INSTALL_HELPER(GETMESSAGE);
    //INSTALL_HELPER(SHELL);
}


BOOL PBCALL core_check() 
{
    PBExceptionStorageInstance.clearLastExceptionMessage();
    return true;
}

