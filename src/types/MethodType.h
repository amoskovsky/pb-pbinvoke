#pragma once

#include "RawType.h"
#include <hash_map>
using namespace stdext;


class IndexedMethodType: public RawType {
public:
	IndexedMethodType(Type returnType, vector<Type> &paramTypes, bool isStdCall = true, DWORD methodAddr = 0);
	virtual ~IndexedMethodType();
	virtual int getPBTypeClass() { return TC_METHOD_TYPE; }
	inline vector<Type> &getParamTypes() { return m_paramTypes; }
	inline Type &getReturnType() { return m_returnType; }
	inline bool isStdCall() { return m_isStdCall; }
	inline void setStdCall(bool isStdCall) { m_isStdCall = isStdCall; }
	virtual string getTypeSignature(const string& pointerSig = "");
	virtual bool hasTChar();
private:
	vector<Type> m_paramTypes;
	Type m_returnType;
	bool m_isStdCall;
	Library * m_library;
	string m_methodName;
	DWORD m_methodAddr;
	static string buildTypeSignature(const string &pointerName, const string &methodName, Type returnType, vector<Type> &paramTypes, bool isStdCall = true);

};


class MethodType: public RawType {
public:
	MethodType(Type returnType, vector<NamedType> &params, bool isStdCall = true);
	virtual int getPBTypeClass() { return TC_METHOD_TYPE; }
	inline vector<NamedType> &getParams() { return m_params; }
	inline Type &getReturnType() { return m_indexedMethodType->getReturnType(); }
	inline bool isStdCall() { return m_indexedMethodType->isStdCall(); }
	inline void setStdCall(bool isStdCall) {m_indexedMethodType->setStdCall(isStdCall); }
	virtual string getTypeSignature(const string& pointerSig = "");
	virtual bool isCompatible(BaseType* type);
	virtual int getItemIndex(const string& itemName);
	virtual bool hasTChar() { return m_indexedMethodType->hasTChar(); }
private:
	IndexedMethodType *m_indexedMethodType;
	vector<NamedType> m_params;
	string buildTypeSignature(const string &pointerName);
	hash_map<string, int> m_paramNameMap;
	void init();
};

