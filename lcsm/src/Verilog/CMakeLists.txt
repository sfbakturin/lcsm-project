# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_SOURCES
    COMMFILES Bit.cpp Strength.cpp Value.cpp Module.cpp Port.cpp PortType.cpp PortDirectionType.cpp
)

# Add sources from subdirectories.
ADD_SUBDIRECTORY(ModuleDeclare)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
