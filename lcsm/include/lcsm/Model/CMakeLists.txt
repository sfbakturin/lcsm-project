# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_HEADERS
    COMMFILES Component.h Identifier.h Width.h Wire.h Verilog.h Builder.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(std)
ADD_SUBDIRECTORY(File)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
