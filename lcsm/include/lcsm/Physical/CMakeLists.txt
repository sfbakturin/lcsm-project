# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_HEADERS
    COMMFILES Context.h DataBits.h Evaluator.h Event.h Instruction.h Timestamp.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(std)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
