CMAKE_MINIMUM_REQUIRED(VERSION 3.11)
PROJECT(LogicalCircuitStateMachine LANGUAGES CXX
        DESCRIPTION " LCSM project is a set of tools and libraries that provide construction and simulation of simple logic circuits for educational purposes."
        HOMEPAGE_URL "https://github.com/sfbakturin/lcsm-project"
)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

OPTION(LCSM_BUILD_TESTS "Build LCSM's tests." OFF)
OPTION(LCSM_BUILD_VISUAL "Build LCSM Visual application." OFF)
OPTION(LCSM_BUILD_DOCS "Build LCSM's API documentation." OFF)

OPTION(BUILD_USE_SANITIZER "Enable to build with default sanitizers." OFF)
OPTION(BUILD_ENABLE_WERROR "Treat all warnings as compile errors." OFF)

INCLUDE(GNUInstallDirs)
INCLUDE(CMakePackageConfigHelpers)
INCLUDE(CTest)

FIND_PACKAGE(Python COMPONENTS Interpreter)

ENABLE_TESTING()

# Helper to setup base settings for target.
FUNCTION(LCSM_BASE_SETUP_TARGET targetName)
    # Setup sanitizers.
    IF(BUILD_USE_SANITIZER)
        IF(MSVC)
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

# Helper to add to list for target.
FUNCTION(LCSM_ADD_TO_TARGET)
    SET(options)
    SET(oneValueArgs VARLIST)
    SET(multiValueArgs COMMFILES ONLYWINDOWS ONLYLINUX ONLYAPPLE)
    CMAKE_PARSE_ARGUMENTS(ARG
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
        ${ARGN}
    )
    SET(sourceAbsolutes "")
    FOREACH(source ${ARG_COMMFILES})
        GET_FILENAME_COMPONENT(sourceAbsolute "${source}" ABSOLUTE)
        LIST(APPEND "${ARG_VARLIST}" "${sourceAbsolute}")
        LIST(APPEND sourceAbsolutes "${sourceAbsolute}")
    ENDFOREACH()
    IF(WIN32)
        FOREACH(source ${ARG_ONLYWINDOWS})
            GET_FILENAME_COMPONENT(sourceAbsolute "${source}" ABSOLUTE)
            LIST(APPEND "${ARG_VARLIST}" "${sourceAbsolute}")
            LIST(APPEND sourceAbsolutes "${sourceAbsolute}")
        ENDFOREACH()
    ELSEIF(APPLE)
        FOREACH(source ${ARG_ONLYAPPLE})
            GET_FILENAME_COMPONENT(sourceAbsolute "${source}" ABSOLUTE)
            LIST(APPEND "${ARG_VARLIST}" "${sourceAbsolute}")
            LIST(APPEND sourceAbsolutes "${sourceAbsolute}")
        ENDFOREACH()
    ELSE()
        FOREACH(source ${ARG_ONLYLINUX})
            GET_FILENAME_COMPONENT(sourceAbsolute "${source}" ABSOLUTE)
            LIST(APPEND "${ARG_VARLIST}" "${sourceAbsolute}")
            LIST(APPEND sourceAbsolutes "${sourceAbsolute}")
        ENDFOREACH()
    ENDIF()
    SET(${ARG_VARLIST} "${${ARG_VARLIST}}" PARENT_SCOPE)
ENDFUNCTION()

ADD_SUBDIRECTORY(lcsm)

IF(LCSM_BUILD_VISUAL)
    ADD_SUBDIRECTORY(lcsm-visual)
ENDIF()
