module;
#include <any>
#include <functional>
#include <memory>
#include <vector>
export module Chess.Utils.Event;

namespace Chess::Utils
{
    export template <typename Signature, typename Owner> class Event
    {
    public:
        template <typename Method, typename Class> void Add(Method method, Class& instance)
        {
            subscribers_.push_back(MakeSubscription(method, instance));
        }

        template <typename Method, typename Class> void Remove(Method method, Class& instance)
        {
            const void* key = std::addressof(instance);
            for (auto index = subscribers_.size(); index-- > 0;)
            {
                if (subscribers_[index].instance != key)
                {
                    continue;
                }
                if (const auto* stored = std::any_cast<Method>(&subscribers_[index].method); stored != nullptr && *stored == method)
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
            const void*              instance;
            std::any                 method;
            std::function<Signature> callback;
        };

        template <typename Method, typename Class> static Subscription MakeSubscription(Method method, Class& instance)
        {
            return { std::addressof(instance),
                     method,
                     [&instance, method](auto&&... args) { (instance.*method)(std::forward<decltype(args)>(args)...); } };
        }

        template <typename... Args> void Invoke(Args&&... args) const
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
