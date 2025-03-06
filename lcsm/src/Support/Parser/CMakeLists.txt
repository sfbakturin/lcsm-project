# Add sources.
LCSM_ADD_SOURCE(
    LIST LCSM_SOURCES
    SOURCES CFileSource.cpp CStringSource.cpp FileSource.cpp StringSource.cpp
)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
