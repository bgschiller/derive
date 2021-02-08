#pragma once

#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace Derive::api {

std::string sourceChunk(const clang::SourceManager &sm,
                        clang::SourceRange range);

// /**
//  * An opaque span of C++ source. Not something we
//  * intend to modify. */
// struct SomethingElse {
//   const SourceRange range;

//   void write(const clang::SourceManager &sm, std::ostream &out);
// };

// /**
//  * A header file has a file name, and is comprised of
//  * a sequence of ClassDecls, which we care about, and
//  * SomethingElse, which we don't. */
// struct Header {
//   const std::string inputFilename;
//   const SourceRange range;

//   std::vector<ClassDecl> contents;

//   void write(const clang::SourceManager &sm) const;
// };

// /**
//  * A ClassDecl inheriting from a derive class.
//  * Comprised of a sequence of Fields, GeneratedMethods, and other stuff
//  */
// struct ClassDecl {
//   const std::string fullyQualifiedName;
//   const std::string localName;
//   const std::string contents;
//   const std::vector<Field> fields;

//   const SourceRange range;
//   std::vector<GeneratedMethod> generatedMethods;

//   void write(const clang::SourceManager &sm, std::ostream &headerOut,
//              std::ostream &implOut) const;
// };

// enum class Visibility { Public, Protected, Private };

// struct Field {
//   const Visibility visibility;
//   const std::string name;
//   const std::string type;
//   const std::string contents;
// };

// struct GeneratedMethod {
//   const Visibility visibility;
//   const std::string declaration;
//   const std::string implementation;

//   void write(std::ostream &headerOut, std::ostream &implOut) const;
// };

} // namespace Derive::api
