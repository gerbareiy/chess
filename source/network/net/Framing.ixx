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
            std::string frame;
            frame.reserve(HEADER_SIZE + payload.size());
            frame.push_back(static_cast<char>(payload.size() >> 24 & 0xFF));
            frame.push_back(static_cast<char>(payload.size() >> 16 & 0xFF));
            frame.push_back(static_cast<char>(payload.size() >> 8 & 0xFF));
            frame.push_back(static_cast<char>(payload.size() & 0xFF));
            frame.append(payload);
            return frame;
        }

        static std::uint32_t DecodeLength(std::array<std::uint8_t, HEADER_SIZE> const& header)
        {
            const std::uint32_t thirdByte  = static_cast<std::uint32_t>(header[0]) << 24;
            const std::uint32_t secondByte = static_cast<std::uint32_t>(header[1]) << 16;
            const std::uint32_t firstByte  = static_cast<std::uint32_t>(header[2]) << 8;
            const std::uint32_t zeroByte   = header[3];
            return thirdByte | secondByte | firstByte | zeroByte;
        }
    };
} // namespace Chess::Net
