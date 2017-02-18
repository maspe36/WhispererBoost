#include "..\include\Core\Player.h"

void Player::Draw()
{
}

void Player::Draw(int amount)
{
}

void Player::Shuffle()
{
}

void Player::RefillMana()
{
}

Player::Player(string name, vector<Card*> deck)
	: Name(name), Deck(deck), Health(30), Mana(1), TotalMana(1), Alive(true)
{
}

Player::~Player()
{
}
