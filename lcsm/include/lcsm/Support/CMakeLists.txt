# Add headers.
LCSM_ADD_TO_TARGET(
    VARLIST LCSM_HEADERS
    COMMFILES Algorithm.hpp Meta.hpp Numbers.hpp PointerView.hpp StaticArray.hpp Subprocesses.h Files.h
)

# Add subdirectories headers.
ADD_SUBDIRECTORY(IO)

# Cache variable for parent scope.
SET(LCSM_HEADERS "${LCSM_HEADERS}" PARENT_SCOPE)
