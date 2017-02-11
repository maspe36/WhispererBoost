#include "Core\Derived\Constant.h"

Constant::Constant(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics)
	: Card(cost, name, text, color, type, mechanics)
{
}

Constant::~Constant()
{
}
