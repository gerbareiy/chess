module;
#include <array>
#include <cstdint>
#include <string>
export module Chess.Net.Framing;

namespace Chess::Net
{
    export class Framing
    {
    public:
        static constexpr std::size_t HEADER_SIZE = 4;

        static std::string Encode(const std::string& payload)
        {
            const auto size = static_cast<uint32_t>(payload.size());

            std::string frame;
            frame.reserve(HEADER_SIZE + payload.size());
            frame.push_back(static_cast<char>((size >> 24) & 0xFF));
            frame.push_back(static_cast<char>((size >> 16) & 0xFF));
            frame.push_back(static_cast<char>((size >> 8) & 0xFF));
            frame.push_back(static_cast<char>(size & 0xFF));
            frame.append(payload);
            return frame;
        }

        static uint32_t DecodeLength(const std::array<uint8_t, HEADER_SIZE>& header)
        {
            return (static_cast<uint32_t>(header[0]) << 24) | (static_cast<uint32_t>(header[1]) << 16) | (static_cast<uint32_t>(header[2]) << 8)
                   | static_cast<uint32_t>(header[3]);
        }
    };
} // namespace Chess::Net
