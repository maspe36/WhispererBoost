#pragma once
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

class Client
	: public boost::enable_shared_from_this<Client>
{
public:
	#pragma region Types

	/* Defines a 'pointer' type that uses a shared boost pointer */
	typedef boost::shared_ptr<Client> pointer;

	#pragma endregion

	#pragma region Instance Vars

	/* The delimeter we append to the end of a message for the client to know we are done writing */
	const std::string delimeter = "#";

	/* The stream buffer this client will read from */
	boost::asio::streambuf sbuffer;

	#pragma endregion

	#pragma region Public Methods

	/* Returns a pointer of a new Connection instance with the appropriate io_service. */
	static pointer Create(boost::asio::io_service& ioService);

	/* Returns the socket used in the connection */
	boost::asio::ip::tcp::socket& GetSocket();

	/* Method to be called when we know the connection is good.
	Right now it simply writes hello world to the new client asynchronously */
	void Start();

	/* Writes the given string as a byte stream, accepts any size string */
	void Write(std::string data);

	/* Method to be called when the connection closes, and handles it cleanly */
	void DoClose();

	#pragma endregion

private:

	#pragma region Private Instance Vars

	/* The socket our client connects on */
	boost::asio::ip::tcp::socket socket;

	#pragma endregion

	#pragma region PrivateMethods

	/* Method to be called after we recieve word back from the client */
	void OnReceive(const boost::system::error_code& erorCode);

	/* Method to be called after we write to the client */
	void OnWrite(const boost::system::error_code& errorCode, size_t bytesTransferred);

	#pragma endregion

	#pragma region Private Constructor

	/* Creates a new client and returns a pointer */
	Client(boost::asio::io_service& ioService);

	#pragma endregion
};

