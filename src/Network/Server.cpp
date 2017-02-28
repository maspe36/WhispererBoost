#include "../include/Network/Server.h"

/* Start a server with an io_service, on the designated port  */
Server::Server(boost::asio::io_service & io_service, int Port)
	: acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), Port))
{
	std::cout << "Listening for clients..." << std::endl;
	Start();
}

void Server::WriteToAll(std::string data)
{
	for (auto i : clients) 
	{
		i->Write(data);
	}
}

/* Creates a new connection object and waits for a connection asynchronously  */
void Server::Start()
{
	Client::pointer NewClient =
		Client::Create(acceptor.get_io_service());

	// Calls OnAccept when a connection happens
	acceptor.async_accept(NewClient->GetSocket(),
		boost::bind(&Server::OnAccept, this, NewClient,
			boost::asio::placeholders::error));
}

/* 
Method to be called on a connection completion.
Checks to see if the connection is good, and if it is runs the start method. 
Then return to StartAccept() with another function call.
*/
void Server::OnAccept(Client::pointer NewClient, const boost::system::error_code & error)
{
	if (!error)
	{
		clients.push_back(NewClient);
		NewClient->Start();
		std::cout << "Client connected!" << std::endl;
	}
	// pesudo recursive
	Start();
}

/* Method to be called when the connection closes */
void Server::DoClose(Client::pointer connection)
{
	std::cout << "Lost connection to client!" << std::endl;
	connection->GetSocket().close();
}
