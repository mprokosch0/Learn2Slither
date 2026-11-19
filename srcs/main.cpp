#include "learn2Slither.hpp"

int main()
{
	try
	{
		srand(time(NULL));
		Engine engine(1422, 800);
		engine.addTexture("Snake.xpm", std::make_pair(16, 16));
		engine.run();
	}
	catch(const std::exception& e)
	{}
	
	return 0;
}