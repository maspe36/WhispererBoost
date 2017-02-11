#pragma once
#include "..\include\Core\Card.h"

class Constant :
	public Card
{
public:
	#pragma region Constructor & Destructor

	Constant(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics);
	virtual ~Constant();

	#pragma endregion
};

