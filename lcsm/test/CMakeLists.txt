INCLUDE(CTest)

ADD_EXECUTABLE(test_dummy test_dummy.cpp)
TARGET_LINK_LIBRARIES(test_dummy PRIVATE LCSM)

ADD_EXECUTABLE(test_dummy2 test_dummy2.cpp)
TARGET_LINK_LIBRARIES(test_dummy2 PRIVATE LCSM)

ADD_EXECUTABLE(test_dummy_remove test_dummy_remove.cpp)
TARGET_LINK_LIBRARIES(test_dummy_remove PRIVATE LCSM)

ADD_EXECUTABLE(test_dummy_copy test_dummy_copy.cpp)
TARGET_LINK_LIBRARIES(test_dummy_copy PRIVATE LCSM)

ADD_EXECUTABLE(test_pin_to_pin test_pin_to_pin.cpp)
TARGET_LINK_LIBRARIES(test_pin_to_pin PRIVATE LCSM)

ADD_EXECUTABLE(test_power_to_pin test_power_to_pin.cpp)
TARGET_LINK_LIBRARIES(test_power_to_pin PRIVATE LCSM)

ADD_EXECUTABLE(test_ground_to_pin test_ground_to_pin.cpp)
TARGET_LINK_LIBRARIES(test_ground_to_pin PRIVATE LCSM)

ADD_EXECUTABLE(test_internal_circuit test_internal_circuit.cpp)
TARGET_LINK_LIBRARIES(test_internal_circuit PRIVATE LCSM)

# ADD_EXECUTABLE(test_logical_not test_logical_not.cpp)
# TARGET_LINK_LIBRARIES(test_logical_not PRIVATE LCSM)

ENABLE_TESTING()

ADD_TEST(NAME test-dummy COMMAND $<TARGET_FILE:test_dummy>)
ADD_TEST(NAME test-dummy2 COMMAND $<TARGET_FILE:test_dummy2>)
ADD_TEST(NAME test-dummy-remove COMMAND $<TARGET_FILE:test_dummy_remove>)
ADD_TEST(NAME test-dummy-copy COMMAND $<TARGET_FILE:test_dummy_copy>)
ADD_TEST(NAME test-pin-to-pin COMMAND $<TARGET_FILE:test_pin_to_pin>)
ADD_TEST(NAME test-power-to-pin COMMAND $<TARGET_FILE:test_power_to_pin>)
ADD_TEST(NAME test-ground-to-pin COMMAND $<TARGET_FILE:test_ground_to_pin>)
ADD_TEST(NAME test-internal-circuit COMMAND $<TARGET_FILE:test_internal_circuit>)
# ADD_TEST(NAME test-logical-not COMMAND $<TARGET_FILE:test_logical_not>)
