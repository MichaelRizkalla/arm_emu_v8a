#if !defined(MEMORYRESOURCEPROBETEST_H_INCLUDED_AC0DFDB3_C783_4CF3_87DE_0145F9454193)
    #define MEMORYRESOURCEPROBETEST_H_INCLUDED_AC0DFDB3_C783_4CF3_87DE_0145F9454193

    #include <GTest/gtest.h>

    #include <API/Api.h>
    #include <Allocation/MemoryResource.h>
    #include <Allocation/MemoryResourceProbe.h>
    #include <vector>

BEGIN_NAMESPACE

namespace test {

    class MemoryResourceProbeTest : public ::testing::Test {
      protected:
        MemoryResourceProbeTest();
        ~MemoryResourceProbeTest();

        template < class T >
        std::pmr::polymorphic_allocator< T > GetAlloc() noexcept;

        void TestSingleThreadedProbing();
        void TestMultiThreadingProbing();

        arm_emu::MemoryResource m_resource;
    };

} // namespace test

END_NAMESPACE

#endif // !defined(MEMORYRESOURCEPROBETEST_H_INCLUDED_AC0DFDB3_C783_4CF3_87DE_0145F9454193)
