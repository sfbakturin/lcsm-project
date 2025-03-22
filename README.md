# The LCSM Project

**L**ogical **C**ircuit **S**tate **M**achine - is a still in early development set of libraries and programs for constructing and simulating simple logic circuits with support for Verilog modules (values ​​and strengths of values) for educational purposes only.

The project will include (or already partially includes):

* [LCSM](lcsm/) - the core of everything, the main library of the project. *Currently in-development stage*.
* [LCSM Visual](lcsm-visual/) - the simplest editor and visualizer on Qt, implementing graphical display of construction and simulation. *Currently not started*.

> [!WARNING]
> At any moment the LCSM's library interface may change significantly due to the early stage of development!

## How-to-build

You will need a compiler that supports C++11 and [Icarus Verilog v12](https://github.com/steveicarus/iverilog) (at runtime), make sure the commands are found: `iverilog`, `vvp`.

1. Check out LCSM project:

    ```bash
    git clone https://github.com/sfbakturin/lcsm-project.git
    ```

2. Configure build files and build:

    ```bash
    cd lcsm-project/
    mkdir -v build
    cmake .. -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS=ON # -D LCSM_BUILD_TESTS=ON -- activate CTest's
    cmake --build .
    ```

3. **Not recommended yet**, but you can install it:

    ```bash
    cmake --install .
    ```

4. If you configured with build tests, you can run them:

    ```bash
    ctest -E "Verilog" # skip VerilogTest, since CTest considers it an error to start a child process
    ./lcsm/test/src/userend/VerilogTest # run VerilogTest
    ```
