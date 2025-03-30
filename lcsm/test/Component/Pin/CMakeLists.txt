FUNCTION(LCSM_TEST_PIN testName)
    LCSM_ADD_TEST(
        CATEGORYNAME Component
        TESTNAME Pin${testName}
        FILES Pin${testName}Test.cpp
    )
ENDFUNCTION()

LCSM_TEST_PIN(InputToOutput)
LCSM_TEST_PIN(InputToInput)
LCSM_TEST_PIN(2InsTo2Outs)
