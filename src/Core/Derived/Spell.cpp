#include "Core/Derived/Spell.h"

Spell* Spell::GetSpell(Card * card)
{
	if (Spell* spell = dynamic_cast<Spell*>(card))
	{
		return spell;
	}
	return nullptr;
}

Spell::Spell(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics)
	: Card(cost, name, text, color, type, mechanics)
{
}

Spell::~Spell()
{
}
