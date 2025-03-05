# Add headers.
LCSM_ADD_HEADER(
    LIST LCSM_HEADERS
    HEADERS Algorithm.hpp Meta.hpp Numbers.hpp PointerView.hpp StaticArray.hpp
)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
