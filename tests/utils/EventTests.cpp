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

    class Recorder
    {
    public:
        int value     = 0;
        int callCount = 0;
        int first     = 0;
        int second    = 0;

        void OnValue(int value_)
        {
            value = value_;
            ++callCount;
        }

        void OnPair(int first_, int second_)
        {
            first  = first_;
            second = second_;
        }

        void OnTick()
        {
            ++callCount;
        }
    };

    TEST(EventTests, AddReceivesInvocation)
    {
        IntSource source;
        Recorder  recorder;
        source.onValue.Add(&Recorder::OnValue, recorder);

        source.Raise(42);

        EXPECT_EQ(recorder.value, 42);
    }

    TEST(EventTests, DeliversToAllSubscribers)
    {
        IntSource source;
        Recorder  first;
        Recorder  second;
        source.onValue.Add(&Recorder::OnValue, first);
        source.onValue.Add(&Recorder::OnValue, second);

        source.Raise(5);

        EXPECT_EQ(first.value, 5);
        EXPECT_EQ(second.value, 5);
    }

    TEST(EventTests, RemoveStopsDelivery)
    {
        IntSource source;
        Recorder  recorder;
        source.onValue.Add(&Recorder::OnValue, recorder);

        source.onValue.Remove(&Recorder::OnValue, recorder);
        source.Raise(1);

        EXPECT_EQ(recorder.callCount, 0);
    }

    TEST(EventTests, RemoveOnlyAffectsGivenHandler)
    {
        IntSource source;
        Recorder  first;
        Recorder  second;
        source.onValue.Add(&Recorder::OnValue, first);
        source.onValue.Add(&Recorder::OnValue, second);

        source.onValue.Remove(&Recorder::OnValue, first);
        source.Raise(1);

        EXPECT_EQ(first.callCount, 0);
        EXPECT_EQ(second.callCount, 1);
    }

    TEST(EventTests, RemoveUnknownHandlerIsNoop)
    {
        IntSource source;
        Recorder  recorder;

        EXPECT_NO_THROW(source.onValue.Remove(&Recorder::OnValue, recorder));
    }

    TEST(EventTests, EachSubscriptionIsIndependent)
    {
        IntSource source;
        Recorder  recorder;
        source.onValue.Add(&Recorder::OnValue, recorder);
        source.onValue.Add(&Recorder::OnValue, recorder);

        source.Raise(1);

        EXPECT_EQ(recorder.callCount, 2);
    }

    TEST(EventTests, RemoveDropsOnlyOneOfSeveralSubscriptions)
    {
        IntSource source;
        Recorder  recorder;
        source.onValue.Add(&Recorder::OnValue, recorder);
        source.onValue.Add(&Recorder::OnValue, recorder);

        source.onValue.Remove(&Recorder::OnValue, recorder);
        source.Raise(1);

        EXPECT_EQ(recorder.callCount, 1);
    }

    TEST(EventTests, RemovingEverySubscriptionStopsDelivery)
    {
        IntSource source;
        Recorder  recorder;
        source.onValue.Add(&Recorder::OnValue, recorder);
        source.onValue.Add(&Recorder::OnValue, recorder);

        source.onValue.Remove(&Recorder::OnValue, recorder);
        source.onValue.Remove(&Recorder::OnValue, recorder);
        source.Raise(1);

        EXPECT_EQ(recorder.callCount, 0);
    }

    TEST(EventTests, DisconnectAllRemovesEverySubscriber)
    {
        IntSource source;
        Recorder  first;
        Recorder  second;
        source.onValue.Add(&Recorder::OnValue, first);
        source.onValue.Add(&Recorder::OnValue, second);

        source.Clear();
        source.Raise(1);

        EXPECT_EQ(first.callCount, 0);
        EXPECT_EQ(second.callCount, 0);
    }

    TEST(EventTests, ForwardsMultipleArguments)
    {
        PairSource source;
        Recorder   recorder;
        source.onPair.Add(&Recorder::OnPair, recorder);

        source.Raise(3, 4);

        EXPECT_EQ(recorder.first, 3);
        EXPECT_EQ(recorder.second, 4);
    }

    TEST(EventTests, VoidSignatureInvokesEachRaise)
    {
        VoidSource source;
        Recorder   recorder;
        source.onTick.Add(&Recorder::OnTick, recorder);

        source.Raise();
        source.Raise();

        EXPECT_EQ(recorder.callCount, 2);
    }
} // namespace ServerTests
