module;
#include <iostream>
#include <memory>
#include <string>
export module Chess.LabelShower;
import Chess.Inputer;

namespace Chess
{
    export class LabelShower
    {
    public:
        explicit LabelShower(const std::shared_ptr<Inputer>& inputer)
        {
            const auto show = [this](const std::string& str)
            {
                Show(str);
            };
            inputer->ConnectEnter(show);
        }

        ~LabelShower() = default;

        static void Show(const std::string& toPrint)
        {
            std::cout << toPrint;
        }
    };
} // namespace Chess
