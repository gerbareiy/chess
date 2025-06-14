#include "LableDisplayer.h"

#include <iostream>

Chess::LableDisplayer::LableDisplayer(std::shared_ptr<Inputer> const& Inputer)
{
	Inputer->ConnectEnter(std::bind(&LableDisplayer::Show, this, std::placeholders::_1));
}

void Chess::LableDisplayer::Show(std::string const& toPrint) const
{
	std::cout << toPrint;
}
