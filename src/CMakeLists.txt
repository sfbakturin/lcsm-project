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

    Model/Value.cpp
)

IF(MSVC)
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE /W4)
ELSE()
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
ENDIF()
