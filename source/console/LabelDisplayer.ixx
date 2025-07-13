module;
#include <iostream>
#include <memory>
#include <string>
export module Chess.LabelDisplayer;
import Chess.Inputer;

namespace Chess
{
    export class LabelDisplayer
    {
    public:
        explicit LabelDisplayer(const std::shared_ptr<Inputer>& inputer)
        {
            const auto show = [this](const std::string& str)
            {
                Show(str);
            };
            inputer->ConnectEnter(show);
        }

        ~LabelDisplayer() = default;

        static void Show(const std::string& toPrint)
        {
            std::cout << toPrint;
        }
    };
} // namespace Chess
