PROJECT(LCSMVisual
        LANGUAGES CXX
        VERSION 1.0.0
        DESCRIPTION "LCSM simple visualizer."
        HOMEPAGE_URL "https://github.com/sfbakturin/lcsm-project"
)

# Find includes in the build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# MOC, UIC & RCC.
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

# Find Qt5.
FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Widgets Core Gui LinguistTools)

# Visualizer.
ADD_EXECUTABLE(lcsm-visual)

SET(LCSM_VISUAL_LANGUAGES
    Languages/LogicalCircuitStateMachine_ru_RU.ts
)

QT_ADD_TRANSLATION(QM_FILES ${LCSM_VISUAL_LANGUAGES})
LCSM_BASE_SETUP_TARGET(lcsm-visual)

ADD_SUBDIRECTORY(Core)
ADD_SUBDIRECTORY(GUI)
ADD_SUBDIRECTORY(Items)
ADD_SUBDIRECTORY(View)

TARGET_SOURCES(lcsm-visual
    PRIVATE
        lcsm-visual.cpp
        ${LCSM_VISUAL_SOURCES}
        ${LCSM_VISUAL_HEADERS}
)

TARGET_INCLUDE_DIRECTORIES(lcsm-visual
    PUBLIC
    Core GUI Items View
)

# Link Qt5.
TARGET_LINK_LIBRARIES(lcsm-visual PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui LCSM)
