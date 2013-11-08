#include "../stdafx.h"

#include "StructType.h"
#include "TypeFactory.h"
#include "../Core.h"
#include "../PBException.h"

IndexedStructType::IndexedStructType(vector<Type> &itemTypes, int pack, int explicitAlign, bool isUnion, bool isArgStack) 
	: RawType("", sizeof(DWORD) /* sizeof empty struct */)
	, m_itemTypes(itemTypes)
	, m_pack(pack)
	, m_isUnion(isUnion)
	, m_explicitAlign(explicitAlign)
	, m_isArgStack(isArgStack)
{
	init();
}


IndexedStructType::IndexedStructType(const string& body, int pack, int explicitAlign, bool isUnion, bool isArgStack)
	: RawType("", sizeof(DWORD) /* sizeof empty struct */)
	, m_pack(pack)
	, m_isUnion(isUnion)
	, m_explicitAlign(explicitAlign)
	, m_isArgStack(isArgStack)
{
	parseBody(body);
	init();
}
bool IndexedStructType::hasTChar()
{
	for (size_t i = 0; i < m_itemTypes.size(); i ++) {
		if (m_itemTypes[i]->hasTChar())
			return true;
	}
	return false;
}


StructType::StructType(vector<NamedType> &items, int pack, int explicitAlign, bool isUnion, bool isArgStack) 
	: RawType("", sizeof(DWORD) /* sizeof empty struct */)
	, m_items(items)
	, m_indexedStructType((IndexedStructType*) CoreInstance.registerType(Type(
		new IndexedStructType(NamedType::getTypes(items), pack, explicitAlign, isUnion, isArgStack)
		)).get())
	
{
	init();
}

StructType::StructType(const string &tagName) 
	: RawType("", sizeof(DWORD) /* sizeof empty struct */)
	, m_indexedStructType(NULL)
	, m_tagName(tagName)
{
	int i = 1;
}
 
void StructType::init()
{
	for (size_t i = 0; i < m_items.size(); i ++) {
		m_itemNameMap[m_items[i].name] = i + 1;
	}
}



bool StructType::isCompatible(BaseType* type) 
{ 
	return type != NULL && 
		type->getPBTypeClass() == getPBTypeClass() &&
		m_indexedStructType == ((StructType*)type)->m_indexedStructType
		;
}


void StructType::complete(vector<NamedType> &items, int pack, int explicitAlign, bool isUnion, bool isArgStack) 
{
	m_items = items;
	m_indexedStructType = (IndexedStructType*) CoreInstance.registerType(Type(
		new IndexedStructType(NamedType::getTypes(items), pack, explicitAlign, isUnion, isArgStack)
		)).get();
	init();
	dprintf("StructType.this=%p, m_items.size()=%i, m_indexedStructType=%p\n", this, m_items.size(), m_indexedStructType);
}

void IndexedStructType::parseBody(const string &body)
{
	TypeFactory &tf = TypeFactory::getInstance();
	m_itemTypes.clear();
	size_t pos, prev = 0;
	while ((pos = body.find(";", prev)) != string::npos) {
		m_itemTypes.push_back(tf.createType(body.substr(prev, pos - prev)));
		prev = pos + 1;
	}
	if (prev < body.size()) { // no ; after the last item
		m_itemTypes.push_back(tf.createType(body.substr(prev, body.size() - prev)));
	}
}

string IndexedStructType::buildTypeSignature(const string &pointerSig)
{
	string sig;
	if (m_isArgStack) {
		sig += "__argstack ";
	}
	else {
		if (m_explicitAlign > 0) {
			sig += "__align(";
			sig += CUtil::toString(m_explicitAlign);
			sig += ") ";
		}
		if (m_pack != 8) {
			sig += "__pack(";
			sig += CUtil::toString(m_pack);
			sig += ") ";
		}
	}
	sig += isUnion() ? "_union {" : "_struct {";
	//sig += "struct {";
	string sep;
	for (size_t i = 0; i < m_itemTypes.size(); i ++) {
		sig += sep + m_itemTypes[i]->getTypeSignature();
		sep = ", ";
	}
	sig += "}";
	sig += pointerSig;

	return sig;
}

string IndexedStructType::getTypeSignature(const string& pointerSig)
{
	return buildTypeSignature(pointerSig);
}


void IndexedStructType::init()
{
	dprintf("+StructType::init\n");
	size_t i;

	// calc struct align
	vector<int> itemAlign;
	itemAlign.resize(m_itemTypes.size(), 0);
	
	if (m_isArgStack) {
		m_pack = 4;
	}
	int pack = m_pack;
	if (m_explicitAlign > 0) {
		if (pack < m_explicitAlign) {
			pack = m_explicitAlign;
		}
		m_align = m_explicitAlign;
	}
	else {
		m_align = 1;
	}

	for (i = 0; i < m_itemTypes.size(); i ++) {
		if (!m_itemTypes[i]->isComplete())
			throw RuntimeException("Can't use structure or union " + m_itemTypes[i]->getTagName() +" while it's being defined");

		int itemExplAlign = m_isArgStack ? 4 : m_itemTypes[i]->getExplicitAlign();
		if (itemExplAlign > 0) {
			if (itemAlign[i] == 0) {
				itemAlign[i] = itemExplAlign;
			}
			if (pack < itemExplAlign) {
				pack = itemExplAlign;
			}
		}
		
		if (itemAlign[i] == 0) {
			itemAlign[i] = m_itemTypes[i]->getAlign();
		}
		
		if (m_align < itemAlign[i]) {
			m_align = itemAlign[i];
		}
	}
	if (m_align > pack) {
		m_align = pack;
	}

	// calc offsets
	m_itemOffsets.resize(m_itemTypes.size());
	int currentOffset = 0;
	m_sizeOf = 0;
	for (i = 0; i < m_itemTypes.size(); i ++) {
		dprintf(" field %i, type=%s\n", i, m_itemTypes[i]->getTypeSignature().c_str());
		//apply alignment
		currentOffset = itemAlign[i] * (int)((currentOffset + itemAlign[i] - 1) / itemAlign[i]);
		m_itemOffsets[i] = currentOffset;
		size_t size = m_itemTypes[i]->getSizeOf();
		dprintf(" size=%i, offset=%i\n", size, m_itemOffsets[i]);
		if (m_isUnion) {
			if (m_sizeOf < size) {
				m_sizeOf = size;
			}
		}
		else {
			currentOffset += size;
			m_sizeOf = currentOffset;
		}
	}
	// adjust struct size by applying align
	m_sizeOf = m_align * (int)((m_sizeOf + m_align - 1) / m_align);
	dprintf(" sizeof(struct)=%i, alignof(struct)=%i\n", m_sizeOf, m_align);

	dprintf(" sig=%s\n", getTypeSignature().c_str());
	dprintf("-StructType::init\n");

}

BOOL PBCALL str_getTypeData1(StructType* structType, int &itemCount)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		itemCount = structType->getItems().size();
		return true;
	}
	catch (...) {
		return false;
	}
}

BOOL PBCALL str_getTypeData2(StructType* structType, BaseType* itemTypes[], int itemTypeClasses[], int itemOffsets[], size_t itemCount)
{
	PBExceptionStorageInstance.clearLastExceptionMessage();
	try {
		vector<NamedType> &items = structType->getItems();
		if (itemCount > items.size())
			itemCount = items.size();
		for (size_t i = 0; i < itemCount; i ++) {
			itemTypes[i] = items[i].type.get();
			itemTypeClasses[i] = itemTypes[i]->getPBTypeClass();
			itemOffsets[i] = structType->getItemOffset(i);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

string StructType::getTypeSignature(const string& pointerSig)
{
	return buildTypeSignature(pointerSig);
}

//struct R {
//	R s;
//};
string StructType::buildTypeSignature(const string &pointerSig)
{
	string sig;
	if (m_indexedStructType == NULL ){
		sig += m_tagName;
	}
	else {
		if (m_indexedStructType->isArgStack()) {
			sig += "__argstack ";
		}
		else {
			if (m_indexedStructType->getExplicitAlign() > 0) {
				sig += "__align(";
				sig += CUtil::toString(m_indexedStructType->getExplicitAlign());
				sig += ") ";
			}
			if (m_indexedStructType->getPack() != 8) {
				sig += "__pack(";
				sig += CUtil::toString(m_indexedStructType->getPack());
				sig += ") ";
			}
		}
		string sep;
		sig += m_indexedStructType->isUnion() ? "union {" : "struct {";
		//sig += "struct {";
		for (size_t i = 0; i < m_items.size(); i ++) {
			sig += sep + m_items[i].type->getTypeSignature() + " " + m_items[i].name;
			sep = ", ";
		}
		sig += "}";
	}
	sig += pointerSig;

	return sig;
}

int StructType::getItemIndex(const string& itemName)
{
	int index = m_itemNameMap[itemName];
	if (index == 0)
		throw RuntimeException("No struct/union field with name '" + itemName + "'");
	return index - 1; // 1-based are stored, but 0-based are needed.
}

