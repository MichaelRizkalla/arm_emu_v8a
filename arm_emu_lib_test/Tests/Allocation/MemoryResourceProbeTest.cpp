
#include <Interrupt/Interrupt.h>
#include <Tests/Allocation/MemoryResourceProbeTest.h>
#include <Utility/UniqueRef.h>
#include <barrier>
#include <thread>

BEGIN_NAMESPACE

namespace test {

    namespace {

        struct TestStruct {
            arm_emu::UniqueRef< int >               val1;
            arm_emu::UniqueRef< SharedRef< void > > val2;
        };

    } // namespace

    MemoryResourceProbeTest::MemoryResourceProbeTest() = default;

    MemoryResourceProbeTest::~MemoryResourceProbeTest() = default;

    template < class T >
    std::pmr::polymorphic_allocator< T > MemoryResourceProbeTest::GetAlloc() noexcept {
        std::pmr::polymorphic_allocator< T > alloc { &m_resource };
        return alloc;
    }

    void MemoryResourceProbeTest::TestSingleThreadedProbing() {
        using namespace arm_emu;

        auto&                            probe         = MemoryResourceProbe::GetInstance();
        auto                             startingValue = probe.GetAllocatedMemory().size();
        std::vector< SharedRef< void > > data;

        auto intObj = std::allocate_shared< int >(GetAlloc< int >(), 5);
        data.emplace_back(intObj); // uses std::allocator<>

        ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 1);
        ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 1);
        ASSERT_NE(probe.GetAllocatedMemory().at(probe.GetAllocatedMemory().size() - 1).first, 0);

        auto val1 = allocate_unique< int >(GetAlloc< int >(), 1);
        auto val2 = allocate_unique< SharedRef< void > >(GetAlloc< SharedRef< void > >(), intObj);
        auto testStruct =
            std::allocate_shared< TestStruct >(GetAlloc< TestStruct >(), std::move(val1), std::move(val2));

        ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 4);
        ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 4);
        ASSERT_NE(probe.GetAllocatedMemory().at(probe.GetAllocatedMemory().size() - 1).first, 0);
        ASSERT_EQ(probe.GetAllocatedMemory().at(probe.GetAllocatedMemory().size() - 2).first,
                  sizeof(SharedRef< void >));
        ASSERT_EQ(probe.GetAllocatedMemory().at(probe.GetAllocatedMemory().size() - 3).first, sizeof(int));

        testStruct->val1 = nullptr;

        ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 3);
        ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 3);

        std::destroy_at(&testStruct);

        ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 1);
        ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 1);

        data.clear();
        std::destroy_at(&intObj);

        ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 0);
        ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 0);
    }

    void MemoryResourceProbeTest::TestMultiThreadingProbing() {
        using namespace arm_emu;
        const int nThreads = std::thread::hardware_concurrency() - 1;

        std::vector< std::thread > threads {};

        std::barrier mBarrier { nThreads };

        auto test_runner = [&]() {
            auto& probe         = MemoryResourceProbe::GetInstance();
            auto  startingValue = probe.GetAllocatedMemory().size();

            std::vector< SharedRef< void > > data;

            mBarrier.arrive_and_wait();

            auto intObj = std::allocate_shared< int >(GetAlloc< int >(), 5);
            data.emplace_back(intObj); // uses std::allocator<>

            mBarrier.arrive_and_wait();

            ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 1 * nThreads);
            ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 1 * nThreads);

            mBarrier.arrive_and_wait();

            auto val1 = allocate_unique< int >(GetAlloc< int >(), 1);
            auto val2 = allocate_unique< SharedRef< void > >(GetAlloc< SharedRef< void > >(), intObj);
            auto testStruct =
                std::allocate_shared< TestStruct >(GetAlloc< TestStruct >(), std::move(val1), std::move(val2));

            mBarrier.arrive_and_wait();

            ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 4 * nThreads);
            ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 4 * nThreads);

            mBarrier.arrive_and_wait();

            testStruct->val1 = nullptr;

            mBarrier.arrive_and_wait();

            ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 3 * nThreads);
            ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 3 * nThreads);

            mBarrier.arrive_and_wait();

            std::destroy_at(&testStruct);

            mBarrier.arrive_and_wait();

            ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 1 * nThreads);
            ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 1 * nThreads);

            mBarrier.arrive_and_wait();

            data.clear();
            std::destroy_at(&intObj);

            mBarrier.arrive_and_wait();

            ASSERT_EQ(probe.GetAllocatedMemory().size() - startingValue, 0 * nThreads);
            ASSERT_EQ(probe.GetCurrentAllocationCount() - startingValue, 0 * nThreads);
        };

        for (auto i = 0; i < nThreads; ++i) {
            threads.emplace_back(test_runner);
        }

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    TEST_F(MemoryResourceProbeTest, SingleThreadProbing) {
        TestSingleThreadedProbing();
    }

    TEST_F(MemoryResourceProbeTest, MultiThreadProbing) {
        TestMultiThreadingProbing();
    }

} // namespace test

END_NAMESPACE
