#define BOOST_PYTHON_STATIC_LIB
#include "../../../include/Core/Derived/Spell.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


Spell::Spell(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, Player* owner)
	: Card(cost, name, text, color, type, mechanics, owner)
{
}

Spell::~Spell()
{
}

BOOST_PYTHON_MODULE(Spell)
{
	using namespace boost::python;

	typedef vector<int> vectorInt;
	typedef vector<Card::MechanicVariants> vectorMecanics;

	class_<vectorInt>("vectorInt")
		.def(vector_indexing_suite<vectorInt>());
	class_<vectorMecanics>("vectorMecanics")
		.def(vector_indexing_suite<vectorMecanics>());
	
	class_<Spell>("Spell", init<vectorInt, string, string, Card::ColorVariants, Card::TypeVariants, vectorMecanics, Player*>())
		.def_readwrite("Cost", &Spell::Cost)
		.def_readwrite("Name", &Spell::Name)
		.def_readwrite("Text", &Spell::Text)
		.def_readwrite("Color", &Spell::Color)
		.def_readwrite("Type", &Spell::Type)
		.def_readwrite("Mechanics", &Spell::Mechanics)
		.def_readwrite("Alive", &Spell::Alive)
		.def_readwrite("Owner", &Spell::Owner);
}
