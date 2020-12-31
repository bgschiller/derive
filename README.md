# Derive

Generate definitions for common boilerplate:

- `show` - toString()
- `equals` - `operator==()` based on value equality of members.
- `jsonable` - toJSON(), fromJSON(std::string)
- `guarded<T>` - protect all members with an assertion that a mutex `T` is held.

## Building

To build the `derive` generator tool itself, not to use it.

1. First, get a copy of llvm set up
   1. Download llvm source `https://github.com/llvm/llvm-project`
   2. Make two build dirs inside llvm-project: `mkdir build-release && mkdir build-debug`
   3. cd into the `build-release` directory and build llvm with `cmake -G Ninja ../llvm-project/llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=host -DLLVM_BUILD_TOOLS=Off`
   4. cd into the `build-debug` directory and build llvm with `cmake -G Ninja ../llvm-project/llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_TARGETS_TO_BUILD=host -DLLVM_BUILD_TOOLS=Off`
2. Back in this directory, point derive at llvm
   1. Make build directories inside `derive`: `mkdir build-debug && mkdir build-release`.
   2. within `derive/build-debug`, run `LLVM_INSTALL_PREFIX=<path to your llvm build-debug dir> cmake ..`.
   3. within `derive/build-release`, run `LLVM_INSTALL_PREFIX=<path to your llvm build-release dir> cmake ..`.
3. run `make` in the build directory of your choice.
   You can use build-debug for quick-to-compile that include debug symbols, or build-debug for slow-to-compile but more optimized builds.
