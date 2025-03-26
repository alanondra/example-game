#include <iostream>
#include <exception>

import Game;
import Windowing.Exceptions;

int main(int argc, const char * argv[])
{
	try {
		Game game;
		game.run();
	} catch (const SdlException& e) {
		std::cout << e.what() << std::endl << e.getSdlError().value_or("Unknown") << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}
