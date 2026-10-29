#include "learn2Slither.hpp"

int main()
{
	try
	{
		Engine engine(500, 500);
		engine.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}