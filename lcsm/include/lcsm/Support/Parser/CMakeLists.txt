# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS CharSource.h StringSource.h FileSource.h CFileSource.h CStringSource.h
)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
