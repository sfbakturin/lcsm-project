INCLUDE(CTest)

ADD_EXECUTABLE(test_dummy test_dummy.cpp)
TARGET_LINK_LIBRARIES(test_dummy PRIVATE LCSM LCSMC)

ADD_EXECUTABLE(test_pin_to_pin test_pin_to_pin.cpp)
TARGET_LINK_LIBRARIES(test_pin_to_pin PRIVATE LCSM LCSMC)

ADD_EXECUTABLE(test_logical_not test_logical_not.cpp)
TARGET_LINK_LIBRARIES(test_logical_not PRIVATE LCSM LCSMC)

ENABLE_TESTING()

ADD_TEST(NAME test-dummy COMMAND $<TARGET_FILE:test_dummy>)
ADD_TEST(NAME test-pin-to-pin COMMAND $<TARGET_FILE:test_pin_to_pin>)
ADD_TEST(NAME test-logical-not COMMAND $<TARGET_FILE:test_logical_not>)
