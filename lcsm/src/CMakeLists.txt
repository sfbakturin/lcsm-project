ADD_LIBRARY(LCSM
    Model/Identifier.cpp
    Model/Wire.cpp

    Model/std/Constant.cpp
    Model/std/Ground.cpp
    Model/std/Power.cpp
    Model/std/Pin.cpp
    Model/std/Transistor.cpp
    Model/std/TransmissionGate.cpp
    Model/std/Clock.cpp
    Model/std/Tunnel.cpp
    Model/std/Button.cpp
    Model/std/Digit.cpp
    Model/std/Probe.cpp

    Verilog/Value.cpp
    Verilog/Strength.cpp
    Verilog/Bit.cpp

    Physical/DataBits.cpp
    Physical/Event.cpp
    Physical/Instruction.cpp
    Physical/Timestamp.cpp
    Physical/Context.cpp
    Physical/Evaluator.cpp
    Physical/EvaluatorNode.cpp

    Physical/std/PinInput.cpp
    Physical/std/PinOutput.cpp
    Physical/std/Constant.cpp
    Physical/std/Power.cpp
    Physical/std/Ground.cpp
    Physical/std/Wire.cpp

    LCSMBuilder.cpp
    LCSMCircuit.cpp
    LCSMState.cpp
    LCSMEngine.cpp
)

IF(MSVC)
    TARGET_COMPILE_OPTIONS(LCSM PRIVATE /W4)
ELSE()
    TARGET_COMPILE_OPTIONS(LCSM PRIVATE -Wall -Wextra -Wpedantic)
    # TARGET_COMPILE_OPTIONS(LCSM PUBLIC -Wall -Wextra -Wpedantic -fsanitize=address,undefined,leak)
    # target_link_options(LCSM PUBLIC -Wall -Wextra -Wpedantic -fsanitize=address,undefined,leak)
ENDIF()
