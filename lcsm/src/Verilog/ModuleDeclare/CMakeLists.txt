# Add sources.
LCSM_ADD_SOURCE(
    LIST LCSM_SOURCES
    SOURCES Kind.cpp Token.cpp Lexer.cpp Parser.cpp
)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
