# # Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_SOURCES
    COMMFILES CompletedProcess.cpp CommonSubprocessRun.cpp CommonRemoveFile.cpp Strings.cpp
    ONLYWINDOWS WindowsSubprocessRun.cpp WindowsRemoveFile.cpp
    ONLYLINUX LinuxSubprocessRun.cpp LinuxRemoveFile.cpp
)

# Add subdirectories sources.
ADD_SUBDIRECTORY(IO)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
