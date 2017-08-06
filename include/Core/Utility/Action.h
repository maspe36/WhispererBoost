#pragma once
#include <vector>

class Player;
class Card;
class Creature;

class Action
{
public:
	#pragma region Enums

	/* All possible types of actions */
	enum ActionTypeVariants
	{
		Attack,
		Damage,
		Heal,
		Buff,
		Debuff,
		Draw,
		Discard,
		Summon,
		Destroy,
		TurnStart,
		TurnEnd
	};

	#pragma endregion

	#pragma region Instance Vars

	/* A vector of all the cards affected */
	std::vector<Card*> CardTargets;

	/* A vector of all the players affected */
	std::vector<Player*> PlayerTargets;

	/* The creature attacking */
	Creature* Attacker;

	/* The creature being attacked */
	Creature* Target;

	/* The player who owns the target creature */
	Player* TargetOwner;

	/* The player who caused this action to be created */
	Player* Owner;

	/* The type of action the object is */
	ActionTypeVariants Type;

	#pragma endregion

	#pragma region Constructors & Destructors
	
	/* Default constructor */
	Action();

	/* Generic actions like turnstart and end that don't specifically target */
	Action(Player* Owner, ActionTypeVariants ActionType);

	/* Only card targets */
	Action(std::vector<Card*> CardTargets, Player* Owner, ActionTypeVariants ActionType);

	/* Only player targets */
	Action(std::vector<Player*> PlayerTargets, Player* Owner, ActionTypeVariants ActionType);

	/* Attacking and defending creature */
	Action(Creature* Attacker, Creature* Target, Player* Owner, ActionTypeVariants ActionType);

	/* Attacking and defending player */
	Action(Creature* Attacker, Player* TargetOwner, Player* Owner, ActionTypeVariants ActionType);

	/* Card AND player targets */
	Action(std::vector<Card*> CardTargets, std::vector<Player*> PlayerTargets, Player* Owner, ActionTypeVariants ActionType);

	/* One for everything */
	Action(std::vector<Card*> CardTargets, std::vector<Player*> PlayerTargets, Creature* Attacker, Creature* Target, Player* TargetOwner, Player* Owner, ActionTypeVariants ActionType);

	/* Deconstructor for the card */
	~Action();
	
	#pragma endregion
};

