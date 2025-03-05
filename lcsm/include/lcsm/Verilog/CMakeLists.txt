# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS Bit.h Capacitive.h Module.h Port.h Strength.h Template.h Value.h
)

# Add headers from subdirectories.
ADD_SUBDIRECTORY(ModuleDeclare)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
