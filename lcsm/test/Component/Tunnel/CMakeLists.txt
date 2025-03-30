LCSM_ADD_TEST(
    CATEGORYNAME Component
    TESTNAME TunnelA
    FILES TunnelATest.cpp
)

SET(TunnelTests B1 B2 B3)

FOREACH(i ${TunnelTests})
    LCSM_ADD_TEST(
        CATEGORYNAME Component
        TESTNAME Tunnel${i}
        FILES Tunnel${i}Test.cpp TunnelBTesting.h TunnelBTesting.cpp
    )
ENDFOREACH()
