PROJECT(LCSM LANGUAGES CXX VERSION 1.0.0)

ADD_LIBRARY(LCSM)
LCSM_BASE_SETUP_TARGET(LCSM)

# Helper to add list of sources.
FUNCTION(LCSM_ADD_SOURCE)
    SET(options)
    SET(oneValueArgs LIST)
    SET(multiValueArgs SOURCES)
    CMAKE_PARSE_ARGUMENTS(ARG
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
        ${ARGN}
    )
    SET(sourceAbsolutes "")
    FOREACH(source ${ARG_SOURCES})
        GET_FILENAME_COMPONENT(sourceAbsolute "${source}" ABSOLUTE)
        LIST(APPEND "${ARG_LIST}" "${sourceAbsolute}")
        LIST(APPEND sourceAbsolutes "${sourceAbsolute}")
    ENDFOREACH()
    SET(${ARG_LIST} "${${ARG_LIST}}" PARENT_SCOPE)
ENDFUNCTION()

# Helper to add list of headers.
FUNCTION(LCSM_ADD_HEADER)
    SET(options)
    SET(oneValueArgs LIST)
    SET(multiValueArgs HEADERS)
    CMAKE_PARSE_ARGUMENTS(ARG
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
        ${ARGN}
    )
    SET(headerAbsolutes "")
    FOREACH(header ${ARG_HEADERS})
        GET_FILENAME_COMPONENT(headerAbsolute "${header}" ABSOLUTE)
        LIST(APPEND "${ARG_LIST}" "${headerAbsolute}")
        LIST(APPEND headerAbsolutes "${headerAbsolute}")
    ENDFOREACH()
    SET(${ARG_LIST} "${${ARG_LIST}}" PARENT_SCOPE)
ENDFUNCTION()

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

SET(LCSM_PUBLIC_HEADERS "${LCSM_HEADERS}")
SET_TARGET_PROPERTIES(LCSM PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSON ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${LCSM_PUBLIC_HEADERS}"
)

FOREACH(header ${LCSM_PUBLIC_HEADERS})
    FILE(RELATIVE_PATH header_file_path "${CMAKE_CURRENT_SOURCE_DIR}/lcsm" "${header}")
    GET_FILENAME_COMPONENT(header_directory_path "${header_file_path}" DIRECTORY)
    INSTALL(
        FILES ${header}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${header_directory_path}"
    )
ENDFOREACH()

INSTALL(TARGETS LCSM
    EXPORT "LCSMTargets"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lcsm
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# generate and install export file
INSTALL(EXPORT "LCSMTargets"
    FILE "LCSMTargets.cmake"
    NAMESPACE ${namespace}::
    DESTINATION cmake
)

# generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LCSMConfigVersion.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
)
# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/LCSMConfig.cmake"
    INSTALL_DESTINATION cmake
)
# install config files
INSTALL(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LCSMConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LCSMConfigVersion.cmake"
    DESTINATION cmake
)

IF(LCSM_BUILD_TESTS)
    ADD_SUBDIRECTORY(test)
ENDIF()
