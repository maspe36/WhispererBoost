#pragma once
#include "../Game/Card.h"

class Constant :
	public Card
{
public:
	#pragma region Constructor & Destructor

	/* The constructor for the derived constant class */
	Constant(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics);

	/* The virtual destructor for the derived constant class */
	virtual ~Constant();

	#pragma endregion
};

