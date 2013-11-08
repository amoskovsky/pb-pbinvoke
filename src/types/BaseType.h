#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>

using namespace std;




/** Short type for byte buffers*/
typedef vector<BYTE> blob;

// forward decls
class ParamStorage;
class Method;
class ParamStorage;
class Library;
class ExecutionContext;

enum PBTypeClass {
	TC_BASE_TYPE   = 1,
	TC_STRING_TYPE = 2,
	TC_METHOD_TYPE = 3,
	TC_BOOL_TYPE   = 4,
	TC_INT_TYPE    = 5,
	TC_DOUBLE_TYPE = 6,
	TC_FLOAT_TYPE  = 7,
	TC_STRUCT_TYPE = 8,
	TC_PTR_TYPE    = 9,
	TC_CHAR_TYPE   = 10,
	TC_REF_TYPE    = 11,
	TC_UNION_TYPE  = 12,
	TC_ENUM_TYPE   = 13,
	TC_VOID_TYPE   = 14,
	TC_SHORT_TYPE  = 15,
	TC_LONGLONG_TYPE = 16,
	TC_ARRAY_TYPE  = 17,
};

enum DataCharset {
	DC_BINARY  = 1,
	DC_ANSI    = 2,
	DC_UNICODE = 3,
};

class BaseType
{
protected:
	//BaseType(const string &typeSignature);
	BaseType();
	//friend class TypeFactory;
public:
	virtual ~BaseType();

	/**
		Returns enumerated PB wrapper-class names for this C++ class
	*/
	virtual int getPBTypeClass() = 0;
	

	/**
		Returns size of C type value in memory.
		Subclasses must override it.
	*/
	virtual int getSizeOf() = 0;


	/**
		Converts PB value to C type and stores to memory
		
		Each type knows its size.

		@param cBuf          buffer to store C value to
		@param pbBuf         buffer to read PB value from
	*/
	inline void setData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf);
	
	inline void setPtr(ExecutionContext *context, BYTE* cBuf, BYTE* pbBuf);
	
	inline void setInt64(ExecutionContext *context, __int64* cBuf, BYTE* str, int strLen, DataCharset charset);
	inline void getInt64(ExecutionContext *context, __int64* cBuf, string* &str);

	/**
		Converts PB string value (SZ) to C type and stores to memory
		
		Converts A/W if needed.

		@param cBuf          buffer to store SZ pointer to
		@param pbBuf         buffer to read PB string value from
		@param isPBUnicode   specifies if PB Runtime (the caller) is unicode
	*/
	virtual void setArrayData(ExecutionContext *context, BYTE * &cBuf, BYTE *pbBuf, int arrayLen, DataCharset charset) = 0;
	
	//void setPointerToArrayData(ExecutionContext *context, BYTE * cPtrBuf, BYTE *pbBuf, int arrayLen, DataCharset charset);

	/**
		Converts C string value in memory to PB type and stores to buffer.
		
		Is used to marshal values from C++ to PB.

		@param cBuf         memory pointer to read from
		@param pbBuf        Buffer to copy PB value to
		@param pbBufSize    [in/out] If 0 then is set to needed buffer size without copying.
		                             If >0 then the value is copied to the buffer with max bufSize bytes.
								     If the value is NULL then bufSize is set to -1
	*/
	inline void getData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf);
	
	inline void getPtr(ExecutionContext *context, BYTE* cBuf, BYTE* &pbBuf);
	
	/**
		Converts C string value in memory to PB type and stores to buffer.
		
		Is used to marshal values from C++ to PB.

		@param cBuf         memory pointer to read from
		@param pbBuf        Buffer to copy PB value to
		@param strLen       [in/out] If 0 then is set to needed buffer size without copying.
		                             If >0 then the value is copied to the buffer with max bufSize bytes.
								     If the value is NULL then bufSize is set to -1
		@param isPBUnicode  specifies if PB Runtime (the caller) is unicode
	*/
	virtual void getArrayData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf, int &strLen, DataCharset charset) = 0;

	/**
		Converts C string value in memory to PB type and stores to buffer.
		
		Is used to marshal values from C++ to PB.

		@param ptr          memory pointer to read from
		@param buf          Buffer to copy PB value to
		@param bufSize      [in/out] If 0 then is set to needed buffer size without copying.
		                             If >0 then the value is copied to the buffer with max bufSize bytes.
								     If the value is NULL then bufSize is set to -1
		@param isPBUnicode  specifies if PB Runtime (the caller) is unicode
	*/
	virtual void executeMethod(ExecutionContext *context, DWORD methodAddr, BYTE *retBuf, BYTE *paramBuf, int paramBufSize, bool isStdCall) {}

	/**
		Returns item offset withing the structure (array or struct)

		@param  itemIndex index of the item in order of declaration
		
		@return offset in bytes from the begin of the structure
	*/
	virtual int getItemOffset(int itemIndex) = 0;

	/**
		Allocates a buffer of sizeof(T) bytes, capable to store a value of that type.
		For Strings this is a sizeof(char*).
		
		Each type knows its size.

		@param buf   buffer ptr ref 
	*/
	virtual void allocateBuffer(ExecutionContext *context, BYTE * &buf, int itemCount = 1) = 0;
	
	/**
		Fills the buffer with sizeof(T) zero bytes.
		
		Each type knows its size.

		@param buf   buffer ptr  
	*/
	virtual void setZeroData(ExecutionContext *context, BYTE * buf) = 0;

	/** 
		Gets the type signature string.
		"char*", "int(int, int)", "char[3]"
	*/
	virtual string getTypeSignature(const string& pointerSig = "") = 0;

	/** 
		Gets the default alignment for the type
	*/
	virtual int getAlign() = 0;
	/** 
		Gets the value of __declspec(align(n)) for the item declaration.
		@return align or 0 if not specified
	*/
	virtual int getExplicitAlign() = 0;

	virtual bool isSame(BaseType* type) { return getTypeSignature() == type->getTypeSignature(); }
	virtual bool isCompatible(BaseType* type) { return this == type; }
	
	virtual size_t strLen(ExecutionContext *context, BYTE * buf, int arrayLen, DataCharset charset) = 0;

	/** Is the type defined completely or partially (as struct forward declaration) */
	virtual bool isComplete() { return true; }

	virtual string getTagName() { return ""; }

	virtual int getItemIndex(const string& itemName) { return 0; }
	bool isPointerToMethod();
	inline bool isMethod() { return getPBTypeClass() == TC_METHOD_TYPE; }
	inline bool isVoid() { return getPBTypeClass() == TC_VOID_TYPE; }
	
	virtual bool hasTChar() { return false; }
	
protected:
	

};

inline void BaseType::setData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf)
{
	setArrayData(context, cBuf, pbBuf, 1, DC_BINARY); 
}

inline void BaseType::getData(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf)
{ 
	int len = 1; 
	getArrayData(context, cBuf, pbBuf, len, DC_BINARY); 
}

inline void BaseType::getPtr(ExecutionContext *context, BYTE * cBuf, BYTE *&pbBuf)
{
	pbBuf = *(BYTE**)cBuf;
}
inline void BaseType::setPtr(ExecutionContext *context, BYTE * cBuf, BYTE *pbBuf)
{
	*(BYTE**)cBuf = pbBuf;
}


typedef boost::shared_ptr<BaseType> Type;

struct NamedType {
	Type type;
	string name;

	// helpers
	static vector<NamedType> getNamedTypes(vector<Type> &v);
	inline static vector<NamedType>& getNamedTypes(vector<NamedType> &v) { return v; }

	static vector<string> getNames(vector<Type> &v);
	static vector<string> getNames(vector<NamedType> &v);

	inline static vector<Type>& getTypes(vector<Type> &v) { return v; }
	static vector<Type> getTypes(vector<NamedType> &v);

};


