# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_SOURCES
    COMMFILES CStringReader.cpp FileReader.cpp StringReader.cpp Writer.cpp StringBuilder.cpp FileWriter.cpp
)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
