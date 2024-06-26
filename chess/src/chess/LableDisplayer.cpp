#include "LableDisplayer.h"

#include <iostream>

Chess::LableDisplayer::LableDisplayer(const std::shared_ptr<Inputer>& Inputer)
{
	Inputer->ConnectEnter(std::bind(&LableDisplayer::Show, this, std::placeholders::_1));
}

void Chess::LableDisplayer::Show(std::string toPrint) const
{
	std::cout << toPrint;
}
