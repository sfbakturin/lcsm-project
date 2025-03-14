PROJECT(LCSM LANGUAGES CXX
             VERSION 1.0.0
             DESCRIPTION "LCSM's project main library provides building and simulating simple logic circuits"
             HOMEPAGE_URL "https://github.com/sfbakturin/lcsm-project"
)

ADD_LIBRARY(LCSM)
LCSM_BASE_SETUP_TARGET(LCSM)

# Helper to add to source group.
FUNCTION(LCSM_ADD_GROUP_SOURCE_FILES group)
    SET(foundSources "")
    FOREACH(source ${LCSM_SOURCES})
        IF (group STREQUAL "")
            STRING(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src/[^/]*\.cpp" "" replacedTemp "${source}")
        ELSE()
            STRING(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src/${group}/[^/]*\.cpp" "" replacedTemp "${source}")
        ENDIF()
        IF (replacedTemp STREQUAL "")
            LIST(APPEND foundSources "${source}")
        ENDIF()
    ENDFOREACH()
    SOURCE_GROUP("Source files/${group}" FILES ${foundSources})
ENDFUNCTION()

# Helper to add to header group.
FUNCTION(LCSM_ADD_GROUP_HEADER_FILES group)
    SET(foundHeaders "")
    FOREACH(header ${LCSM_HEADERS})
        IF (group STREQUAL "")
            STRING(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/include/[^/]*" "" replacedTemp "${header}")
        ELSE()
            STRING(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/include/${group}/[^/]*" "" replacedTemp "${header}")
        ENDIF()
        IF (replacedTemp STREQUAL "")
            # MESSAGE(STATUS "${header} from ${group}")
            LIST(APPEND foundHeaders "${header}")
        ENDIF()
    ENDFOREACH()
    SOURCE_GROUP("Header files/${group}" FILES ${foundHeaders})
ENDFUNCTION()

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)

LCSM_ADD_GROUP_SOURCE_FILES("")
LCSM_ADD_GROUP_SOURCE_FILES("Model")
LCSM_ADD_GROUP_SOURCE_FILES("Model/std")
LCSM_ADD_GROUP_SOURCE_FILES("Physical")
LCSM_ADD_GROUP_SOURCE_FILES("Physical/std")
LCSM_ADD_GROUP_SOURCE_FILES("Verilog")
LCSM_ADD_GROUP_SOURCE_FILES("Support")
LCSM_ADD_GROUP_SOURCE_FILES("Support/Parser")
LCSM_ADD_GROUP_SOURCE_FILES("Verilog/ModuleDeclare")

LCSM_ADD_GROUP_HEADER_FILES("lcsm")
LCSM_ADD_GROUP_HEADER_FILES("lcsm/Model")
LCSM_ADD_GROUP_HEADER_FILES("lcsm/Model/std")
LCSM_ADD_GROUP_HEADER_FILES("lcsm/Physical")
LCSM_ADD_GROUP_HEADER_FILES("lcsm/Physical/std")
LCSM_ADD_GROUP_HEADER_FILES("lcsm/Support")
LCSM_ADD_GROUP_HEADER_FILES("lcsm/Support/Parser")
LCSM_ADD_GROUP_HEADER_FILES("lcsm/Verilog")
LCSM_ADD_GROUP_HEADER_FILES("lcsm/Verilog/ModuleDeclare")

TARGET_INCLUDE_DIRECTORIES(LCSM
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

TARGET_SOURCES(LCSM
    PRIVATE
        ${LCSM_SOURCES}
        ${LCSM_HEADERS}
)

FOREACH(header ${LCSM_HEADERS})
    FILE(RELATIVE_PATH header_file_path "${CMAKE_CURRENT_SOURCE_DIR}/lcsm" "${header}")
    GET_FILENAME_COMPONENT(header_directory_path "${header_file_path}" DIRECTORY)
    INSTALL(
        FILES ${header}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${header_directory_path}"
    )
ENDFOREACH()

INSTALL(TARGETS LCSM
    EXPORT "LCSMTargets"
    COMPONENT "LogicalCircuitStateMachine"
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


# Generate the version file for the config file.
WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${CMAKE_CURRENT_BINARY_DIR}/LCSMConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

# Create config file.
CONFIGURE_PACKAGE_CONFIG_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LCSMConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
)

# Generate and install export file.
INSTALL(EXPORT "LCSMTargets"
    FILE "LCSMTargets.cmake"
    NAMESPACE LCSM::
    DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
)

# Install config files.
INSTALL(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LCSMConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LCSMConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
)

IF(LCSM_BUILD_DOCS)
    # Check, if Doxygen is installed.
    FIND_PACKAGE(Doxygen REQUIRED)

    # Set Doxygen input and output files.
    SET(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in")
    SET(DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out")

    # Generate DoxyFile from the input file.
    CONFIGURE_FILE(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    # Command for generating documentation from Doxygen config file.
    ADD_CUSTOM_TARGET(docs
        COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYFILE_OUT}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Generating LCSM API documentation with Doxygen"
        VERBATIM
    )
ENDIF()

IF(LCSM_BUILD_TESTS)
    ADD_SUBDIRECTORY(test)
ENDIF()
