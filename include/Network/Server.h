#pragma once
#include "Client.h"

class Server
{
public:
	Server(boost::asio::io_service& io_service, int Port);
	std::vector<Client::pointer> clients;
	
	void WriteToAll(std::string data);
private:
	boost::asio::ip::tcp::acceptor acceptor;

	void Start();
	void OnAccept(Client::pointer new_connection, const boost::system::error_code& error);
	void DoClose(Client::pointer connection);
};

