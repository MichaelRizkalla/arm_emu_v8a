# arm_emu_v8a
It contains my attempt to emulate a arm v8-a architecture written in C++20 (incomplete as of this moment)

## Documentation and credits
This project is inspired by [cpp_box](https://github.com/lefticus/cpp_box/) and use the concepts of it as its bases.
It is only implementing AARCH64 on ARM v8-a CPUs based on the following documentations:
* https://developer.arm.com/documentation/ddi0487/fc/
* https://developer.arm.com/architectures/learn-the-architecture/armv8-a-instruction-set-architecture

# Getting started
This code requires the standard C++20 only.

# Building and Running
To compile using the default system compiler

On windows, use the following to generate VS Solution:
```
mkdir build 
cd build
cmake ..
```

On Unix-based OS, use the following to compile the app:
```
mkdir build && cd build
cmake ..
make
```

To run, just provide a txt file path as an argument.
```
./ARM_EMU [file_path]
```

# Using the emulator
For now, the app takes a txt file formatted like the following
```
0x00000000
0x00000000
... // remaining instructions
```
where each line represents a single instruction and executes the code.

# Future development
As this project is incomplete, the future development goals are as follows:
* Complete the decoded/operations for the remaining instructions
* Add support to provide only .cpp files to run the architecture
* Increase performance by replacing std::bitset<> with std::uint64_t
* Implements some of the features (e.g. FEAT_MTE)


# Remarks
Thanks to [Jason Turner](https://github.com/lefticus/)'s talk that provoked me to pick a hard project to work on it. And his project *cpp_box* which I used as ground zero.

# License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
