#pragma once

#include <boost/shared_ptr.hpp>
#include "../types/Basetype.h"
#include <iostream>
#include "../Compiler.h"
#include "../Runtime.h"
#include "../Core.h"
using namespace std;

enum {
	ATTR_SIGNED   = 1 << 0,
	ATTR_UNSIGNED = 1 << 1,
	ATTR_CHAR     = 1 << 2,  
	ATTR_INT8     = 1 << 3,  
	ATTR_WCHAR_T  = 1 << 4, 
	ATTR_SHORT    = 1 << 5,
	ATTR_INT16    = 1 << 6, 
	ATTR_INT      = 1 << 7,   
	ATTR_INT32    = 1 << 8,  
	ATTR_LONG     = 1 << 9,   
	ATTR_INT64    = 1 << 10,
	ATTR_LONGLONG = 1 << 11,
	ATTR_DOUBLE   = 1 << 12,
	ATTR_FLOAT    = 1 << 13,
	ATTR_VOID     = 1 << 14,
	ATTR_COUNT    = 15,
};

struct ASTPtrTypeOp;
struct ASTRefTypeOp;
struct ASTDeclaratorId;
struct ASTNodeList;
struct ASTMethodTypeOp;
struct ASTDeclarator;
struct ASTStructType;
struct ASTArrayTypeOp;

struct IASTNode {
	virtual ~IASTNode() {}
	virtual int getTypeId() { return ASTNodeId; }

	inline ASTNodeList* getNodeList() { return (ASTNodeList*)this; }
	inline ASTPtrTypeOp* getPtrTypeOp() { return (ASTPtrTypeOp*)this; }
	inline ASTRefTypeOp* getRefTypeOp() { return (ASTRefTypeOp*)this; }
	inline ASTDeclaratorId* getDeclaratorId() { return (ASTDeclaratorId*)this; }
	inline ASTMethodTypeOp* getMethodTypeOp() { return (ASTMethodTypeOp*)this; }
	inline ASTDeclarator* getDeclarator() { return (ASTDeclarator*)this; }
	inline ASTStructType* getStructType() { return (ASTStructType*)this; }
	virtual void setTypeAttr(int attr) { throw RuntimeException("Internal error: illegal use of type attributes"); }
	virtual Type getTypeRef() { throw RuntimeException("Internal error: illegal use of type reference"); }

protected:
	enum {
		ASTNodeId,
		ASTPtrTypeOpId,
		ASTRefTypeOpId,
		ASTDeclaratorIdId,
		ASTNodeListId,
		ASTMethodTypeOpId,
		ASTArrayTypeOpId,
	};
};

typedef boost::shared_ptr<IASTNode> ASTNode;

struct ASTDeclarator: public IASTNode {
	virtual void convert(Type typeRef, NamedType& namedType) = 0;
	virtual void setStdCall(bool _isStdCall) = 0;
	virtual bool getStdCall() = 0;
};

struct ASTPtrTypeOp: public ASTDeclarator {
	ASTNode arg;
	ASTPtrTypeOp(ASTNode _arg) : arg(_arg) {}
	virtual int getTypeId() { return ASTPtrTypeOpId; }
	virtual void setStdCall(bool _isStdCall) {arg->getDeclarator()->setStdCall(_isStdCall);}
	virtual bool getStdCall() {return arg->getDeclarator()->getStdCall();}
	virtual void convert(Type typeRef, NamedType& namedType);
};

struct ASTRefTypeOp: public ASTDeclarator {
	ASTNode arg;
	ASTRefTypeOp(ASTNode _arg) : arg(_arg) {}
	virtual int getTypeId() { return ASTRefTypeOpId; }
	virtual void setStdCall(bool _isStdCall) {arg->getDeclarator()->setStdCall(_isStdCall);}
	virtual bool getStdCall() {return arg->getDeclarator()->getStdCall();}
	virtual void convert(Type typeRef, NamedType& namedType);
};

struct ASTDeclaratorId: public ASTDeclarator {
	bool isStdCall;
	string id;
	ASTDeclaratorId(const string& _id) : id(_id), isStdCall(CoreInstance.getDefCallingConvention()) {}
	virtual int getTypeId() { return ASTDeclaratorIdId; }
	virtual void setStdCall(bool _isStdCall) { isStdCall = _isStdCall; };
	virtual bool getStdCall() { return isStdCall;}
	virtual void convert(Type typeRef, NamedType& namedType);
};

struct ASTNodeList: public IASTNode {
	vector<ASTNode> list;
	ASTNodeList(ASTNode _node) { list.push_back(_node); }
	ASTNodeList() { }
	virtual int getTypeId() { return ASTNodeListId; }
	void typeDef(Type typeRef, Compiler& compiler);
	void nameDef(Type typeRef, Compiler& compiler);
	void nameDef(Type typeRef, vector<NamedType> &names, Compiler& compiler);
};

struct ASTMethodTypeOp: public ASTDeclarator {
	bool isStdCall;
	vector<NamedType> params;
	ASTNode id;
	ASTMethodTypeOp(ASTNode _id, vector<NamedType>& _params): params(_params), isStdCall(CoreInstance.getDefCallingConvention()), id(_id) { }
	virtual void setStdCall(bool _isStdCall) { isStdCall = _isStdCall; }
	virtual bool getStdCall() { return isStdCall;}
	virtual int getTypeId() { return ASTMethodTypeOpId; }
	virtual void convert(Type typeRef, NamedType& namedType);
};

struct ASTStructType: public IASTNode {
	string name;
	Type structType;
	Compiler &compiler;
	ASTStructType(const string& _name, Compiler &_compiler);
	void completeStruct(int structTypeId, vector<NamedType> &_fields);
	void forwardDecl();
	virtual Type getTypeRef();
};

struct ASTInternalType: public IASTNode {
	int attrs;
	Compiler &compiler;
	ASTInternalType(Compiler &_compiler);
	virtual void setTypeAttr(int attr);
	virtual Type getTypeRef();
	static string getInternalTypeName(int attr);
};
struct ASTUserType: public IASTNode {
	string name;
	Compiler &compiler;
	ASTUserType(const string& _name, Compiler &_compiler);
	virtual Type getTypeRef();
};

struct ASTArrayTypeOp: public ASTDeclarator {
	ASTNode m_arg;
	int m_bound;
	bool m_isParam;
	ASTArrayTypeOp(ASTNode arg, int bound, bool isParam) 
		: m_arg(arg), m_bound(bound), m_isParam(isParam) {}
	virtual int getTypeId() { return ASTArrayTypeOpId; }
	virtual void setStdCall(bool isStdCall) {m_arg->getDeclarator()->setStdCall(isStdCall);}
	virtual bool getStdCall() {return m_arg->getDeclarator()->getStdCall();}
	virtual void convert(Type typeRef, NamedType& namedType);
};

