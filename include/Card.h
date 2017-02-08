#pragma once
#include <vector>

using namespace std;

class Card
{
public:
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

	// A cards color
	enum class ColorVariants
	{
		Black,
		Blue,
		Brown,
		Green,
		Red,
		White
	} Color;

	// The card type
	enum class TypeVariants
	{
		Constant,
		Creature,
		Spell
	} Type;

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

	vector<MechanicVariants> Mechanics;

	bool Alive;

	#pragma endregion

	#pragma region Virtual Methods

	virtual void Effect();

	#pragma endregion

	#pragma region Constructor & Destructor

	Card(vector<int> Cost, string Name, string Text, ColorVariants Color, TypeVariants Type, vector<MechanicVariants> Mechanics);
	virtual ~Card();

	#pragma endregion
};