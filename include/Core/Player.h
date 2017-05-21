#pragma once
#include <vector>
#include <random>

#include "Network/Client.h"

using namespace std;

class Game;
class Card;
class Creature;
class Spell;
class Constant;
class Client;

class Player
{
public:
	#pragma region Instance Vars

	/* A players name */
	string Name;

	/* A players health */
	int Health;

	/* A players available mana */
	vector<int> Mana;

	/* A players total mana */
	vector<int> TotalMana;

	/* When false the player is considered dead */
	bool Alive;

	/* The cards in the players hand */
	vector<Card*> Hand;

	/* The cards this player kept off the muligan */
	string Mulligan;

	/* A players deck of cards */
	vector<Card*> Deck;

	/* A players graveyard */
	vector<Card*> Graveyard;

	/* A players constants in play */
	vector<Constant*> Constants;

	/* A players creatures in play */
	vector<Creature*> Creatures;

	/* A players spells in play */
	vector<Spell*> Spells;

	/* The seed used to shuffle this players cards */
	mt19937 Seed;

	/* The game this player is currently in */
	Game* CurrentGame;

	/* The client interface for this player */
	Client::pointer m_Client;

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

	/* Plays a card into the appropriate area */
	void PlayCard(Card* card);

	/* Removes the given card from this players hand */
	void RemoveFromHand(Card* card);

	/* Subtracts the cards cost from this players available mana */
	void SubtractCost(Card * card);

	/* Adds the given cards color to the total mana */
	void AddMana(Card* card);

	/* Returns true if the card is playable */
	bool IsPlayable(Card* card) const;

	/* Returns a formatted string of the status of this player (Mana, cards in play, etc) */
	std::string GetStatus(bool sensitive);

	#pragma endregion

	#pragma region Constructor & Destructor

	/* Create a new player with a given name and deck of cards */
	Player(string name, vector<Card*> deck, Client::pointer client);

	/* Deconstructor for a player */
	~Player();

	#pragma endregion
};

