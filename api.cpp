#include "api.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Lex/Lexer.h"

using namespace clang;

clang::LangOptions lopt;

namespace Derive::api {

std::string sourceChunk(const clang::SourceManager &sm,
                        clang::SourceRange range) {
  clang::SourceLocation b(range.getBegin()), _e(range.getEnd());

  if (b.isMacroID())
    b = sm.getSpellingLoc(b);
  if (_e.isMacroID())
    _e = sm.getSpellingLoc(_e);
  clang::SourceLocation e(clang::Lexer::getLocForEndOfToken(_e, 0, sm, lopt));
  return std::string(sm.getCharacterData(b),
                     sm.getCharacterData(e) - sm.getCharacterData(b));
}

// void SomethingElse::write(const SourceManager &sm, std::ostream &out) {
//   out << text;
// }
}
