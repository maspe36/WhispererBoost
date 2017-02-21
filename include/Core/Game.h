#pragma once
#include <vector>

using namespace std;

class Player;
class Action;
class Card;

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

	// Order of cards played in current game
	vector<Card*> CardOrder;

	// Game history
	vector<Action*> History;

	// Pointer to the active player 
	Player* Active;

	// Index in Players to the active player
	int ActiveIndex;

	#pragma endregion

	#pragma region Methods	

	/* Starts the game loop */
	void Start();

	/* Players decide what cards to mulligan */
	void MulliganState();

	/* Listens for the active players actions (play cards, attack, etc)  */
	void PlayState();

	/* Do things at the start of the active players turn */
	void StartTurn();

	/* Do things at the end of the active players turn */
	void EndTurn();

	/* Change the active player to the next one */
	void ChangeActivePlayer();

	/* Sweep and clear all dead cards in the game */
	void ClearDeadCards();

	/* Sweep and clear all dead players in the game */
	void ClearDeadPlayers();

	/* Look at all the effects of cards in CardOrder and potentially trigger them */
	void CheckEffects(Action* action);

	/* Check if the game is over */
	bool IsGameOver();

	#pragma endregion

	#pragma region Constructors & Destructor

	/* Create a new game and set all players health and mana to the game settings */
	Game(vector<Player*> players);

	/* Deconstructor for a game */
	~Game();

	#pragma endregion
};

