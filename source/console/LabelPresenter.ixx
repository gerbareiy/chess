module;
#include <functional>
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
            handler_ = [this](Core::eInputType type)
            {
                Show(type);
            };
            inputter_->onEnter.Add(handler_);
        }

        ~LabelPresenter()
        {
            inputter_->onEnter.Remove(handler_);
        }

    private:
        std::shared_ptr<Core::Inputter>       inputter_;
        std::function<void(Core::eInputType)> handler_;
    };
} // namespace Chess::Console
