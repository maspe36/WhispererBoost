#pragma once
#include "../Game/Card.h"

class Spell :
	public Card
{
public:
	#pragma region Constructor & Destructor

	/* The constructor for the derived spell class */
	Spell(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics);

	/* The virtual destructor for the derived spell class */
	virtual ~Spell();

	#pragma endregion
};

