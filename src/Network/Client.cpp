#include "../include/Network/Client.h"

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
	boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::OnReceive, shared_from_this(), boost::asio::placeholders::error));

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

void Client::OnWrite(const boost::system::error_code & errorCode, size_t bytesTransferred)
{
}

void Client::OnReceive(const boost::system::error_code & errorCode)
{
	if (errorCode == 0)
	{
		// Get the string from the client
		boost::asio::streambuf::const_buffers_type bufs = sbuffer.data();
		std::string data(
			boost::asio::buffers_begin(bufs),
			boost::asio::buffers_begin(bufs) + sbuffer.size());

		// Send the string minus the delimiter
		std::cout << "Client: " << data.substr(0, data.size() - 1) << std::endl;

		// Empty the stream buffer(?)
		sbuffer.consume(sbuffer.size());

		boost::asio::async_read_until(socket, sbuffer, delimeter.at(0), boost::bind(&Client::OnReceive, shared_from_this(), boost::asio::placeholders::error));
	}
	else
	{
		// We probably want to handle this differently in the future
		DoClose();
	}
}

Client::Client(boost::asio::io_service & ioService)
	: socket(ioService)
{
}
