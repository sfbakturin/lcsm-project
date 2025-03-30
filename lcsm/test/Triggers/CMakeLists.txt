FUNCTION(LCSM_TEST_TRIGGER triggerName)
    LCSM_ADD_TEST(
        CATEGORYNAME Triggers
        TESTNAME ${triggerName}
        FILES ${triggerName}TriggerTest.cpp
    )
ENDFUNCTION()

LCSM_TEST_TRIGGER(RS)
