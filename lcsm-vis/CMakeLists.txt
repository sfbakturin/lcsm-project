# Find includes in the build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# MOC, UIC & RCC.
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

# Find Qt5.
FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Widgets)

# Visualizer.
ADD_EXECUTABLE(lcsm-vis
	lcsm-vis.cpp

	view/mainwindow.h
	view/mainwindow.cpp
	view/mainwindow.ui
)

TARGET_INCLUDE_DIRECTORIES(lcsm-vis PUBLIC view)

# Link Qt5.
TARGET_LINK_LIBRARIES(lcsm-vis PRIVATE Qt5::Widgets)
