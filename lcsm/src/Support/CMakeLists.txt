# Add sources.
LCSM_ADD_SOURCE(
    LIST LCSM_SOURCES
    SOURCES CompletedProcess.cpp CommonSubprocessRun.cpp CommonRemoveFile.cpp
)

# Add system specific sources.
IF(WIN32)
    LCSM_ADD_SOURCE(
        LIST LCSM_SOURCES
        SOURCES WindowsSubprocessRun.cpp WindowsRemoveFile.cpp
    )
ELSEIF(LINUX)
    LCSM_ADD_SOURCE(
        LIST LCSM_SOURCES
        SOURCES LinuxSubprocessRun.cpp LinuxRemoveFile.cpp
    )
ENDIF()

# Add subdirectories sources.
ADD_SUBDIRECTORY(Parser)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
