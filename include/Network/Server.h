#pragma once
#include "Client.h"

class Player;

class Server
{
public:
	#pragma region Instance Vars

	/* All the connected clients */
	std::vector<Player*> clients;

	#pragma endregion

	#pragma region Methods

	/* Write the given string to a byte stream for all connected clients */
	void WriteToAll(std::string data);

	#pragma endregion

	#pragma region Constructor

	/* Start a server with an io_service, on the designated port  */
	Server(boost::asio::io_service& io_service, int Port);

	#pragma endregion

private:
	#pragma region Instance Vars

	/* The acceptor for TCP connections */
	boost::asio::ip::tcp::acceptor acceptor;

	#pragma endregion

	#pragma region Methods

	/* Creates a new connection object and waits for a connection asynchronously  */
	void Start();

	/*
	Method to be called on a connection completion.
	Checks to see if the connection is good, and if it is runs the start method.
	Then return to StartAccept() with another function call.
	*/
	void OnAccept(Client::pointer new_connection, const boost::system::error_code& error);

	/* Method to be called when the connection closes */
	void DoClose(Client::pointer connection);

	#pragma endregion
};

