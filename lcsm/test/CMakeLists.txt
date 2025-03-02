FUNCTION(lcsm_add_test testName)
    ADD_EXECUTABLE(${testName}Test "${CMAKE_CURRENT_SOURCE_DIR}/${testName}Test.cpp")
    TARGET_LINK_LIBRARIES(${testName}Test PRIVATE LCSM)
    ADD_TEST(NAME ${testName} COMMAND ${testName}Test)
ENDFUNCTION()

ADD_SUBDIRECTORY(Circuit)
# ADD_SUBDIRECTORY(Example)
ADD_SUBDIRECTORY(Userend)
