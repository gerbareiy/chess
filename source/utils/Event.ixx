module;
#include <functional>
#include <memory>
#include <vector>
export module Chess.Utils.Event;

namespace Chess::Utils
{
    export template <typename Signature, typename Owner>
    class Event
    {
    public:
        template <typename Handler>
        void Add(Handler& handler)
        {
            subscribers_.push_back({ std::addressof(handler), handler });
        }

        template <typename Handler>
        void Remove(Handler& handler)
        {
            const void* key = std::addressof(handler);
            for (auto index = subscribers_.size(); index-- > 0;)
            {
                if (subscribers_[index].key == key)
                {
                    subscribers_.erase(subscribers_.begin() + index);
                    return;
                }
            }
        }

    private:
        friend Owner;

        struct Subscription
        {
            const void*              key;
            std::function<Signature> callback;
        };

        template <typename... Args>
        void Invoke(Args&&... args) const
        {
            const auto snapshot = subscribers_;
            for (const auto& subscription : snapshot)
            {
                subscription.callback(args...);
            }
        }

        void DisconnectAll()
        {
            subscribers_.clear();
        }

        std::vector<Subscription> subscribers_;
    };
} // namespace Chess::Utils
