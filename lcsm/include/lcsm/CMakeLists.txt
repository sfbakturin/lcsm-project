# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS LCSM.h LCSMCircuit.h LCSMEngine.h LCSMState.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(Model)
ADD_SUBDIRECTORY(Physical)
ADD_SUBDIRECTORY(Support)
ADD_SUBDIRECTORY(Verilog)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
