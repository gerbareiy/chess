module;
#include <boost/signals2.hpp>
#include <memory>
#include <print>
export module Console.Chess.LabelPresenter;
import Chess.eInputType;
import Chess.Inputer;

namespace Console::Chess
{
    export class LabelPresenter
    {
    public:
        static void Show(::Chess::eInputType type)
        {
            switch (type)
            {
            case ::Chess::eInputType::FROM:
                std::println("FROM:");
                break;
            case ::Chess::eInputType::TO:
                std::println("TO:");
                break;
            case ::Chess::eInputType::FILE:
                std::print("File: ");
                break;
            case ::Chess::eInputType::RANK:
                std::print("Rank: ");
                break;
            case ::Chess::eInputType::PROMOTION:
                std::print("PROMOTE\nYou can Choose: B K Q R\nEnter: ");
                break;
            }
        }

        explicit LabelPresenter(const std::shared_ptr<::Chess::Inputter>& inputter)
            : inputter_(inputter)
        {
        }

        void Init()
        {
            const auto show = [this](::Chess::eInputType type)
            {
                Show(type);
            };
            connection_ = inputter_->ConnectOnEnter(show);
        }

        ~LabelPresenter() = default;

    private:
        std::shared_ptr<::Chess::Inputter> inputter_;
        boost::signals2::scoped_connection connection_;
    };
} // namespace Console::Chess
