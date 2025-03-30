# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_HEADERS
    COMMFILES lcsmconfig.h LCSM.h LCSMCircuit.h LCSMEngine.h LCSMState.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(Model)
ADD_SUBDIRECTORY(Physical)
ADD_SUBDIRECTORY(Support)
ADD_SUBDIRECTORY(Verilog)
ADD_SUBDIRECTORY(Testing)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
