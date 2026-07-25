module;
#include <chrono>
#include <optional>
#include <stdexcept>
#include <string>
export module Chess.E2E.ChessMoveTyper;
import Chess.E2E.ConsoleProcess;
import Chess.E2E.MoveOutcome;
import Chess.E2E.Square;

namespace Chess::E2E
{
    export class ChessMoveTyper
    {
    public:
        static MoveOutcome TypeMove(const ConsoleProcess& process, Square from, Square to, std::optional<char> promotion = std::nullopt)
        {
            constexpr auto promptTimeout = std::chrono::seconds(10);

            if (!process.WaitFor("File: ", promptTimeout))
            {
                throw std::runtime_error("timed out waiting for the FROM file prompt");
            }
            process.SendLine(std::string(1, from.file));

            if (!process.WaitFor("Rank: ", promptTimeout))
            {
                throw std::runtime_error("timed out waiting for the FROM rank prompt");
            }
            process.SendLine(std::to_string(from.rank));

            const int afterFrom = process.WaitForAny({ "File: ", "No movable piece there." }, promptTimeout);
            if (afterFrom == -1)
            {
                throw std::runtime_error("timed out waiting for the TO file prompt");
            }
            if (afterFrom == 1)
            {
                return MoveOutcome::NoMovablePiece;
            }

            process.SendLine(std::string(1, to.file));

            if (!process.WaitFor("Rank: ", promptTimeout))
            {
                throw std::runtime_error("timed out waiting for the TO rank prompt");
            }
            process.SendLine(std::to_string(to.rank));

            if (promotion.has_value())
            {
                if (!process.WaitFor("Enter: ", promptTimeout))
                {
                    throw std::runtime_error("timed out waiting for the PROMOTE prompt");
                }
                process.SendLine("x");
                process.SendLine(std::string(1, promotion.value()));
            }

            return MoveOutcome::Ongoing;
        }

        static MoveOutcome PlayMove(
            const ConsoleProcess&     process,
            Square                    from,
            Square                    to,
            std::optional<char>       promotion     = std::nullopt,
            std::chrono::milliseconds settleTimeout = std::chrono::seconds(5))
        {
            const auto typed = TypeMove(process, from, to, promotion);
            if (typed != MoveOutcome::Ongoing)
            {
                return typed;
            }

            switch (process.WaitForAny({ "Illegal move.", "Checkmate!", "Draw!" }, settleTimeout))
            {
            case 0:
                return MoveOutcome::IllegalMove;
            case 1:
                return MoveOutcome::Checkmate;
            case 2:
                return MoveOutcome::Draw;
            default:
                return MoveOutcome::Ongoing;
            }
        }
    };
} // namespace Chess::E2E
