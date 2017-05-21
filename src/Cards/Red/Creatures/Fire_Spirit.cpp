#include "Cards/Red/Creatures/Fire_Spirit.h"
#include "Core/Factory.h"

REGISTER_CLASS("Fire Spirit", Fire_Spirit)

Fire_Spirit::Fire_Spirit()
	: Creature({ 0,0,0,0,1,0 }, "Fire Spirit", "A lonely spirit born from the inferno.", ColorVariants::Red, TypeVariants::Creature, { MechanicVariants::None }, 2, 1)
{
}

Fire_Spirit::~Fire_Spirit()
{
}
