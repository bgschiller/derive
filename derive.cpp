
#include "api.h"
#include "dirs.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Execution.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Refactoring/AtomicChange.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"
#include <iostream>
#include <variant>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

auto ShowyClassDecl =
    cxxRecordDecl(isSameOrDerivedFrom("Derive::show")).bind("cls");

class ShowCallback : public MatchFinder::MatchCallback {
public:
  void run(const MatchFinder::MatchResult &Result) {
    auto *cls = Result.Nodes.getNodeAs<clang::RecordDecl>("cls");
    SourceManager &sm = Result.Context->getSourceManager();
    if (!cls)
      return;
    SourceRange sourceRange = cls->getSourceRange();
    auto startLoc = sm.getPresumedLoc(sourceRange.getBegin());
    std::cout << "Examining " << startLoc.getFilename() << std::endl;
    auto fields = cls->fields();
    for (auto field : fields) {
      std::cout << field->getNameAsString() << ": "
                << field->getType().getAsString() << std::endl;
    }
    auto contents = Derive::api::sourceChunk(sm, sourceRange);
    std::cout << contents;
  }
};

// Set up the command line options
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::OptionCategory ToolTemplateCategory("derive options");

int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);

  CommonOptionsParser OptionsParser(argc, argv, ToolTemplateCategory);
  ClangTool tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  ArgumentsAdjuster ardj1 =
      getInsertArgumentAdjuster(derive_internal::clang_inc_dir1.c_str());
  ArgumentsAdjuster ardj2 =
      getInsertArgumentAdjuster(derive_internal::clang_inc_dir2.c_str());
  tool.appendArgumentsAdjuster(ardj1);
  tool.appendArgumentsAdjuster(ardj2);

  ast_matchers::MatchFinder finder;
  ShowCallback showCallback;
  finder.addMatcher(ShowyClassDecl, &showCallback);

  return tool.run(newFrontendActionFactory(&finder).get());
}
