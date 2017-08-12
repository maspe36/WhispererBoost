#define BOOST_PYTHON_STATIC_LIB
#include "../../../include/Core/Game/Card.h"
#include "../../../include/Core/Game/Player.h"
#include "../../../include/Core/Utility/Action.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace boost::python;

typedef vector<int> vectorInt;
typedef vector<Card::MechanicVariants> vectorMecanics;


void Card::Effect()
{
	pyCard.attr("Effect")();
}

void Card::IsEffectTriggered(Action action)
{
	pyCard.attr("IsEffectTriggered")(action);
}

Card::Card(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, Player* owner)
	: Cost(cost), Name(name), Text(text), Color(color), Type(type), Mechanics(mechanics), Alive(true), Owner(owner), pyCard(nullptr)
{
}

Card::Card(boost::python::object card)
	: pyCard(card)
{
	Cost = extract< vectorInt >(pyCard.attr("Cost"));
	Name = extract< string >(pyCard.attr("Name"));
	Text = extract< string >(pyCard.attr("Text"));
	Color = extract< ColorVariants >(pyCard.attr("Color"));
	Type = extract< TypeVariants >(pyCard.attr("Type"));
	Mechanics = extract< vectorMecanics >(pyCard.attr("Mechanics"));
	Alive = extract< bool >(pyCard.attr("Alive"));
	Owner = extract< Player* >(pyCard.attr("Owner"));
}

Card::~Card()
{
}

BOOST_PYTHON_MODULE(Card)
{
	class_<vectorInt>("vectorInt")
		.def(vector_indexing_suite<vectorInt>());
	class_<vectorMecanics>("vectorMecanics")
		.def(vector_indexing_suite<vectorMecanics>());

	enum_<Card::ColorVariants>("ColorVariants")
		.value("Black", Card::ColorVariants::Black)
		.value("Blue", Card::ColorVariants::Blue)
		.value("Brown", Card::ColorVariants::Brown)
		.value("Green", Card::ColorVariants::Green)
		.value("Red", Card::ColorVariants::Red)
		.value("White", Card::ColorVariants::White);

	enum_<Card::TypeVariants>("TypeVariants")
		.value("Constant", Card::TypeVariants::Constant)
		.value("Creature", Card::TypeVariants::Creature)
		.value("Spell", Card::TypeVariants::Spell);

	enum_<Card::MechanicVariants>("MechanicVariants")
		.value("Buff", Card::MechanicVariants::Buff)
		.value("Continuous", Card::MechanicVariants::Continuous)
		.value("Debuff", Card::MechanicVariants::Debuff)
		.value("Downfall", Card::MechanicVariants::Downfall)
		.value("InHand", Card::MechanicVariants::InHand)
		.value("Introduce", Card::MechanicVariants::Introduce)
		.value("None", Card::MechanicVariants::None)
		.value("OnDiscard", Card::MechanicVariants::OnDiscard)
		.value("OnDraw", Card::MechanicVariants::OnDraw)
		.value("Rush", Card::MechanicVariants::Rush)
		.value("Statis", Card::MechanicVariants::Statis)
		.value("Trigger", Card::MechanicVariants::Trigger)
		.value("TurnEnd", Card::MechanicVariants::TurnEnd)
		.value("TurnStart", Card::MechanicVariants::TurnStart);

	class_<Card>("Card", init<vectorInt, string, string, Card::ColorVariants, Card::TypeVariants, vectorMecanics, Player*>())
		.def_readwrite("Cost", &Card::Cost)
		.def_readwrite("Name", &Card::Name)
		.def_readwrite("Text", &Card::Text)
		.def_readwrite("Color", &Card::Color)
		.def_readwrite("Type", &Card::Type)
		.def_readwrite("Mechanics", &Card::Mechanics)
		.def_readwrite("Alive", &Card::Alive)
		.def_readwrite("Owner", &Card::Owner);
}