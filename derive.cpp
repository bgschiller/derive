
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

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

auto ShowyClassDecl =
    cxxRecordDecl(isSameOrDerivedFrom("Derive::show")).bind("cls");

class ShowCallback : public MatchFinder::MatchCallback {
public:
  ShowCallback(ExecutionContext &Context) : Context(Context) {}

  void run(const MatchFinder::MatchResult &Result) {
    auto *cls = Result.Nodes.getNodeAs<clang::RecordDecl>("cls");
    if (!cls)
      return;
    auto fields = cls->fields();
    for (auto field : fields) {
      std::cout << field->getNameAsString() << ": "
                << field->getType().getAsString() << std::endl;
    }
  }

private:
  ExecutionContext &Context;
};

// Set up the command line options
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::OptionCategory ToolTemplateCategory("tool-template options");

int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);

  auto Executor = clang::tooling::createExecutorFromCommandLineArgs(
      argc, argv, ToolTemplateCategory);

  if (!Executor) {
    llvm::errs() << llvm::toString(Executor.takeError()) << "\n";
    return 1;
  }

  ast_matchers::MatchFinder Finder;
  ShowCallback Callback(*Executor->get()->getExecutionContext());

  // TODO: Put your matchers here.
  // Use Finder.addMatcher(...) to define the patterns in the AST that you
  // want to match against. You are not limited to just one matcher!
  //
  // This is a sample matcher:
  Finder.addMatcher(ShowyClassDecl, &Callback);

  auto Err = Executor->get()->execute(newFrontendActionFactory(&Finder));
  if (Err) {
    llvm::errs() << llvm::toString(std::move(Err)) << "\n";
  }
  Executor->get()->getToolResults()->forEachResult(
      [](llvm::StringRef key, llvm::StringRef value) {
        llvm::errs() << "----" << key.str() << "\n" << value.str() << "\n";
      });
}
