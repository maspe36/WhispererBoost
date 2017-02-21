#pragma once
#include "..\include\Core\Card.h"

class Creature :
	public Card
{
public:
	#pragma region Instance Vars

	// A creatures original Attack
	int const OriginalAttack;
	// A creatures original Defense
	int const OriginalDefense;

	// A creatures base attack (From buffs, debuffs, etc)
	int BaseAttack;
	// A creatures base defense (From buffs, debuffs, etc)
	int BaseDefense;

	// A creatures current attack
	int Attack;
	// A creatures current defense
	int Defense;

	#pragma endregion

	#pragma region Constructor & Destructor

	Creature(int cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, int attack, int defense);
	virtual ~Creature();

	#pragma endregion
};

