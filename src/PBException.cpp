#include "stdafx.h"

#include "PBException.h"

PBExceptionStorage* PBExceptionStorage::m_instance = NULL;

PBExceptionStorage& PBExceptionStorage::getInstance()
{
	if (m_instance == NULL) {
		m_instance = new PBExceptionStorage();
	}
	return *m_instance;
}

