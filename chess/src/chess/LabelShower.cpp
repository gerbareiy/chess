#include "LabelShower.h"

#include <iostream>

Chess::LabelShower::LabelShower(const std::shared_ptr<Inputer>& inputer)
{
    const auto show = [this](const std::string& str)
    {
        Show(str);
    };
    inputer->ConnectEnter(show);
}

void Chess::LabelShower::Show(const std::string& toPrint)
{
    std::cout << toPrint;
}
