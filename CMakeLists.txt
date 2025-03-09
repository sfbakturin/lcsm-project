CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(LogicalCircuitStateMachine LANGUAGES CXX C)

SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_C_EXTENSIONS OFF)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

OPTION(LCSM_BUILD_TESTS "Build LCSM's tests." OFF)
OPTION(LCSM_BUILD_VISUAL "Build LCSM Visual application." OFF)

OPTION(BUILD_USE_SANITIZER "Enable to build with default sanitizers." OFF)
OPTION(BUILD_ENABLE_WERROR "Treat all warnings as compile errors." OFF)

INCLUDE(GNUInstallDirs)
INCLUDE(CMakePackageConfigHelpers)
INCLUDE(CTest)

IF(UNIX AND NOT APPLE)
    SET(LINUX TRUE)
ELSE()
    SET(LINUX FALSE)
ENDIF()

FUNCTION(LCSM_BASE_SETUP_TARGET targetName)
    # Setup sanitizers.
    IF(BUILD_USE_SANITIZER)
        IF(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            TARGET_COMPILE_OPTIONS(${targetName} PUBLIC /fsanitize=address)
            TARGET_COMPILE_DEFINITIONS(${targetName} PUBLIC _DISABLE_STRING_ANNOTATION=1 _DISABLE_VECTOR_ANNOTATION=1)
        ELSE()
            IF(WIN32)
                TARGET_COMPILE_OPTIONS(${targetName} PRIVATE -fsanitize=address)
                TARGET_LINK_OPTIONS(${targetName} PRIVATE -fsanitize=address)
            ELSE()
                TARGET_COMPILE_OPTIONS(${targetName} PUBLIC -fsanitize=address,leak,undefined)
                TARGET_LINK_OPTIONS(${targetName} PUBLIC -fsanitize=address,leak,undefined)
            ENDIF()
            TARGET_COMPILE_OPTIONS(${targetName} PUBLIC -fno-sanitize-recover=all -fno-optimize-sibling-calls -fno-omit-frame-pointer)
        ENDIF()
    ENDIF()

    # Setup warnings.
    IF(MSVC)
        TARGET_COMPILE_OPTIONS(${targetName} PRIVATE /W4)
        IF(BUILD_ENABLE_WERROR)
            TARGET_COMPILE_OPTIONS(${targetName} PRIVATE /WX)
        ENDIF()
    ELSE()
        TARGET_COMPILE_OPTIONS(${targetName} PRIVATE -Wall -Wextra -Wpedantic)
        IF(BUILD_ENABLE_WERROR)
            TARGET_COMPILE_OPTIONS(${targetName} PRIVATE -Werror)
        ENDIF()
    ENDIF()
    IF(WIN32)
        TARGET_COMPILE_DEFINITIONS(${targetName} PUBLIC _CRT_SECURE_NO_WARNINGS)
    ENDIF()
ENDFUNCTION()

IF(LCSM_BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

ADD_SUBDIRECTORY(lcsm)

IF(LCSM_BUILD_VISUAL)
    ADD_SUBDIRECTORY(lcsm-vis)
ENDIF()
