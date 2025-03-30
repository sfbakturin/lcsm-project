FUNCTION(LCSM_TEST_OTHER testName)
    LCSM_ADD_TEST(
        CATEGORYNAME Other
        TESTNAME ${testName}
        FILES ${testName}Test.cpp
    )
ENDFUNCTION()

LCSM_TEST_OTHER("CircuitInCircuit")
LCSM_TEST_OTHER("Copy")
LCSM_TEST_OTHER("LCSMFile")
LCSM_TEST_OTHER("Remove")
LCSM_TEST_OTHER("Verilog")
