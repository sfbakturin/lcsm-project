# Add sources.
LCSM_ADD_SOURCE(
    LIST LCSM_SOURCES
    SOURCES Circuit.cpp Identifier.cpp Wire.cpp VerilogModule.cpp
)

# Add subdirectory sources.
ADD_SUBDIRECTORY(std)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
