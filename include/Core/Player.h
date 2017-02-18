#pragma once
#include <vector>

using namespace std;

class Card;
class Creature;
class Spell;
class Constant;

class Player
{
public:
	#pragma region Instance Vars

	// A players name
	string Name;

	// A players health 
	int Health;

	// A players available mana
	int Mana;

	// A players total mana
	int TotalMana;

	// When false the player is considered dead
	bool Alive;

	// The cards in the players hand
	vector<Card*> Hand;

	// A players deck of cards
	vector<Card*> Deck;

	// A players graveyard
	vector<Card*> Graveyard;

	// A players creatures in play
	vector<Creature*> Creatures;

	// A players spells in play
	vector<Spell*> Spells;

	// A players constants in play
	vector<Constant*> Constants;

	// The seed used to shuffle this players cards 
	mt19937 Seed;

	#pragma endregion

	#pragma region Methods
	
	/* Draw a card from the players deck */
	void Draw();

	/* Draw given number of cards from the players deck */
	void Draw(int amount);

	/* Shuffle the players deck */
	void Shuffle();

	/* Refill the mana available to a player to their total mana */
	void RefillMana();

	#pragma endregion

	#pragma region Constructor & Destructor

	/* Create a new player with a given name and deck of cards */
	Player(string name, vector<Card*> deck);

	/* Deconstructor for a player */
	~Player();

	#pragma endregion
};

