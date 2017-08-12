#pragma once
#include <vector>

using namespace std;

class Player;
class Action;
class Card;
class Server;
class Factory;

class Game
{
public:
	#pragma region Constants & Protocols

	/* The starting health for all players */
	const int PLAYER_HEALTH;

	/* The starting mana for all players */
	const vector<int> PLAYER_MANA;

	/* The character that signifies a user attempting to play a card */
	const static char CARD_PROTOCOL = 'c';

	/* The character that signifies a user attempting to attack */
	const static char ATTACK_PROTOCOL = 'a';

	/* The character that signifies a user ending their turn */
	const static char END_PROTOCOL = 'e';

	/* The character that signifies a player */
	const static char PLAYER_PROTOCOL = 'p';

	#pragma endregion

	#pragma region InstanceVars

	/* The players participating in the game */
	vector<Player*> Players;

	/* Order of cards played in current game */
	vector<Card*> CardOrder;

	/* Game history */
	vector<Action*> History;

	/* Pointer to the active player */
	Player* ActivePlayer;

	/* Index in Players to the active player */
	int ActiveIndex;

	/* The server this game is from */
	Server* m_Server;

	Factory* m_Factory;

	#pragma endregion

	#pragma region Methods	

	/* Starts the game loop */
	void Start();

	/* The game state controller for mulligans */
	void MulliganState();

	/* Inner loop that actually handles a mulligan */
	void MulliganLoop(int done, vector<Player*> players) const;

	/* Listens for the active players actions (play cards, attack, etc)  */
	void PlayState();

	/* Do things at the start of the active players turn */
	void StartTurn() const;

	/* Do things at the end of the active players turn */
	void EndTurn();

	/* Handle how an attack will go through */
	void AttackPlay(std::string attack);

	/* Handle a card played from the active players hand at the given index */
	void CardPlay(int index) const;

	/* Handles the the play given for the active player */
	void HandlePlay(std::string play);

	/* Change the active player to the next one */
	void ChangeActivePlayer();

	/* Sweep and clear all dead cards in the game */
	void ClearDeadCards();

	/* Look at all the effects of cards in CardOrder and potentially trigger them */
	static void CheckEffects(Action* action);

	/* Write to only the connected players in this game */
	void WriteToPlayers(std::string data) const;
	
	/* Writes the game status to the clients */
	void WriteGameStatus();

	/* Check if the game is over */
	bool IsGameOver();

	#pragma endregion

	#pragma region Constructors & Destructor

	/* Create a new game and set all players health and mana to the game settings */
	explicit Game(vector<Player*> players, Server* server, Factory* factory);

	/* Deconstructor for a game */
	~Game();

	#pragma endregion

private:
	#pragma region Methods

	/* Return a substring of the given string after the given splitter */
	static std::string GetAfterChar(std::string data, char splitter);

	#pragma endregion 
};

