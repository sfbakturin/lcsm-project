# Add sources.
LCSM_ADD_SOURCE(
    LIST LCSM_SOURCES
    SOURCES Bit.cpp Strength.cpp Value.cpp Module.cpp Port.cpp PortType.cpp IOType.cpp
)

# Add sources from subdirectories.
ADD_SUBDIRECTORY(ModuleDeclare)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
