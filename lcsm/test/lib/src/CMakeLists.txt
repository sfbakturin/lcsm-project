# LCSM Testing library sources.

# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST LCSMTesting_SOURCES
    COMMFILES Asserts.cpp Test.cpp
)

# Cache variable for parent scope.
SET(LCSMTesting_SOURCES "${LCSMTesting_SOURCES}" PARENT_SCOPE)
