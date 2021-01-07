# Derive

Generate definitions for common boilerplate:

- `show` - toString()
- `equals` - `operator==()` based on value equality of members.
- `jsonable` - toJSON(), fromJSON(std::string)
- `guarded<T>` - protect all members with an assertion that a mutex `T` is held.

## What it does

Write a C++ struct and inherit from one of the classes in the `Derive` namespace. When you call the `derive` executable on the header file where this is defined, it will produce output implementing the boilerplate. For example,

```cpp
// input: showy-meal.derive.h
#include "derive.h"
#include <string>

struct Breakfast : Derive::show {
  int numPancakes;
  std::string beverage;
};
```

```bash
derive showy-meal.derive.h # invoke derive on the header
# (usually done automatically within CMake)
```

```cpp
// output: showy-meal.h
#include <string>

struct Breakfast {
   int numPancakes;
   std::string beverage;

   std::string toString() const;
};

// output: showy-meal.cpp
#include "showy-meal.h"
#include <sstream>

std::string Breakfast::toString() const {
   std::ostringstream output;
   output << "Breakfast("
      << "numPancakes=" << numPancakes
      << ", "
      << "beverage=" << beverage
      << ")";
   return output.str();
}
```

Multiple derivations can be combined:

```cpp
// input: book.derive.h
#include "derive.h"
#include <string>
#include <chrono>

struct Author : Derive::show, Derive::jsonable {
   std::string name;
   std::chrono::year_month_day dateOfBirth;
};

// output: book.h
#include <string>
#include <chrono>

namespace Library {
struct Author {
   std::string name;
   std::chrono::year_month_day dateOfBirth;

   std::string toString() const;

   static Author fromJSON(const std::string& json);

   std::string toJSON() const;
};
}

// output: book.cpp
#include "book.h"

#include <sstream>

#include "json.hpp"

using namespace Library;

std::string Author::toString() const {
   std::ostringstream output;
   output << "Author("
      << "name=" << name
      << ", "
      << "dateOfBirth=" << dateOfBirth
      << ")";
   return output.str();
}

// TODO: do we want to potentially return an error here
// if the author can't be parsed, or maybe has the wrong
// tag? throw an exception?
Author Author::fromJSON(const std::string& json) {
   auto jobj = nlohmann::json::parse(json);
   std::chrono::year_month_day ymd;
   std::istringstream dateOfBirthStream{jobj["dateOfBirth"].get<std::string>()};
   std::chrono::from_stream(dateOfBirthStream, "%F", ymd);
   return Author{
      jobj["name"].get<std::string>(),
      ymd
   };
}

std::string Author::toJSON() const {
   nlohmann::json jobj;
   jobj["name"] = name;
   jobj["dateOfBirth"] = std::chrono::format("%F", dateOfBirth);
   return jobj.dump();
}

```

## Building

To build the `derive` generator tool itself, not to use it.

1. First, get a copy of llvm set up
   1. Download llvm source `https://github.com/llvm/llvm-project`
   2. Make two build dirs inside llvm-project: `mkdir build-release && mkdir build-debug`
   3. cd into the `build-release` directory and build llvm with `cmake -G Ninja ../llvm-project/llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=host -DLLVM_BUILD_TOOLS=Off`
   4. cd into the `build-debug` directory and build llvm with `cmake -G Ninja ../llvm-project/llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_TARGETS_TO_BUILD=host -DLLVM_BUILD_TOOLS=Off`
2. Back in this directory, point derive at llvm
   1. Make build directories inside `derive`: `mkdir build-debug && mkdir build-release`.
   2. within `derive/build-debug`, run `LLVM_INSTALL_PREFIX=<path to your llvm build-debug dir> cmake -GNinja ..`.
   3. within `derive/build-release`, run `LLVM_INSTALL_PREFIX=<path to your llvm build-release dir> cmake -GNinja ..`.
3. run `ninja` in the build directory of your choice.
   You can use build-debug for quick-to-compile that include debug symbols, or build-debug for slow-to-compile but more optimized builds.
