#include "VMFunction.h"



VMFunction::VMFunction(const string& name, size_t parametersCount, void* functionPtr) :
	m_name(name),
	m_parametersCount(parametersCount),
	m_functionPtr(functionPtr)
{
}


VMFunction::~VMFunction()
{
}
