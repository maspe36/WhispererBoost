#include "../include/Network/Server.h"

#include "Core/Player.h"

#include <iostream>
#include <boost/bind/bind.hpp>

void Server::WriteToAll(std::string data)
{
	for (auto i : m_Clients)
	{
		i->Write(data);
	}
}

Server::Server(boost::asio::io_service & io_service, int Port)
	: acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), Port))
{
	std::cout << "Listening for clients..." << std::endl;
	Start();
}

void Server::Start()
{
	Client::pointer NewClient =
		Client::Create(acceptor.get_io_service());

	// Calls OnAccept when a connection happens
	acceptor.async_accept(NewClient->GetSocket(),
		boost::bind(&Server::OnAccept, this, NewClient,
			boost::asio::placeholders::error));
}

void Server::OnAccept(Client::pointer newClient, const boost::system::error_code & error)
{
	if (!error)
	{
		newClient->Start(this);
		m_Clients.push_back(newClient);
		std::cout << "Client connected!" << std::endl;
	}
	// pesudo recursive
	Start();
}

void Server::DoClose(Client::pointer connection)
{
	std::cout << "Lost connection to client!" << std::endl;
	connection->GetSocket().close();
}
