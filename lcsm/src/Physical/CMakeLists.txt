# Add sources.
LCSM_ADD_SOURCE(
    LIST LCSM_SOURCES
    SOURCES Context.cpp DataBits.cpp EvaluatorNode.cpp Event.cpp Instruction.cpp PrivateContext.cpp Timestamp.cpp
)

# Add subdirectory sources.
ADD_SUBDIRECTORY(std)

# Cache variable for parent scope.
SET(LCSM_SOURCES "${LCSM_SOURCES}" PARENT_SCOPE)
