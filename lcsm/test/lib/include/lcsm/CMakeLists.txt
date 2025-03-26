# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSMTesting_HEADERS
    COMMFILES lcsmtestingconfig.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(Testing)

# Cache variable for parent scope.
SET(LCSMTesting_HEADERS "${LCSMTesting_HEADERS}" PARENT_SCOPE)
