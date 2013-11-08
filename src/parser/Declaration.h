#pragma once

#include <boost/shared_ptr.hpp>

class IDeclaration
{
public:
	IDeclaration(void);
	virtual ~IDeclaration(void);
};

typedef boost::shared_ptr<IDeclaration> Declaration;
typedef vector<Declaration> Declarations;

