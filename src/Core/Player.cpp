#include "..\include\Core\Player.h"

#include "Core\Card.h"

#include <algorithm>
#include <random>

void Player::Draw()
{
	Card* temp = Deck.front();
	Deck.erase(Deck.begin());
	Hand.push_back(temp);
}

void Player::Draw(int amount)
{
	for (int i = 0; i < amount; i++) 
	{
		Card* temp = Deck.front();
		Deck.erase(Deck.begin());
		Hand.push_back(temp);
	}
}

void Player::Shuffle()
{
	shuffle(begin(Deck), end(Deck), Seed);
}

void Player::RefillMana()
{
	Mana = TotalMana;
}

Player::Player(string name, vector<Card*> deck)
	: Name(name), Deck(deck), Alive(true), Seed(random_device{}())
{
	// Set the owner to this for all cards in the players deck
	for (Card* card : Deck)
	{
		card->Owner = this;
	}
}

Player::~Player()
{
}
