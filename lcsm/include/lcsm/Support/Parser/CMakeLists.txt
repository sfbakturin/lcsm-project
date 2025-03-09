# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS CharSource.h StringSource.h FileSource.h CStringSource.h
)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
