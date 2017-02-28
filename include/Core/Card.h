#pragma once
#include <vector>

using namespace std;

class Player;

class Card
{
public:
	#pragma region Enums

	/* Different colors available for a card */
	enum class ColorVariants
	{
		Black,
		Blue,
		Brown,
		Green,
		Red,
		White
	};

	/* Different types of cards */
	enum class TypeVariants
	{
		Constant,
		Creature,
		Spell
	};

	/* Different possible "mechanics" or types of effects */
	enum MechanicVariants
	{
		None,
		Buff,
		Debuff,
		Continuous,
		Downfall,
		InHand,
		Introduce,
		OnDiscard,
		OnDraw,
		Rush,
		Statis,
		Trigger,
		TurnStart,
		TurnEnd
	};

	#pragma endregion

	#pragma region Instance Vars

	/* The mana cost to play this card */
	int Cost;

	/* A cards name */
	string Name;

	/* The wording on a card */
	string Text;

	/* Color of the card */
	ColorVariants Color;

	/* Type of the card */
	TypeVariants Type;

	/* The different mechanics a card has */
	vector<MechanicVariants> Mechanics;

	/* When false the card is considered dead */
	bool Alive;

	/* The player that owns this card */
	Player* Owner;

	#pragma endregion

	#pragma region Virtual Methods

	/* Triggers the cards effect */
	virtual void Effect();

	#pragma endregion

	#pragma region Constructor & Destructor

	/* Constructs the card */
	Card(int cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics);

	/* Deconstructor for the card */
	virtual ~Card();

	#pragma endregion
};