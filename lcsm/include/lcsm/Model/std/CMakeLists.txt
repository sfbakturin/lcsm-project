# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_HEADERS
    COMMFILES Button.h Clock.h Constant.h Digit.h Ground.h Pin.h Power.h Probe.h Splitter.h Transistor.h TransmissionGate.h Tunnel.h
)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
