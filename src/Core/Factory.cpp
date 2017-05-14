#include "Core/Factory.h"

Registrar::Registrar(string name, boost::function<Card*()> classFactoryFunction)
{
	// register the class factory function
	Factory::Instance()->RegisterFactoryFunction(name, classFactoryFunction);
}


Factory * Factory::Instance()
{
	static Factory factory;
	return &factory;
}

void Factory::RegisterFactoryFunction(string name, boost::function<Card*()> classFactoryFunction)
{
	// register the class factory function 
	factoryFunctionRegistry[name] = classFactoryFunction;
}

Card* Factory::Create(string name)
{
	Card* instance = nullptr;

	// find name in the registry and call factory method.
	auto it = factoryFunctionRegistry.find(name);
	if (it != factoryFunctionRegistry.end())
	{
		instance = it->second();
	}
		
	// wrap instance in a shared ptr and return
	if (instance != nullptr)
		return instance;
	else
		return nullptr;
}