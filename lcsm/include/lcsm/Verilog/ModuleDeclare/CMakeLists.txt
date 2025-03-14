# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_HEADERS
    COMMFILES Token.h Lexer.h Parser.h Context.h
)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
