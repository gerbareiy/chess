#include "InputDisplayer.h"

#include <iostream>

Chess::InputDisplayer::InputDisplayer(const std::shared_ptr<InputHandler>& handler)
{
	handler->ConnectEnter(std::bind(&InputDisplayer::Show, this, std::placeholders::_1));
}

void Chess::InputDisplayer::Show(std::string toPrint) const
{
	std::cout << toPrint;
}
