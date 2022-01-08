#if !defined(RESULTTEST_H_INCLUDED_6C6ADBCE_D2A5_40DA_9111_265948A28EF0)
    #define RESULTTEST_H_INCLUDED_6C6ADBCE_D2A5_40DA_9111_265948A28EF0

    #include <GTest/gtest.h>

    #include <API/Api.h>
    #include <Program/ControlledResult.h>
    #include <Program/Result.h>
    #include <Program/ResultElement.h>

BEGIN_NAMESPACE

namespace test {

    class ResultTest : public ::testing::Test {
      protected:
        ResultTest();
        ~ResultTest();

        void CheckResultElement(bool stepIn);
        void CheckResult();
        void CheckControlledResult();

        SharedRef< arm_emu::ResultElement > m_resultElement;
    };

} // namespace test

END_NAMESPACE

#endif // !defined(RESULTTEST_H_INCLUDED_6C6ADBCE_D2A5_40DA_9111_265948A28EF0)
