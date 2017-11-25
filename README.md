# The Human Plus project

## How to generate the project with CMake

To make Human Plus work, the first thing you need to do is to download the latest version of LLVM source from [their site](http://llvm.org/), or clone [their git repository](https://git.llvm.org/git/llvm), (or the [mirror on GitHub](https://github.com/llvm-mirror/llvm)).

Build LLVM with CMake in a directory, and use your build system to compile the **entire project**.
Please note that LLVM is required for the Human Plus Compiler. You can avoid this project if this is not in your intentions.

Now you can build Human Plus with CMake by passing the directory containing the LLVM **build** (not the source) with the ``LLVM_BUILD_DIR`` variable.
