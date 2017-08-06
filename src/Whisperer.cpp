#include "../include/Core/Game/Game.h"
#include "../include/Core/Game/Player.h"
#include "../include/Network/Server.h"

#include <string>
#include "boost/thread.hpp"

int main()
{
	try
	{
		std::cout << "Initializing io_service..." << std::endl;
		boost::asio::io_service io_service;
		std::cout << "Starting server..." << std::endl;
		Server* server = new Server(io_service, 8888);
		std::cout << "Server started!" << std::endl;

		// Run the blocking io_service method run()
		boost::thread ioThread(
			boost::bind(&boost::asio::io_service::run, &io_service));
		
		// Consistently try to find games.
		while (true)
		{
			if (server->MatchQueue.size() >= 2) 
			{
				// Get the two players who have been in queue the longest.
				std::cout << "Found 2 players in queue!" << std::endl;

				std::cout << server->MatchQueue.size() << " players in queue!" << std::endl;

				// Remove the first 2 players and check if they are still connected.
				Player* firstPlayer = server->MatchQueue.front();
				std::cout << firstPlayer->Name << " was selected for a game..." << std::endl;
				server->MatchQueue.pop();

				Player* secondPlayer = server->MatchQueue.front();
				std::cout << secondPlayer->Name << " was selected for a game..." << std::endl;
				server->MatchQueue.pop();

				std::cout << server->MatchQueue.size() << " players in queue!" << std::endl;

				std::cout << "Starting game between " << firstPlayer->Name << " and " << secondPlayer->Name << "..." << std::endl;

				Game* newGame = new Game({ firstPlayer, secondPlayer }, server);
				newGame->Start();
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

    return 0;
}
