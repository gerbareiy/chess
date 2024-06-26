#include "InputerDisplayer.h"

#include <iostream>

Chess::InputerDisplayer::InputerDisplayer(const std::shared_ptr<Inputer>& Inputer)
{
	Inputer->ConnectEnter(std::bind(&InputerDisplayer::Show, this, std::placeholders::_1));
}

void Chess::InputerDisplayer::Show(std::string toPrint) const
{
	std::cout << toPrint;
}
