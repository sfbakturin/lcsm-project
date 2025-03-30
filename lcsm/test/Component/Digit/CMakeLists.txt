FUNCTION(LCSM_TEST_DIGIT testName)
    LCSM_ADD_TEST(
        CATEGORYNAME Component
        TESTNAME Digit${testName}
        FILES Digit${testName}Test.cpp DigitTesting.h DigitTesting.cpp
    )
ENDFUNCTION()

LCSM_TEST_DIGIT(WithDecimalPoint)
LCSM_TEST_DIGIT(WithoutDecimalPoint)
