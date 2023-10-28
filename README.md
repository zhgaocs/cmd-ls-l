# cmd-ls-l

An implementation of the Linux `ls -l` command in C.

## Project Structure

- `include`: Header files.
  - `auxf.h`: Header file for auxiliary functions.
- `src`: Source code files.
  - `auxf.c`: Source code for auxiliary functions.
  - `main.c`: Main program source code.
- `CMakeLists.txt`: CMake configuration file.
- `README.md`: This file.

## Description

`cmd-ls-l` is a C program that mimics the behavior of the Linux `ls -l` command. It lists the contents of a directory with detailed information about each file and directory.

## Features

- Lists files and directories in a specified directory.
- Displays file permissions, owner, group, size, modification date, and file/directory name.

## Building and Running

To build and run the project, follow these steps:

1. Create a build directory:

```shell
mkdir build
cd build
```

2. Generate the build files using CMake:

```shell
cmake ..
```

3. Build the project:
   
```shell
make
```

4. Run the program
