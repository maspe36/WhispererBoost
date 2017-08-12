#pragma once
#include <string>
#include "../Game/Card.h"

class Factory
{
public:
	#pragma region Instance Vars

	/* The folder the card scripts are stored in */
	std::string moduleFolder;

	/* Delimeter to split the data string on */
	char delimeter;

	#pragma endregion

	#pragma region Public Methods

	/* Return a Card* from the name of a python script, requires the python instance the class constructor creates */
	Card* GetCard(std::string name);

	/* Return a vector<Card*> from a vector of python script names, requires the python instance the class constructor creates */
	vector<Card*> GetDeck(std::string cards);

	#pragma endregion

	#pragma region Constructor & Destructor

	/* Default constructor */
	Factory();

	/* Default destructor */
	virtual ~Factory();

	#pragma endregion
};