# LCSM library sources.

# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_SOURCES
    COMMFILES LCSM.cpp LCSMCircuit.cpp LCSMCircuitView.cpp LCSMEngine.cpp LCSMState.cpp
)

# Add subdirectory sources.
ADD_SUBDIRECTORY(Model)
ADD_SUBDIRECTORY(Physical)
ADD_SUBDIRECTORY(Support)
ADD_SUBDIRECTORY(Verilog)
ADD_SUBDIRECTORY(Testing)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
