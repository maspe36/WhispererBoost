#include "Core/Derived/Creature.h"
#include "Core/Player.h"

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

Creature::Creature(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, int attack, int defense)
	: Card(cost, name, text, color, type, mechanics),
	  OriginalAttack(attack), OriginalDefense(defense),
	  BaseAttack(attack), BaseDefense(defense),
	  Attack(attack), Defense(defense), 
	  Stasis(1), Flurry(1), OriginalFlurry(1), CanAttack(false)
{
}

Creature::~Creature()
{
}
