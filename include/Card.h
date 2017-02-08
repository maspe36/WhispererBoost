#pragma once
#include <vector>

using namespace std;

class Card
{
public:
	// The mana cost to play this card, 
	// same order as the Color enum
	vector<int> Cost;

	// A cards name
	string Name;

	// The wording on a card
	string Text;

	// A cards color
	enum _Color 
	{
		_Black,
		_Blue,
		_Brown,
		_Green,
		_Red,
		_White
	} Color;

	// The card type
	enum _Type 
	{
		_Constant,
		_Creature,
		_Spell
	} Type;
};