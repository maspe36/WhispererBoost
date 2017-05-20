#pragma once
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

class Player;
class Server;

class Client
	: public boost::enable_shared_from_this<Client>
{
public:
	#pragma region Types

	/* Defines a 'pointer' type that uses a shared boost pointer */
	typedef boost::shared_ptr<Client> pointer;

	#pragma endregion

	#pragma region Instance Vars

	/* The delimeter we append to the end of a message for the client to know we are done writing. */
	const std::string Delimeter = "\n";

	/* The stream buffer this client will read from. */
	boost::asio::streambuf Buffer;

	/* The game representation of this client. */
	Player* m_Player;

	/* The server this client is connected to. */
	Server* m_Server;

	/* A flag to block on if this client is currently listening. */
	bool Listening;

	/* A flag to quickly check if this client is connected and available to be written too. */
	bool Connected;

	#pragma endregion

	#pragma region Methods

	/* Returns a pointer of a new Connection instance with the appropriate io_service. */
	static pointer Create(boost::asio::io_service& ioService);

	/* Returns the socket used in the connection. */
	boost::asio::ip::tcp::socket& GetSocket();

	/* Method to be called when we know the connection is good. */
	void Start(Server* server);

	/* Writes the given string as a byte stream, accepts any size string. */
	void Write(std::string data);

	/* Method to be called when the connection closes, and handles it cleanly. */
	void DoClose();

	/* Starts listening again after TurnListen died out. */
	void StartListening();

	#pragma endregion

private:

	#pragma region Instance Vars

	/* The socket our client connects on */
	boost::asio::ip::tcp::socket Socket;

	#pragma endregion

	#pragma region Methods

	/* Returns the string from the buffer without the delimeter */
	static std::string GetString(boost::asio::streambuf& sbuffer);

	/* Method to be called after we recieve word back from the client for the first time, get their username. */
	void UserNameReceive(const boost::system::error_code& errorCode);

	/* Method to be called after we recieve word back from the client for the second time, get the deck they will use. */
	void DeckReceive(const boost::system::error_code& errorCode);

	/* Method to be called when we are listening for a mulligan response from this client */
	void MulliganRecieve(const boost::system::error_code& errorCode);

	/* Method to be called in the game loop */
	void TurnListen(const boost::system::error_code& errorCode);

	/* Method to be called after we write to the client */
	void OnWrite(const boost::system::error_code& errorCode, size_t bytesTransferred) const;
	
	#pragma endregion

	#pragma region Constructor

	/* Creates a new client and returns a pointer */
	explicit Client(boost::asio::io_service& ioService);

	#pragma endregion
};

