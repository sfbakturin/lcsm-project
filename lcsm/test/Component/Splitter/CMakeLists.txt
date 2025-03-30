FUNCTION(LCSM_TEST_SPLITTER testName)
    LCSM_ADD_TEST(
        CATEGORYNAME Component
        TESTNAME Splitter${testName}
        FILES Splitter${testName}Test.cpp SplitterTesting.h SplitterTesting.cpp
    )
ENDFUNCTION()

LCSM_TEST_SPLITTER(2BitTo2Bit)
LCSM_TEST_SPLITTER(3BitTo2Bit)
LCSM_TEST_SPLITTER(4BitTo2Bit)
LCSM_TEST_SPLITTER(4BitTo3Bit)
