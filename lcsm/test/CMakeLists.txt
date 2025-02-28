FUNCTION(lcsm_add_test testName testCategory)
    ADD_EXECUTABLE(${testName}Test "${CMAKE_CURRENT_SOURCE_DIR}/${testCategory}/${testName}Test.cpp")
    TARGET_LINK_LIBRARIES(${testName}Test PRIVATE LCSM)
    ADD_TEST(NAME ${testName} COMMAND ${testName}Test)
ENDFUNCTION()

FUNCTION(lcsm_add_circuit_test testName)
    lcsm_add_test(${testName} Circuit)
ENDFUNCTION()

FUNCTION(lcsm_add_userend_test testName)
    lcsm_add_test(${testName} Userend)
ENDFUNCTION()

lcsm_add_circuit_test(PinToPin1)
lcsm_add_circuit_test(PinToPin2)
lcsm_add_circuit_test(PinToPin3)
lcsm_add_circuit_test(GroundToPin)
lcsm_add_circuit_test(PowerToPin)

lcsm_add_userend_test(CircuitInCircuit1)
lcsm_add_userend_test(Copy1)
lcsm_add_userend_test(Remove1)
