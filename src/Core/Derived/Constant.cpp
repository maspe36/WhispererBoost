#define BOOST_PYTHON_STATIC_LIB
#include "../../../include/Core/Derived/Constant.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


Constant::Constant(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, Player* owner)
	: Card(cost, name, text, color, type, mechanics, owner)
{
}

Constant::~Constant()
{
}

BOOST_PYTHON_MODULE(Constant)
{
	using namespace boost::python;

	typedef vector<int> vectorInt;
	typedef vector<Card::MechanicVariants> vectorMecanics;

	class_<vectorInt>("vectorInt")
		.def(vector_indexing_suite<vectorInt>());
	class_<vectorMecanics>("vectorMecanics")
		.def(vector_indexing_suite<vectorMecanics>());

	class_<Constant>("Constant", init<vectorInt, string, string, Card::ColorVariants, Card::TypeVariants, vectorMecanics, Player*>())
		.def_readwrite("Cost", &Constant::Cost)
		.def_readwrite("Name", &Constant::Name)
		.def_readwrite("Text", &Constant::Text)
		.def_readwrite("Color", &Constant::Color)
		.def_readwrite("Type", &Constant::Type)
		.def_readwrite("Mechanics", &Constant::Mechanics)
		.def_readwrite("Alive", &Constant::Alive)
		.def_readwrite("Owner", &Constant::Owner);
}