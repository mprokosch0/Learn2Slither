#include "test.hpp"

int main(void)
{
	try
	{
		Board	board(10, 10);
		while (1)
		{
			std::cout << board << std::endl;
			std::string input;
			std::cin >> input;
			Actions act;
			if (input == "w")
				act = Actions::FORWARD;
			else if (input == "a")
				act = Actions::LEFT;
			else if (input == "d")
				act = Actions::RIGHT;
			else
				continue ;
			double reward = board.snakeMove(act);
			std::cout << "[H[2J[3J" << std::endl;
			if (board.hasLost())
			{
				std::cout << "Game over !!" << std::endl;
				board.reset();
			}
			if (board.hasWon())
			{
				std::cout << "GG !!" << std::endl;
				board.reset();
			}
			std::cout << "state = " << Interpreter(board.getSnakeViews(), board.getSnakeSize(), reward).getState() << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}