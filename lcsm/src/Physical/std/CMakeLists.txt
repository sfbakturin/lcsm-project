# Add sources.
LCSM_ADD_SOURCE(
    LIST LCSM_SOURCES
    SOURCES Button.cpp Clock.cpp Constant.cpp Digit.cpp Ground.cpp Pin.cpp Power.cpp Probe.cpp Splitter.cpp Transistor.cpp TransmissionGate.cpp Tunnel.cpp Wire.cpp Verilog.cpp
)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
