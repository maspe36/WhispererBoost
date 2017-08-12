#define BOOST_PYTHON_STATIC_LIB
#include "../../../include/Core/Utility/Factory.h"
#include "../../../include/Core/Game/Card.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include <boost/python.hpp>

using namespace boost::python;

template<typename Out>
void split(const std::string &data, char delim, Out result)
{
	std::stringstream stringStream;
	stringStream.str(data);
	std::string item;

	while (std::getline(stringStream, item, delim))
	{
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &data, char delim)
{
	std::vector<std::string> elements;
	split(data, delim, std::back_inserter(elements));
	return elements;
}

// ReSharper disable CppMemberFunctionMayBeConst
Card* Factory::GetCard(std::string name)
// ReSharper restore CppMemberFunctionMayBeConst
{
	try
	{
		std::string pyModule = moduleFolder + "." + name;
		object derived = import(str(pyModule)).attr(str(name))();

		Card* card = new Card(derived);
		return card;
	}
	catch (...)
	{
		PyErr_Print();
	}

	return nullptr;
}

vector<Card*> Factory::GetDeck(std::string cards)
{
	vector<string> items = split(cards, delimeter);
	vector<Card*> cCards;

	for (auto card : items)
	{
		cCards.push_back(GetCard(card));
	}

	return cCards;
}

Factory::Factory()
	: moduleFolder("cards"), delimeter('\n')
{
	Py_Initialize();
}

Factory::~Factory()
{
	Py_Finalize();
}
