# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS Circuit.h Identifier.h Width.h Wire.h Verilog.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(std)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
