#pragma once
#include "Card.h"
#include "boost/function.hpp"

#include <map>
#include <functional>

using namespace std;

// A helper class to register a factory function
class Registrar {
public:
	Registrar(string className, boost::function<Card*()> classFactoryFunction);
};

// A preprocessor define used by derived classes
#define REGISTER_CLASS(NAME, TYPE) static Registrar registrar(NAME, [](void) -> Card * { return new TYPE();});

// The factory - implements singleton pattern!
class Factory
{
public:
	// Get the single instance of the factory
	static Factory * Instance();

	// register a factory function to create an instance of className
	void RegisterFactoryFunction(string name, boost::function<Card*()> classFactoryFunction);

	// create an instance of a registered class
	Card* Create(string name);

private:
	// a private ctor
	Factory() {}

	// the registry of factory functions
	map<string, boost::function<Card*()>> factoryFunctionRegistry;

};