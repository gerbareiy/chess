#include "chess/Game.h"

#include <memory>

int main()
{
	auto chess = std::make_unique<Chess::Game>();
	chess->Play();

	return 0;
}