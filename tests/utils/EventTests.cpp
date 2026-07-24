#include <functional>
#include <gtest/gtest.h>
import Chess.Utils.Event;

namespace ServerTests
{
    using Chess::Utils::Event;

    class IntSource
    {
    public:
        Event<void(int), IntSource> onValue;

        void Raise(int value)
        {
            onValue.Invoke(value);
        }

        void Clear()
        {
            onValue.DisconnectAll();
        }
    };

    class PairSource
    {
    public:
        Event<void(int, int), PairSource> onPair;

        void Raise(int first, int second)
        {
            onPair.Invoke(first, second);
        }
    };

    class VoidSource
    {
    public:
        Event<void(), VoidSource> onTick;

        void Raise()
        {
            onTick.Invoke();
        }
    };

    TEST(EventTests, AddReceivesInvocation)
    {
        IntSource                source;
        int                      received = 0;
        std::function<void(int)> handler  = [&](int value) { received = value; };
        source.onValue.Add(handler);

        source.Raise(42);

        EXPECT_EQ(received, 42);
    }

    TEST(EventTests, DeliversToAllSubscribers)
    {
        IntSource                source;
        int                      first   = 0;
        int                      second  = 0;
        std::function<void(int)> handler1 = [&](int value) { first += value; };
        std::function<void(int)> handler2 = [&](int value) { second += value; };
        source.onValue.Add(handler1);
        source.onValue.Add(handler2);

        source.Raise(5);

        EXPECT_EQ(first, 5);
        EXPECT_EQ(second, 5);
    }

    TEST(EventTests, RemoveStopsDelivery)
    {
        IntSource                source;
        int                      count   = 0;
        std::function<void(int)> handler = [&](int) { ++count; };
        source.onValue.Add(handler);

        source.onValue.Remove(handler);
        source.Raise(1);

        EXPECT_EQ(count, 0);
    }

    TEST(EventTests, RemoveOnlyAffectsGivenHandler)
    {
        IntSource                source;
        int                      first    = 0;
        int                      second   = 0;
        std::function<void(int)> handler1 = [&](int) { ++first; };
        std::function<void(int)> handler2 = [&](int) { ++second; };
        source.onValue.Add(handler1);
        source.onValue.Add(handler2);

        source.onValue.Remove(handler1);
        source.Raise(1);

        EXPECT_EQ(first, 0);
        EXPECT_EQ(second, 1);
    }

    TEST(EventTests, RemoveUnknownHandlerIsNoop)
    {
        IntSource                source;
        std::function<void(int)> handler = [&](int) {};

        EXPECT_NO_THROW(source.onValue.Remove(handler));
    }

    TEST(EventTests, EachSubscriptionIsIndependent)
    {
        IntSource                source;
        int                      count   = 0;
        std::function<void(int)> handler = [&](int) { ++count; };
        source.onValue.Add(handler);
        source.onValue.Add(handler);

        source.Raise(1);

        EXPECT_EQ(count, 2);
    }

    TEST(EventTests, RemoveDropsOnlyOneOfSeveralSubscriptions)
    {
        IntSource                source;
        int                      count   = 0;
        std::function<void(int)> handler = [&](int) { ++count; };
        source.onValue.Add(handler);
        source.onValue.Add(handler);

        source.onValue.Remove(handler);
        source.Raise(1);

        EXPECT_EQ(count, 1);
    }

    TEST(EventTests, RemovingEverySubscriptionStopsDelivery)
    {
        IntSource                source;
        int                      count   = 0;
        std::function<void(int)> handler = [&](int) { ++count; };
        source.onValue.Add(handler);
        source.onValue.Add(handler);

        source.onValue.Remove(handler);
        source.onValue.Remove(handler);
        source.Raise(1);

        EXPECT_EQ(count, 0);
    }

    TEST(EventTests, DisconnectAllRemovesEverySubscriber)
    {
        IntSource                source;
        int                      first    = 0;
        int                      second   = 0;
        std::function<void(int)> handler1 = [&](int) { ++first; };
        std::function<void(int)> handler2 = [&](int) { ++second; };
        source.onValue.Add(handler1);
        source.onValue.Add(handler2);

        source.Clear();
        source.Raise(1);

        EXPECT_EQ(first, 0);
        EXPECT_EQ(second, 0);
    }

    TEST(EventTests, ForwardsMultipleArguments)
    {
        PairSource                    source;
        int                           first  = 0;
        int                           second = 0;
        std::function<void(int, int)> handler = [&](int a, int b)
        {
            first  = a;
            second = b;
        };
        source.onPair.Add(handler);

        source.Raise(3, 4);

        EXPECT_EQ(first, 3);
        EXPECT_EQ(second, 4);
    }

    TEST(EventTests, VoidSignatureInvokesEachRaise)
    {
        VoidSource            source;
        int                   count   = 0;
        std::function<void()> handler = [&] { ++count; };
        source.onTick.Add(handler);

        source.Raise();
        source.Raise();

        EXPECT_EQ(count, 2);
    }
} // namespace ServerTests
