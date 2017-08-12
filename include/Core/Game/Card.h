#pragma once
#define BOOST_PYTHON_STATIC_LIB

#include <vector>
#include <boost/python.hpp>

using namespace std;
using namespace boost::python;

class Player;
class Action;

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
	vector<int> Cost;

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

	/* A python card object */
	object pyCard;

	#pragma endregion

	#pragma region Virtual Methods

	/* Triggers the cards effect */
	virtual void Effect();

	/* Checks if the effect should be triggered */
	virtual void IsEffectTriggered(Action action);

	#pragma endregion

	#pragma region Constructor & Destructor

	/* Constructs the card */
	Card(vector<int> cost, string name, string text, ColorVariants color, TypeVariants type, vector<MechanicVariants> mechanics, Player* owner);

	/* Delegation constructor */
	explicit Card(object card);

	/* Deconstructor for the card */
	virtual ~Card();

	#pragma endregion
};