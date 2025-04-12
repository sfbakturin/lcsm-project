# LCSM Visual core-part sources.

# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_VISUAL_SOURCES
    COMMFILES Project.cpp CoreScene.cpp Library.cpp
)

# Add headers
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_VISUAL_HEADERS
    COMMFILES Project.h CoreScene.h Library.h
)

# Cache variables for parent scope.
SET(LCSM_VISUAL_SOURCES "${LCSM_VISUAL_SOURCES}" PARENT_SCOPE)
SET(LCSM_VISUAL_HEADERS "${LCSM_VISUAL_HEADERS}" PARENT_SCOPE)
