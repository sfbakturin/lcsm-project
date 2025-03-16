# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_SOURCES
    COMMFILES Circuit.cpp Identifier.cpp Wire.cpp VerilogModule.cpp Builder.cpp
)

# Add subdirectory sources.
ADD_SUBDIRECTORY(File)
ADD_SUBDIRECTORY(std)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
