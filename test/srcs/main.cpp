#include "test.hpp"

int main(void)
{
	try
	{
		Board	board(10, 10);
		Agent	ai;
		int		nbGames = 0;
		int		nbWin = 0;
		int		nbLoss = 0;
		ai.importModel("test.mdl");
		ai.setState(Interpreter(board.getSnakeViews(), board.getSnakeSize(),
				0, Actions::FORWARD, 0, 0).getState());
		while (nbGames < 50000)
		{
			while (1)
			{
				std::cout << "Game count = " << ai.getGamePlayed() << std::endl;
				std::cout << "Win ratio = " << ai.getWinRatio() * 100 << '%' << std::endl;
				std::cout << "Win count = " << nbWin << std::endl;
				std::cout << "Loss count = " << nbLoss << std::endl;
				std::cout << "Snake size = " << board.getSnakeSize() << std::endl;
				std::cout << board << std::endl;
				Actions act = ai.chooseAction();
				double reward = board.snakeMove(act);
				Interpreter t(board.getSnakeViews(), board.getSnakeSize(), reward, act,
					board.hasWon(), board.hasLost());
				ai.updateQTable(t);
				// if (gameCount >= 2000000)
				// 	usleep(400000);
				if (board.hasLost())
				{
					nbLoss++;
					break ;
				}
				if (board.hasWon())
				{
					nbWin++;
					break ;
				}
				std::cout << "[H[2J[3J" << std::endl;
			}
			nbGames++;
			ai.decayExploration();
			ai.incrementTotalGames(board.hasWon());
			board.reset();
			ai.setState(Interpreter(board.getSnakeViews(), board.getSnakeSize(),
				0, Actions::FORWARD, 0, 0).getState());
		}
		ai.exportModel("test.mdl");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}