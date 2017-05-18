#pragma once
#include "Card.h"
#include "boost/function.hpp"

#include <map>
#include <functional>

using namespace std;

/* An internal helper class to register a factory function */
class Registrar {
public:
	Registrar(string className, boost::function<Card*()> classFactoryFunction);
};

/* A preprocessor macro to associate a string to a class type */
#define REGISTER_CLASS(NAME, TYPE) static Registrar registrar(NAME, [](void) -> Card * { return new TYPE();});

class Factory
{
public:
	#pragma region Instance Vars

	/* Get the single instance of the factory */
	static Factory * Instance();

	#pragma endregion 

	#pragma region Methods

	/* Register a factory function to create an instance of className */
	void RegisterFactoryFunction(string name, boost::function<Card*()> classFactoryFunction);

	/* Create an instance of a registered class */
	Card* Create(string name);

	/* Fill the given vector<Card*> with the instances of the cards in the decklist */
	static void FillDeck(vector<Card*>& deck, std::string decklist, std::string delimeter);

	#pragma endregion 

private:
	#pragma region Methods

	static vector<string> Split(const string& str, const string& delim);

	#pragma endregion 

	#pragma region Constructor

	/* Empty constructor for a factory */
	Factory();

	#pragma endregion 

	#pragma region Instance Variables

	/* The registry of type constructors */
	map<string, boost::function<Card*()>> factoryFunctionRegistry;

	#pragma endregion 
};