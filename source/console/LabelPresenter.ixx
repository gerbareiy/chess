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
        std::shared_ptr<::Chess::Inputter> m_inputter;
        boost::signals2::scoped_connection m_connection;

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
            : m_inputter(inputter)
        {
        }

        void Init()
        {
            const auto show = [this](::Chess::eInputType type)
            {
                Show(type);
            };
            m_connection = m_inputter->ConnectOnEnter(show);
        }

        ~LabelPresenter() = default;
    };
} // namespace Console::Chess
