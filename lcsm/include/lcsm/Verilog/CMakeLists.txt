# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_HEADERS
    COMMFILES Bit.h Capacitive.h Module.h Port.h Strength.h Value.h
)

# Add headers from subdirectories.
ADD_SUBDIRECTORY(ModuleDeclare)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
