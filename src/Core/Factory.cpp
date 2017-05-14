#include "Core/Factory.h"

Registrar::Registrar(string name, boost::function<Card*()> classFactoryFunction)
{
	// Register the class factory function
	Factory::Instance()->RegisterFactoryFunction(name, classFactoryFunction);
}


Factory * Factory::Instance()
{
	static Factory factory;
	return &factory;
}

void Factory::RegisterFactoryFunction(string name, boost::function<Card*()> classFactoryFunction)
{
	factoryFunctionRegistry[name] = classFactoryFunction;
}

Card* Factory::Create(string name)
{
	Card* instance = nullptr;

	// Find name in the registry and call the associated method
	auto it = factoryFunctionRegistry.find(name);
	if (it != factoryFunctionRegistry.end())
	{
		instance = it->second();
	}
		
	// Returns a nullptr if the associated method could not be found
	return instance;
}

Factory::Factory()
{
}
