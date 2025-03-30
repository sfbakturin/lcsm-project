FUNCTION(LCSM_TEST_CLOCK testName)
    LCSM_ADD_TEST(
        CATEGORYNAME Component
        TESTNAME Clock${testName}
        FILES Clock${testName}Test.cpp ClockTesting.h ClockTesting.cpp
    )
ENDFUNCTION()

LCSM_TEST_CLOCK(Low1High1)
LCSM_TEST_CLOCK(Low2High1)
LCSM_TEST_CLOCK(Low2High2)
LCSM_TEST_CLOCK(Low1High0)
LCSM_TEST_CLOCK(Low0High2)
