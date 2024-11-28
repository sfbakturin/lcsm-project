ADD_LIBRARY(LCSM
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
    Verilog/Strength.cpp
    Verilog/Bit.cpp

    IR/DataBits.cpp

    IR/Instruction.cpp

    IR/CG/CGObject.cpp
    IR/CG/CGPin.cpp
    IR/CG/CGPinInput.cpp
    IR/CG/CGPinOutput.cpp
    IR/CG/CGConstant.cpp
    IR/CG/CGPower.cpp
    IR/CG/CGGround.cpp
    IR/CG/CGSplitter.cpp
    IR/CG/CGState.cpp
    IR/CG/CGCollector.cpp
    IR/CG/CGTransistor.cpp
    IR/CG/CGWire.cpp
    IR/CG/CGNode.cpp
    IR/CG/CG.cpp

    IR/Event.cpp

    LCSMBuilder.cpp
    LCSMCircuit.cpp
    LCSMState.cpp
    LCSMEngine.cpp
)

TARGET_LINK_LIBRARIES(LCSM PRIVATE LCSMC)

IF(MSVC)
    TARGET_COMPILE_OPTIONS(LCSM PRIVATE /W4)
ELSE()
    TARGET_COMPILE_OPTIONS(LCSM PRIVATE -Wall -Wextra -Wpedantic)
ENDIF()
