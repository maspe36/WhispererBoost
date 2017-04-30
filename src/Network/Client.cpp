#include "../include/Network/Client.h"

#include "Core/Game.h"
#include "Core/Player.h"
#include "Core/Card.h"
#include "Network/Server.h"

#include <iostream>
#include <boost/bind/bind.hpp>

typedef boost::shared_ptr<Client> pointer;

pointer Client::Create(boost::asio::io_service & ioService)
{
	return pointer(new Client(ioService));
}

boost::asio::ip::tcp::socket & Client::GetSocket()
{
	return socket;
}

void Client::Start(Server* server)
{
	// This is the server we are connected to in this client.
	this->m_Server = server;

	Write("Connected!");

	boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::UserNameReceive, shared_from_this(), boost::asio::placeholders::error));

	std::cout << "Listening for messages from client..." << std::endl;
}

void Client::Write(std::string data)
{
	/* 
	The goal of this write is to write to this byte stream until the delemiter is reached. 
	This avoids clunky headers and we don't have to follow any protocol.
	*/

	// Add the delimiter to the string automagically
	data.append(delimeter);

	boost::asio::async_write(socket, boost::asio::buffer(data.c_str(), data.size()),
		boost::bind(&Client::OnWrite, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void Client::DoClose()
{
	std::cout << "Lost connection to client!" << std::endl;
	socket.close();
}

void Client::StartListening()
{
	Listening = true;
	boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::TurnListen, shared_from_this(), boost::asio::placeholders::error));
}

std::string Client::GetString(boost::asio::streambuf& sbuffer)
{
	boost::asio::streambuf::const_buffers_type bufs = sbuffer.data();
	std::string data(
		boost::asio::buffers_begin(bufs),
		boost::asio::buffers_begin(bufs) + sbuffer.size());

	// Return everything except the last character delimeter
	return data.substr(0, data.size() - 1);
}

void Client::UserNameReceive(const boost::system::error_code & errorCode)
{
	/* Create this clients equivlent player */
	if (errorCode == nullptr)
	{
		std::string data = GetString(sbuffer);

		// Empty the stream buffer
		sbuffer.consume(sbuffer.size());

		std::cout << "Client username: " << data << std::endl;

		m_Player = new Player(data, {}, shared_from_this());

		boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::DeckReceive, shared_from_this(), boost::asio::placeholders::error));
	}
	else
	{
		DoClose();
	}
}

void Client::DeckReceive(const boost::system::error_code & errorCode)
{
	/* Create this clients deck */
	if (errorCode == nullptr)
	{
		std::string data = GetString(sbuffer);

		// Empty the stream buffer
		sbuffer.consume(sbuffer.size());

		std::cout << m_Player->Name << " deck list: " << data << std::endl;

		// We crash right now because this stinker is empty, no factory yet.
		std::vector<Card*> cards;

		m_Player->Deck = cards;

		// Tell the server we are looking for a game.
		m_Server->MatchQueue.push(m_Player);

		std::cout << m_Player->Name << " is now in queue for a game!" << std::endl;
		boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::MulliganRecieve, shared_from_this(), boost::asio::placeholders::error));
	}
	else
	{
		DoClose();
	}
}

void Client::MulliganRecieve(const boost::system::error_code & errorCode)
{
	if (errorCode == nullptr)
	{
		std::string data = GetString(sbuffer);

		// Empty the stream buffer
		sbuffer.consume(sbuffer.size());

		// Print the string minus the delimiter
		std::cout << m_Player->Name << " kept: " << data << std::endl;

		// Pass their mulligan response to the server
		m_Player->Mulligan = data;
	}
	else
	{
		DoClose();
	}
}

void Client::TurnListen(const boost::system::error_code & errorCode)
{
	/* This makes a call back to the server with the input from the client and keeps listening until the player ends their turn. */
	if (errorCode == nullptr)
	{
		string data = GetString(sbuffer);

		// Empty the stream buffer
		sbuffer.consume(sbuffer.size());

		this->m_Player->CurrentGame->HandlePlay(data);

		// The client wants to end their turn, stop calling this method recursivly and just let it end
		if (data != "end")
		{
			// Handle the clients input for the game
			boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::TurnListen, shared_from_this(), boost::asio::placeholders::error));
		}
		else 
		{
			// Stop listening
			Listening = false;
		}
	}
	else
	{
		DoClose();
	}
}

void Client::OnWrite(const boost::system::error_code & errorCode, size_t bytesTransferred) const
{
	std::cout << "Sent " + std::to_string(bytesTransferred) + " bytes" << std::endl;
}

Client::Client(boost::asio::io_service & ioService)
	: m_Player(nullptr), m_Server(nullptr), Listening(false), socket(ioService)
{
}
