#define BOOST_PYTHON_STATIC_LIB
#include "../../../include/Core/Derived/Creature.h"
#include "../../../include/Core/Game/Player.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


void Creature::Attacking(Creature* creature)
{
	// Make sure they can attack and aren't attacking an illegal target
	if (CanAttack && Owner != creature->Owner)
	{
		// Deal the damage of the attack
		creature->Defense -= Attack;
		Defense -= creature->Attack;

		// Check if the attacked creature died
		if (creature->IsDead())
		{
			creature->Alive = false;
		}

		// Check if this card died
		if (IsDead())
		{
			Alive = false;
		}
	}
}

void Creature::Attacking(Player* player) const
{
	// Make sure they can attack and aren't attacking an illegal target
	if (CanAttack && Owner != player)
	{
		// Deal the damage to the player
		player->Health -= Attack;

		// Check if they died
		if (player->IsDead())
		{
			player->Alive = false;
		}
	}
}

bool Creature::IsDead() const
{
	if (Defense <= 0)
	{
		return true;
	}

	if (!Alive)
	{
		return true;
	}

	return false;
}

Creature::Creature(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, Player* owner, int attack, int defense)
	: Card(cost, name, text, color, type, mechanics, owner),
	  OriginalAttack(attack), OriginalDefense(defense),
	  BaseAttack(attack), BaseDefense(defense),
	  Attack(attack), Defense(defense), 
	  Stasis(1), Flurry(1), OriginalFlurry(1), CanAttack(false)
{
}

Creature::~Creature()
{
}

BOOST_PYTHON_MODULE(Creature)
{
	using namespace boost::python;

	typedef vector<int> vectorInt;
	typedef vector<Card::MechanicVariants> vectorMecanics;

	class_<vectorInt>("vectorInt")
		.def(vector_indexing_suite<vectorInt>());
	class_<vectorMecanics>("vectorMecanics")
		.def(vector_indexing_suite<vectorMecanics>());

	class_<Creature>("Creature", init<vectorInt, string, string, Card::ColorVariants, Card::TypeVariants, vectorMecanics, Player*, int, int>())
		.def_readwrite("Cost", &Creature::Cost)
		.def_readwrite("Name", &Creature::Name)
		.def_readwrite("Text", &Creature::Text)
		.def_readwrite("Color", &Creature::Color)
		.def_readwrite("Type", &Creature::Type)
		.def_readwrite("Mechanics", &Creature::Mechanics)
		.def_readwrite("Alive", &Creature::Alive)
		.def_readwrite("Owner", &Creature::Owner)
		.def_readwrite("OriginalAttack", &Creature::OriginalAttack)
		.def_readwrite("OriginalDefense", &Creature::OriginalDefense)
		.def_readwrite("BaseAttack", &Creature::BaseAttack)
		.def_readwrite("BaseDefense", &Creature::BaseDefense)
		.def_readwrite("Attack", &Creature::Attack)
		.def_readwrite("Defense", &Creature::Defense)
		.def_readwrite("Stasis", &Creature::Stasis)
		.def_readwrite("Flurry", &Creature::Flurry)
		.def_readwrite("OriginalFlurry", &Creature::OriginalFlurry)
		.def_readwrite("CanAttack", &Creature::CanAttack);
}
