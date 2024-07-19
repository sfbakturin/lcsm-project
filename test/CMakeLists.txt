include(CTest)

ADD_EXECUTABLE(test_dummy test_dummy.cpp)
TARGET_LINK_LIBRARIES(test_dummy PRIVATE ${PROJECT_NAME})

ENABLE_TESTING()

ADD_TEST(NAME test-dummy COMMAND $<TARGET_FILE:test_dummy>)
