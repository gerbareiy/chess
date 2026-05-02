module;
#include <boost/signals2.hpp>

#include <memory>
#include <print>
export module Chess.LabelDisplayer;
import Chess.eInputType;
import Chess.Inputer;

namespace Chess
{
    export class LabelPresenter
    {
        std::shared_ptr<Inputter> m_inputter;
        boost::signals2::scoped_connection m_connection;

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

        explicit LabelPresenter(const std::shared_ptr<Inputter>& inputter)
            : m_inputter(inputter)
        {
        }

        void Init()
        {
            const auto show = [this](eInputType type) { Show(type); };
            m_connection    = m_inputter->ConnectOnEnter(show);
        }

        ~LabelPresenter() = default;
    };
} // namespace Chess
