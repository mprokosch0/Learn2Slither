#include "learn2Slither.hpp"

int main()
{
	try
	{
		srand(time(NULL));
		Engine engine(1422, 800);
		engine.run();
	}
	catch(const std::exception& e)
	{}
	
	return 0;
}