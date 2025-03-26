# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSMTesting_HEADERS
    COMMFILES Asserts.h Test.h
)

# Cache variable for parent scope.
SET(LCSMTesting_HEADERS "${LCSMTesting_HEADERS}" PARENT_SCOPE)
