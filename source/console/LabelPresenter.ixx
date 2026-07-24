module;
#include <boost/signals2.hpp>
#include <memory>
#include <print>
export module Chess.Console.LabelPresenter;
import Chess.Core.eInputType;
import Chess.Core.Inputter;

namespace Chess::Console
{
    export class LabelPresenter
    {
    public:
        static void Show(Core::eInputType type)
        {
            switch (type)
            {
            case Core::eInputType::FROM:
                std::println("FROM:");
                break;
            case Core::eInputType::TO:
                std::println("TO:");
                break;
            case Core::eInputType::FILE:
                std::print("File: ");
                break;
            case Core::eInputType::RANK:
                std::print("Rank: ");
                break;
            case Core::eInputType::PROMOTION:
                std::print("PROMOTE\nYou can Choose: B K Q R\nEnter: ");
                break;
            }
        }

        explicit LabelPresenter(const std::shared_ptr<Core::Inputter>& inputter)
            : inputter_(inputter)
        {
        }

        void Init()
        {
            const auto show = [this](Core::eInputType type)
            {
                Show(type);
            };
            connection_ = inputter_->ConnectOnEnter(show);
        }

        ~LabelPresenter() = default;

    private:
        std::shared_ptr<Core::Inputter>    inputter_;
        boost::signals2::scoped_connection connection_;
    };
} // namespace Chess::Console
