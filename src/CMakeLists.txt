ADD_LIBRARY(${PROJECT_NAME}
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

    IR/Bit.cpp
    IR/Value.cpp

    IR/Instruction.cpp
    IR/Instruction/WriteValueInstruction.cpp
    IR/Instruction/BroadcastValueInstruction.cpp
    IR/Instruction/UpdateStateInstruction.cpp
    IR/Instruction/WriteValueStabilizedInstruction.cpp

    IR/CG/CGObject.cpp
    IR/CG/CGPin.cpp
    IR/CG/CGPinInput.cpp
    IR/CG/CGPinOutput.cpp
    IR/CG/CGConstant.cpp
    IR/CG/CGPower.cpp
    IR/CG/CGGround.cpp
    IR/CG/CGWire.cpp
    IR/CG/CGEdge.cpp
    IR/CG/CGNode.cpp
    IR/CG/CG.cpp

    SimContext.cpp
    SimBuilder.cpp
    SimCircuit.cpp
    SimState.cpp
    SimEngine.cpp
)

IF(MSVC)
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE /W4)
ELSE()
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
ENDIF()
