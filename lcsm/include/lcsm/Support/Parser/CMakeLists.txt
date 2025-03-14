# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_HEADERS
    COMMFILES CharSource.h StringSource.h FileSource.h CStringSource.h
)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
