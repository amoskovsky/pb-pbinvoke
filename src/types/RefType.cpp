#include "../stdafx.h"

#include "RefType.h"

#include "../PBException.h"

RefType::RefType(Type &dataType)
	: PointerType(dataType, "&")
{

}




