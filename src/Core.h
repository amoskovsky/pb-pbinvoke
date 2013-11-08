#pragma once

#pragma warning(disable: 4786)

#include "Runtime.h"
#include "WinHooks.h"

#include <stack>

// breaks VC6 compatibility
#include <hash_map>
using namespace stdext;


#define PBM_CUSTOM01 (WM_USER + 0)


class Core : public ExecutionContext
{
public:
	Type& registerType(Type &typeRef);
	static Core& getInstance();
	/** Gets current packing for structures */
	int getPack();
	void setPack(int pack);
	void pushPack(int pack);
	void popPack();
	void setCallbackMessageHandler(HWND callbackMessageHandler);
	void setCallbackMessageId(UINT callbackMessageId);
	inline HWND getCallbackMessageHandler() { return m_callbackMessageHandler; }
	inline UINT getCallbackMessageId() { return m_callbackMessageId; }
	Type& findType(const string &nameSpace, const string& typeName, bool throwNotFound = true);
	Type& defineType(const string &nameSpace, const string& typeName, Type &typeRef);
	Type& findName(const string &nameSpace, const string& name, bool throwNotFound = true);
	Type& defineName(const string &nameSpace, const string& name, Type &typeRef);
	void compile(const string &nameSpace, const string& source);
	static void resetState();
	void initTChar();
	inline bool isUnicode() { return m_isUnicode; }
	inline void setUnicode(bool isUnicode) { m_isUnicode = isUnicode; }
	inline vector<WinHookHelper>& getWinHookHelpers() { return m_winHookHelpers;} 
	inline bool getDefCallingConvention() { return m_defCallingConvention;}
	inline void setDefCallingConvention(bool isStdCall) { m_defCallingConvention = isStdCall;}
private:
	static Core * m_instance;
	Core();
	virtual ~Core();
	Core(Core&) {}
	Core& operator=(Core&) {}

	/** All types seen by PBInvoke */
	vector<Type> m_types;
	/** Map typesig -> type index in m_types */
	hash_map<string, int> m_typeMap;
	/** Map typename -> type index in m_types */
	hash_map<string, int> m_typeDefs;
	/** Map var or fn name -> type index in m_types */
	hash_map<string, int> m_names;

	vector<WinHookHelper> m_winHookHelpers;
	int m_pack;

	stack<int> m_packStack;
	HWND m_callbackMessageHandler;
	UINT m_callbackMessageId;
	int m_maxInternalTypeIndex;
	void initInternalTypes(void);
	void initInternalNames(void);
	inline void registerType(Type &typeRef, int &typeIndex);
	bool m_isTCharInitialized;
	bool m_isUnicode;
	bool m_defCallingConvention;

	void initWinAPI();
	void initWinHookHelpers();
	
};

#define CoreInstance Core::getInstance()


