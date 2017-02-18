#pragma once
#include <vector>

using namespace std;

class Player;

class Game
{
public:
	#pragma region Constants

	// The starting health for all players
	const int PLAYER_HEALTH;

	// The starting mana for all players
	const int PLAYER_MANA;

	#pragma endregion

	#pragma region InstanceVars

	// The players participating in the game
	vector<Player*> Players;

	#pragma endregion

	#pragma region Methods	

	#pragma endregion

	#pragma region Constructors & Destructor

	/* Create a new game and set all players health and mana to the game settings */
	Game(vector<Player*> players);

	/* Deconstructor for a game */
	~Game();

	#pragma endregion
};

