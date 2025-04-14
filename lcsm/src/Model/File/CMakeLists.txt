# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST
        LCSM_SOURCES
    COMMFILES
        Kind.cpp
        Token.cpp
        Lexer.cpp
        Reader.cpp
        Writer.cpp
)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
