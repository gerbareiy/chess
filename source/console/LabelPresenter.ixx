module;
#include <memory>
#include <print>
export module Chess.LabelDisplayer;
import Chess.eInputType;
import Chess.Inputer;

namespace Chess
{
    export class LabelPresenter
    {
        std::shared_ptr<Inputer> m_inputer;

    public:
        static void Show(eInputType type)
        {
            switch (type)
            {
            case eInputType::FROM:
                std::println("FROM:");
                break;
            case eInputType::TO:
                std::println("TO:");
                break;
            case eInputType::FILE:
                std::print("File: ");
                break;
            case eInputType::RANK:
                std::print("Rank: ");
                break;
            case eInputType::PROMOTION:
                std::print("PROMOTE\nYou can Choose: B K Q R\nEnter: ");
                break;
            }
        }

        void Init()
        {
            const auto show = [this](eInputType type)
            {
                Show(type);
            };
            m_inputer->ConnectOnEnter(show);
        }

        explicit LabelPresenter(const std::shared_ptr<Inputer>& inputer)
            : m_inputer(inputer)
        {
        }

        ~LabelPresenter() = default;
    };
} // namespace Chess
