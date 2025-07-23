module;
#include <memory>
#include <print>
export module Chess.LabelDisplayer;
import Chess.eInputType;
import Chess.Inputer;

namespace Chess
{
    export class LabelDisplayer
    {
        std::shared_ptr<Inputer> m_inputer;

    public:
        static void Show(const std::string& message)
        {
            std::print("{}", message);
        }

        static void Show(eInputType type)
        {
            switch (type)
            {
            case eInputType::FROM:
                Show("FROM:\n");
                break;
            case eInputType::TO:
                Show("TO:\n");
                break;
            case eInputType::FILE:
                Show("File: ");
                break;
            case eInputType::RANK:
                Show("Rank: ");
                break;
            case eInputType::PROMOTION:
                Show("PROMOTE\nYou can Choose: B K Q R\nEnter: ");
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

        explicit LabelDisplayer(const std::shared_ptr<Inputer>& inputer)
            : m_inputer(inputer)
        {
        }

        ~LabelDisplayer() = default;
    };
} // namespace Chess
