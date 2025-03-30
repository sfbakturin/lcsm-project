FUNCTION(LCSM_TEST_BUTTON testName)
    LCSM_ADD_TEST(
        CATEGORYNAME Component
        TESTNAME Button${testName}
        FILES Button${testName}Test.cpp
    )
ENDFUNCTION()

LCSM_TEST_BUTTON(ActiveOnPress)
LCSM_TEST_BUTTON(NonActiveOnPress)
