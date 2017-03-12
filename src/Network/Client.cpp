#include "../include/Network/Client.h"

#include "Core\Player.h"
#include "Core\Card.h"

typedef boost::shared_ptr<Client> pointer;

pointer Client::Create(boost::asio::io_service & ioService)
{
	return pointer(new Client(ioService));
}

boost::asio::ip::tcp::socket & Client::GetSocket()
{
	return socket;
}

void Client::Start()
{
	Write("Connected!");
	// Calls OnReceive when a message is recieved
	boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::OnFirstReceive, shared_from_this(), boost::asio::placeholders::error));

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

std::string Client::GetString(boost::asio::streambuf& sbuffer)
{
	boost::asio::streambuf::const_buffers_type bufs = sbuffer.data();
	std::string data(
		boost::asio::buffers_begin(bufs),
		boost::asio::buffers_begin(bufs) + sbuffer.size());

	// Return everything except the last character delimeter
	return data.substr(0, data.size() - 1);
}

void Client::OnFirstReceive(const boost::system::error_code & errorCode)
{
	/* Create this clients equivlent player */
	if (errorCode == 0)
	{
		std::string data = GetString(sbuffer);

		// Print the string minus the delimiter
		std::cout << "Client 1st Recieve: " << data << std::endl;

		// Example string...
		// maspe36#Hellhound; Foo; Bar;
		// Before the # is the name of the player, and after are the cards names seperated by ';'.

		std::string name;
		std::vector<Card*> cards;



		m_Player = new Player(name, cards, shared_from_this());

		// Empty the stream buffer
		sbuffer.consume(sbuffer.size());

		boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::OnReceive, shared_from_this(), boost::asio::placeholders::error));
	}
	else
	{
		// We probably want to handle this differently in the future
		DoClose();
	}
}

void Client::OnReceive(const boost::system::error_code & errorCode)
{
	if (errorCode == 0)
	{
		std::string data = GetString(sbuffer);

		// Print the string minus the delimiter
		std::cout << "Client: " << data << std::endl;

		// Empty the stream buffer
		sbuffer.consume(sbuffer.size());

		boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::OnReceive, shared_from_this(), boost::asio::placeholders::error));
	}
	else
	{
		// We probably want to handle this differently in the future
		DoClose();
	}
}

void Client::OnWrite(const boost::system::error_code & errorCode, size_t bytesTransferred)
{
}

Client::Client(boost::asio::io_service & ioService)
	: socket(ioService)
{
}
