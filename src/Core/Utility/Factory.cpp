#include "../../../include/Core/Utility/Factory.h"

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

void Factory::FillDeck(vector<Card*>& deck, std::string decklist, std::string delimeter)
{
	vector<std::string> cardlist = Split(decklist, delimeter);

	for (auto name : cardlist)
	{
		auto card = Instance()->Create(name);
		deck.push_back(card);
	}
}

vector<std::string> Factory::Split(const string & text, const string & delimeter)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;

	while ((end = text.find(delimeter, start)) != std::string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}

	tokens.push_back(text.substr(start));

	return tokens;
}

Factory::Factory()
{
}
