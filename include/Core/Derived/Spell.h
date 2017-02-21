#pragma once
#include "..\include\Core\Card.h"

class Spell :
	public Card
{
public:
	#pragma region Constructor & Destructor

	Spell(int cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics);
	virtual ~Spell();

	#pragma endregion
};

