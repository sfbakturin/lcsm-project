# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS Context.h DataBits.h Evaluator.h Event.h Instruction.h Timestamp.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(std)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
