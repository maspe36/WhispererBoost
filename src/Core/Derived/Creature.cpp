#include "Core\Derived\Creature.h"

Creature::Creature(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, int attack, int defense)
	: Card(cost, name, text, color, type, mechanics), 
	OriginalAttack(attack), OriginalDefense(defense),
	BaseAttack(attack), BaseDefense(defense),
	Attack(attack), Defense(defense)
{
}

Creature::~Creature()
{
}
