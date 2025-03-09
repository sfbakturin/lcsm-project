# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS Algorithm.hpp Meta.hpp Numbers.hpp PointerView.hpp StaticArray.hpp Subprocesses.h Files.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(Parser)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
