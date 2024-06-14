# The Bisquit Programming Language

Bisquit is a first attempt at designing and implementing an interpreted language; Drawing inspiration from typical assembly languages and BASIC.

**For a quick tour of the language, check the PDF in the `/doc/` directory**, and check out the supplied example programs.

To build the interpreter, go to the root of the project directory and do:

    mkdir build && cd build
    cmake ..
    make
  
The bisquit_interpreter executable will then be built.

#### Usage:

To run a code file, do:
```
bisquit_interpreter <source file>
```
