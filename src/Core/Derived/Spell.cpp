#include "Core\Derived\Spell.h"

Spell::Spell(int cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics)
	: Card(cost, name, text, color, type, mechanics)
{
}

Spell::~Spell()
{
}