# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS Token.h Lexer.h Parser.h Context.h
)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
