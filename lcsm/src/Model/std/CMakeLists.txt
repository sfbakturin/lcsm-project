# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_SOURCES
    COMMFILES Button.cpp Clock.cpp Constant.cpp Digit.cpp Ground.cpp Pin.cpp Power.cpp Probe.cpp Splitter.cpp Transistor.cpp TransmissionGate.cpp Tunnel.cpp
)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
