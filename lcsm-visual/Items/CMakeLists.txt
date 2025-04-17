# LCSM Visual item-part sources.

# Add sources.
LCSM_ADD_TO_TARGET(
    VARLIST
        LCSM_VISUAL_SOURCES
    COMMFILES
        Item.cpp
        PinItem.cpp
        CircuitItem.cpp
        ComponentItem.cpp
        VerilogItem.cpp
        MovableItem.cpp
        ImmovableItem.cpp
        WireItem.cpp
        WireLine.cpp
        ConstantItem.cpp
        PowerItem.cpp
        GroundItem.cpp
        ClockItem.cpp
        SplitterItem.cpp
        TransistorItem.cpp
        TunnelItem.cpp
)

# Add headers
LCSM_ADD_TO_TARGET(
    VARLIST
        LCSM_VISUAL_HEADERS
    COMMFILES
        Item.h
        PinItem.h
        CircuitItem.h
        ComponentItem.h
        VerilogItem.h
        MovableItem.h
        ImmovableItem.h
        WireItem.h
        WireLine.h
        ConstantItem.h
        PowerItem.h
        GroundItem.h
        ClockItem.h
        SplitterItem.h
        TransistorItem.h
        TunnelItem.h
)

# Cache variables for parent scope.
SET(LCSM_VISUAL_SOURCES "${LCSM_VISUAL_SOURCES}" PARENT_SCOPE)
SET(LCSM_VISUAL_HEADERS "${LCSM_VISUAL_HEADERS}" PARENT_SCOPE)
