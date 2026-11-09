#include "test.hpp"

int main(void)
{
	try
	{
		Board	board(10, 10);
		Agent	ai(0.05, 0.99, 0.9);
		int		nbGames = 0;
		int		nbWin = 0;
		int		nbLoss = 0;
		ai.importModel("0.9AppleDist0.3emptySquare4DangerSize.mdl");
		ai.setState(Interpreter(board.getSnakeViews(), board.getSnakeSize(),
				0, Actions::FORWARD, 0, 0).getState());
		ai.setTraining(false);
		int timeouts = 0;
		while (nbGames < 100000)
		{
			size_t moves = 0;
			while (1)
			{
				if (moves > 500)
				{
					std::cout << "Game count = " << ai.getGamePlayed() << std::endl;
					std::cout << "Session ratio = " << nbWin / (float)nbGames * 100 << '%' << std::endl;
					std::cout << "Win ratio = " << ai.getWinRatio() * 100 << '%' << std::endl;
					std::cout << "Win count = " << nbWin << std::endl;
					std::cout << "Loss count = " << nbLoss << std::endl;
					std::cout << "timeouts = " << timeouts << std::endl;
					std::cout << "Epsilon = " << ai.getExplRate() << std::endl;
					std::cout << "Snake size = " << board.getSnakeSize() << std::endl;
					std::cout << board << std::endl;
					std::cout << "[H[2J[3J";
					usleep(500000);
				}
				// std::cout << board << std::endl;
				// std::cout << "[H[2J[3J";
				// usleep(50000);
				Actions act = ai.chooseAction();
				double reward = board.snakeMove(act);
				Interpreter t(board.getSnakeViews(), board.getSnakeSize(), reward, act,
							board.hasWon(), board.hasLost());
				ai.updateQTable(t);
				if (board.hasLost())
				{
					(void)nbLoss++;
					break ;
				}
				if (board.hasWon())
				{
					(void)nbWin++;
					break ;
				}
				moves++;
			}
			// if (moves == 2000)
			// {
			// 	(void)nbLoss++;
			// 	(void)timeouts++;
			// }
			nbGames++;
			ai.decayExploration();
			ai.incrementTotalGames(board.hasWon());
			board.reset();
			ai.setState(Interpreter(board.getSnakeViews(), board.getSnakeSize(),
				0, Actions::FORWARD, 0, 0).getState());
		}
		ai.exportModel("0.9AppleDist0.3emptySquare4DangerSize.mdl");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}