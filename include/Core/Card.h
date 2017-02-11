#pragma once
#include <vector>

using namespace std;

class Card
{
public:
	#pragma region Enums

	// Different colors available for a card
	enum class ColorVariants
	{
		Black,
		Blue,
		Brown,
		Green,
		Red,
		White
	};

	// Different types of cards
	enum class TypeVariants
	{
		Constant,
		Creature,
		Spell
	};

	// Different possible "mechanics" or types of effects
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

	/* The mana cost to play this card (Alphabetical)
	0: Black
	1: Blue
	2: Brown
	3: Green
	4: Red
	5: White */
	vector<int> Cost;

	// A cards name
	string Name;

	// The wording on a card
	string Text;

	// Color of the card
	ColorVariants Color;

	// Type of the card
	TypeVariants Type;

	// The different mechanics a card has
	vector<MechanicVariants> Mechanics;

	// When false the card is considered dead
	bool Alive;

	#pragma endregion

	#pragma region Virtual Methods

	virtual void Effect();

	#pragma endregion

	#pragma region Constructor & Destructor

	Card(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics);
	virtual ~Card();

	#pragma endregion
};