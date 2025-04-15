# The LCSM Project

**L**ogical **C**ircuit **S**tate **M**achine - is a still in early development set of libraries and programs for constructing and simulating simple logic circuits with support for Verilog modules (values ​​and strengths of values) for educational purposes only.

The project will include (or already partially includes):

* [LCSM](lcsm/) - the core of everything, the main library of the project. *Currently in-development stage*.
* [LCSM Visual](lcsm-visual/) - the simplest editor and visualizer on Qt, implementing graphical display of construction and simulation. *Currently in-development stage*.

> [!WARNING]
> At any moment the LCSM's library interface may change significantly due to the early stage of development!

## How-to-build

You will need a compiler that supports [C++11](https://en.cppreference.com/w/cpp/11).

[Icarus Verilog v12](https://github.com/steveicarus/iverilog) is required at *runtime* (to compile and run Verilog imported module), make sure the commands are found: `iverilog`, `vvp`.

1. Check out LCSM project:

    ```bash
    git clone https://github.com/sfbakturin/lcsm-project.git
    ```

2. Configure build files:

    ```bash
    cd lcsm-project
    mkdir build && cd build
    cmake .. -G <generator> [options]
    ```

    Some common options:

    * `-D LCSM_BUILD_VISUAL=ON` — build LCSM's simplest visualizer. Default: no build.
    * `-D LCSM_BUILD_DOCS=ON` — build LCSM's API [Doxygen](https://doxygen.nl/) documentation Default: no build.

    Some additional options:

    * `-D LCSM_BUILD_TESTS=ON` — build LCSM's sanity tests Default: no build.

    CMake options:

    * `-D BUILD_SHARED_LIBS=ON` — build LCSM library as *shared* (*dynamic*). Default: build *static*.
    * `-D CMAKE_INSTALL_PREFIX=<path>` — installation prefix path. Default: see [CMake help](https://cmake.org/cmake/help/v3.31/variable/CMAKE_INSTALL_PREFIX.html).

3. Build LCSM:

    ```bash
    cmake --build . [--config <config>]
    # --config is required, if you're building with MSVC
    ```

4. **Not recommended yet**, but you can install it:

    ```bash
    cmake --install .
    ```

5. If you configured with build tests, you can run them:

    ```bash
    ctest -E "Verilog" # skip VerilogTest, since CTest considers it an error to start a child process
    ./lcsm/test/src/userend/VerilogTest # run VerilogTest
    ```
