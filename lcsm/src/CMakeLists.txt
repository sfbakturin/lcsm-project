# LCSM library sources.

# Add sources.
LCSM_ADD_SOURCE(
    LIST LCSM_SOURCES
    SOURCES LCSM.cpp LCSMCircuit.cpp LCSMCircuitView.cpp LCSMEngine.cpp LCSMState.cpp
)

# Add subdirectory sources.
ADD_SUBDIRECTORY(Model)
ADD_SUBDIRECTORY(Physical)
ADD_SUBDIRECTORY(Support)
ADD_SUBDIRECTORY(Verilog)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
