# LCSM Visual view-part sources.

# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST
        LCSM_VISUAL_SOURCES
    COMMFILES
        MainWindow.cpp
        DesignExplorerList.cpp
        PropertiesList.cpp
        SimulateDialog.cpp
        DesignDialog.cpp
)

# Add headers
LCSM_ADD_TO_TARGET(
    VARLIST
        LCSM_VISUAL_HEADERS
    COMMFILES
        MainWindow.h
        DesignExplorerList.h
        PropertiesList.h
        SimulateDialog.h
        DesignDialog.h
)

# Cache variables for parent scope.
SET(LCSM_VISUAL_SOURCES "${LCSM_VISUAL_SOURCES}" PARENT_SCOPE)
SET(LCSM_VISUAL_HEADERS "${LCSM_VISUAL_HEADERS}" PARENT_SCOPE)
