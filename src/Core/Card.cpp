#include "..\include\Core\Card.h"

void Card::Effect()
{
}

Card::Card(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics)
	: Cost(cost), Name(name), Text(text), Color(color), Type(type), Mechanics(mechanics), Alive(true)
{
}

Card::~Card()
{
}
