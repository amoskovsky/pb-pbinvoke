#pragma once

#include "RawType.h"
#include <hash_map>
using namespace stdext;

class IndexedStructType: public RawType {
public:
	IndexedStructType(vector<Type> &itemTypes, int pack = 8, int explicitAlign = 0, bool isUnion = false, bool isArgStack = false);
	IndexedStructType(const string& body, int pack = 8, int explicitAlign = 0, bool isUnion = false, bool isArgStack = false);

	virtual int getPBTypeClass() {return TC_STRUCT_TYPE; }
	virtual int getSizeOf() {return m_sizeOf; }
	
	virtual int getItemOffset(int itemIndex) {return m_itemOffsets[itemIndex]; }
	inline vector<Type>& getItemTypes() { return m_itemTypes; }
	inline vector<int>& getItemOffsets() { return m_itemOffsets; }
	inline int getPack() { return m_pack; }
	virtual string getTypeSignature(const string& pointerSig = "");
	virtual int getExplicitAlign() { return m_explicitAlign; }
	virtual int getAlign() { return m_align; }
	inline bool isArgStack() { return m_isArgStack; }
	inline bool isUnion() { return m_isUnion; }
	virtual bool hasTChar();

private:
	
	/** Item types */
	vector<Type> m_itemTypes;
	
	/** Item offsets in the structure */
	vector<int> m_itemOffsets;
	
	/** Is the structure a union (all offsets == 0)? */
	bool m_isUnion;

	/** Packing of items, the value of #pragma pack(n) */
	int m_pack;

	/** Explicitly given alignment of the structure in memory, the value of __declspec(align(n)) for struct */
 	int m_explicitAlign;

	/** Is the structure for arg stack storage. */
	bool m_isArgStack;

	/** Effective alignment of the structure in memory */
 	int m_align;


	string buildTypeSignature(const string &pointerSig);
	void parseBody(const string &body);
protected:
	void init();
};


class StructType: public RawType {
public:
	StructType(vector<NamedType> &items, int pack = 8, int explicitAlign = 0, bool isUnion = false, bool isArgStack = false);
	//incomplete struct def
	StructType(const string &tagName);
	void complete(vector<NamedType> &items, int pack = 8, int explicitAlign = 0, bool isUnion = false, bool isArgStack = false);

	virtual int getPBTypeClass() {return TC_STRUCT_TYPE; }
	virtual int getSizeOf() {return m_indexedStructType->getSizeOf(); }
	
	virtual int getItemOffset(int itemIndex) {return m_indexedStructType->getItemOffset(itemIndex); }
	inline vector<NamedType>& getItems() { return m_items; }
	inline vector<int>& getItemOffsets() { return m_indexedStructType->getItemOffsets(); }
	inline int getPack() { return m_indexedStructType->getPack(); }
	virtual string getTypeSignature(const string& pointerSig = "");
	virtual int getExplicitAlign() { return m_indexedStructType->getExplicitAlign(); }
	virtual int getAlign() { return m_indexedStructType->getAlign(); }
	virtual bool isComplete() { return m_indexedStructType != NULL; }
	virtual string getTagName() { return m_tagName; }
	virtual bool isCompatible(BaseType* type);
	virtual int getItemIndex(const string& itemName) ;
	virtual bool hasTChar() { return m_indexedStructType->hasTChar(); }

private:
	
	IndexedStructType *m_indexedStructType;

	/** Item names and types*/
	vector<NamedType> m_items;
 	
	hash_map<string, int> m_itemNameMap;
	string m_tagName;

	string buildTypeSignature(const string &pointerSig);
	void init();
};
