#include "test.hpp"
int main(void)
{
	try
	{
		srand(time(NULL));
		Board	board(10, 10);
		Agent	ai(0.1, 0.92, 0.9);
		int		nbGames = 0;
		long sizeDied = 0, sizeStarved = 0;
		int  nbDied = 0, nbStarved = 0, nbReach10 = 0;
		ai.importModel("29avg.mdl");
		ai.setState(Interpreter(board.getSnakeViews(),
				0, Actions::FORWARD, 0, 0).getState());
		ai.setTraining(false);
		int	nbMoves = 0, snakeSize = 0;
		int max = 0, maxMove = 0;
		int min = 0;
		while (nbGames < 10000)
		{
			size_t moves = 0;
			while (moves < 2500)
			{
				Actions act = ai.chooseAction();
				double reward = board.snakeMove(act);
				Interpreter t(board.getSnakeViews(), reward, act,
							board.hasWon(), board.hasLost());
				ai.updateQTable(t);
				if (board.hasLost())
					break ;
				moves++;
			}
			int sz = board.getSnakeSize();
			if (sz >= 10)
				nbReach10++;
			if (board.hasLost())
			{
				nbDied++;
				sizeDied += sz;
			}
			else
			{
				nbStarved++;
				sizeStarved += sz;
			}
			min = (!min) ? board.getSnakeSize() : std::min(min, (int)board.getSnakeSize());
			max = std::max(max, (int)board.getSnakeSize());
			snakeSize += board.getSnakeSize();
			nbMoves += moves;
			nbGames++;
			if (moves < 2500)
				maxMove = std::max(maxMove, (int)moves);
			
			ai.decayExploration();
			ai.incrementTotalGames();
			board.reset();
			ai.setState(Interpreter(board.getSnakeViews(),
				0, Actions::FORWARD, 0, 0).getState());
			if (!(nbGames % 1000))
			{
				std::cout << "[H[2J[3J";
				std::cout << "Train count = " << ai.getGamePlayed() << std::endl;
				std::cout << "Game count = " << nbGames << std::endl;
				std::cout << "nbReach10 = " << nbReach10 << std::endl;
				std::cout << "nbDied = " << nbDied << std::endl;
				std::cout << "avg sizeDied = " << sizeDied / (float)nbDied<< std::endl;
				std::cout << "nbStarved = " << nbStarved << std::endl;
				std::cout << "avg sizeStarved = " << sizeStarved / (float)nbStarved << std::endl;
				std::cout << "Session ratio = " << nbReach10 / (float)nbGames * 100 << '%' << std::endl;
				std::cout << "avg nbMoves = " << nbMoves / (float)nbGames << std::endl;
				std::cout << "max nbMoves = " << maxMove << std::endl;
				std::cout << "avg Snake size = " << snakeSize / (float)nbGames << std::endl;
				std::cout << "max Snake size = " << max << std::endl;
				std::cout << "min Snake size = " << min << std::endl;
				std::cout << "Epsilon = " << ai.getExplRate() << std::endl;
			}
		}
		ai.exportModel("100.mdl");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}