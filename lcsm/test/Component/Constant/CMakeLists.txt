FUNCTION(LCSM_TEST_CONSTANT testName)
    LCSM_ADD_TEST(
        CATEGORYNAME Component
        TESTNAME Constant${testName}
        FILES Constant${testName}Test.cpp ConstantTesting.h ConstantTesting.cpp
    )
ENDFUNCTION()

LCSM_TEST_CONSTANT(Values)
LCSM_TEST_CONSTANT(Widths)
