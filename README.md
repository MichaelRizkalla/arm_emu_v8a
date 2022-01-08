# arm_emu_v8a
It contains my attempt to emulate an arm v8-a architecture, this project is for learning purposes, and not intended to be used in a professional environment.
However, this project can be used to explore how code is executed on ARM CPU, and to read the underlying generated code.

The project is separated into 3 different libraries `arm_emu_decodes_lib`, `arm_emu_lib`, and `compilation_lib`, and an executable that uses these libraries to create a GUI.

## Documentation and credits
This project is inspired by [cpp_box](https://github.com/lefticus/cpp_box/) and use the concepts of it as its bases.
It will be implementing parts of AARCH64 only on ARM v8-a CPUs based on the following documentations:
* https://developer.arm.com/documentation/ddi0487/fc/
* https://developer.arm.com/architectures/learn-the-architecture/armv8-a-instruction-set-architecture

# Dependencies
On linux, to compile the binaries, please install the following libraries:
```
// libx11-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev, and mesa-common-dev
sudo apt-get install libx11-dev libxrandr-dev \
		libxinerama-dev libxcursor-dev libxi-dev mesa-common-dev
```

On windows, make sure OpenGL is installed (usually is installed with the Graphics driver)

# Building
1- Clone the repo
```
git clone https://github.com/MichaelRizkalla/arm_emu_v8a.git
```
2- Init all submodules
```
git submodule init
git submodule update
```
3- Build

On Linux: 
```
mkdir build && cd build
cmake ..
make
```

On Windows:
```
mkdir build 
cd build
cmake ..
```

# Getting started
1. Install clang compiler v11 or later to be used later.
2. Build the project using a C++20 complaint compiler
3. Run `arm_emu` executable
3. Create a CPU from Edit->Create CPU
4. Load a Clang compiler from Edit->Load Compiler
5. Write your code, compile and load.
6. Step in the program and view CPU registers and stack changes

# Remarks
Thanks to [Jason Turner](https://github.com/lefticus/)'s talk "[Applied Best Practices](https://www.youtube.com/watch?v=DHOlsEd0eDE&t=3366s)" that provoked me to pick a hard project to work on it. And his project *cpp_box* which I used as ground zero.

# Future work
- Write documentation for each library
- Add unit tests and a better CI pipeline (currently almost no unit tests implemented)
- Support more instructions.
- Rework the GUI application as the current event system is not optimal. (Probably move to Qt as well)
- Extend compilation_lib to support more features during reading ELF files.

# License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
