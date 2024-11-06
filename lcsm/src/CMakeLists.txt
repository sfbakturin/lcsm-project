ADD_SUBDIRECTORY(Component)
ADD_SUBDIRECTORY(IR)
ADD_SUBDIRECTORY(Model)

LIST(
    APPEND

    LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES

    ${LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES_COMPONENT}
    ${LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES_IR}
    ${LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES_MODELS}

    CMAKE_PATH(ABSOLUTE_PATH SimContext.cpp)
    SimBuilder.cpp
    SimCircuit.cpp
    SimState.cpp
    SimEngine.cpp
)

SET(LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES ${LCSM_DETAILS_LCSM_MAIN_LIBRARY_SOURCES} PARENT_SCOPE)

ADD_LIBRARY(${PROJECT_NAME}
    Component/Identifier.cpp
    Component/Component.cpp
    Component/IOComponent.cpp
    Component/WiringComponent.cpp
    Component/CircuitComponent.cpp

    Model/Circuit/Constant.cpp
    Model/Circuit/Ground.cpp
    Model/Circuit/Power.cpp
    Model/Circuit/Pin.cpp
    Model/Circuit/Splitter.cpp
    Model/Circuit/Transistor.cpp
    Model/Circuit/TransmissionGate.cpp
    Model/Circuit/Clock.cpp

    Model/Wiring/Wire.cpp
    Model/Wiring/Tunnel.cpp

    Model/IO/Button.cpp
    Model/IO/Digit.cpp
    Model/IO/Probe.cpp

    Verilog/Value.cpp

    IR/DataBits.cpp

    IR/Instruction.cpp
    IR/Instruction/WriteValueInstruction.cpp
    IR/Instruction/BroadcastValueInstruction.cpp
    IR/Instruction/UpdateStateInstruction.cpp

    IR/CG/CGObject.cpp
    IR/CG/CGPin.cpp
    IR/CG/CGPinInput.cpp
    IR/CG/CGPinOutput.cpp
    IR/CG/CGConstant.cpp
    IR/CG/CGPower.cpp
    IR/CG/CGGround.cpp
    IR/CG/CGSplitter.cpp
    IR/CG/CGState.cpp
    IR/CG/CGTransistor.cpp
    IR/CG/CGWire.cpp
    IR/CG/CGNode.cpp
    IR/CG/CGStaticNode.cpp
    IR/CG/CGFastNode.cpp
    IR/CG/CGCompositeNode.cpp
    IR/CG/CGDynamicNode.cpp
    IR/CG/CG.cpp

    LCSMContext.cpp
    LCSMBuilder.cpp
    LCSMCircuit.cpp
    LCSMState.cpp
    LCSMEngine.cpp
)

IF(MSVC)
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE /W4)
ELSE()
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -ferror-limit=999)
ENDIF()
