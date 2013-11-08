#pragma once

#include "Runtime.h"

class PBExceptionStorage: public ExecutionContext {
public:
	static PBExceptionStorage& getInstance();
private:
	PBExceptionStorage() {}
	static PBExceptionStorage * m_instance;
};


#define PBExceptionStorageInstance PBExceptionStorage::getInstance()
