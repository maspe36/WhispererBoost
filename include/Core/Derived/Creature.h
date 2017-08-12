#pragma once
#include "../Game/Card.h"

class Creature :
	public Card
{
public:
	#pragma region Instance Vars

	/* A creatures original Attack */ 
	int OriginalAttack;

	/* A creatures original Defense */
	int OriginalDefense;

	/* A creatures base attack (From buffs, debuffs, etc) */
	int BaseAttack;

	/* A creatures base defense (From buffs, debuffs, etc) */
	int BaseDefense;

	/* A creatures current attack */
	int Attack;

	/* A creatures current defense */
	int Defense;

	/* The "summoning sickness" of a creature. By default this is 1 */
	int Stasis;

	/* The number of attacks this creature has left this turn */
	int Flurry;

	/* The maximum number of attacks this creature can make per turn */
	int OriginalFlurry;

	/* A flag to determine if this creature can attack */
	bool CanAttack;

	#pragma endregion

	#pragma region Methods

	/* Attack a creature */
	void Attacking(Creature* creature);

	/* Attack a player */
	void Attacking(Player* player) const;

	/* Return true if this card is dead */
	bool IsDead() const;

	#pragma endregion 

	#pragma region Constructor & Destructor

	/* The constructor for the derived creature class */
	Creature(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, Player* owner, int attack, int defense);

	/* The virtual destructor for the derived creature class */
	virtual ~Creature();

	#pragma endregion
};

