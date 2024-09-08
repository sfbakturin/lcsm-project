include(CTest)

ADD_EXECUTABLE(test_dummy test_dummy.cpp)
TARGET_LINK_LIBRARIES(test_dummy PRIVATE ${PROJECT_NAME})

ADD_EXECUTABLE(test_logical_not test_logical_not.cpp)
TARGET_LINK_LIBRARIES(test_logical_not PRIVATE ${PROJECT_NAME})

ENABLE_TESTING()

ADD_TEST(NAME test-dummy COMMAND $<TARGET_FILE:test_dummy>)
ADD_TEST(NAME test-logical-not COMMAND $<TARGET_FILE:test_logical_not>)
