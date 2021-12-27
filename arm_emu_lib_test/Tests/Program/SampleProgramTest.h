#if !defined(SAMPLEPROGRAMTEST_H_INCLUDED_D8CA2A2D_A219_4C26_8719_3A7556E93119)
    #define SAMPLEPROGRAMTEST_H_INCLUDED_D8CA2A2D_A219_4C26_8719_3A7556E93119

    #include <GTest/gtest.h>

    #include <API/Api.h>
    #include <Program/Program.h>

BEGIN_NAMESPACE

namespace test {

    class SampleProgramTest : public ::testing::Test {
      protected:
        SampleProgramTest();
        ~SampleProgramTest();

        void CheckSampleProgram(std::uint64_t programNumber);

        Program m_program;
    };

} // namespace test

END_NAMESPACE

#endif // !defined(SAMPLEPROGRAMTEST_H_INCLUDED_D8CA2A2D_A219_4C26_8719_3A7556E93119)
