LIST(
    APPEND

    LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES_IR_CG

    CGObject.cpp
    CGPin.cpp
    CGPinInput.cpp
    CGPinOutput.cpp
    CGConstant.cpp
    CGPower.cpp
    CGGround.cpp
    CGState.cpp
    CGTransistor.cpp
    CGWire.cpp
    CGEdge.cpp
    CGNode.cpp
    CGStaticNode.cpp
    CGFastNode.cpp
    CGCompositeNode.cpp
    CGDynamicNode.cpp
    CG.cpp
)

SET(LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES_IR_CG ${LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES_IR_CG} PARENT_SCOPE)
