#include <string>

namespace derive_internal {
#ifdef CLANG_INC_DIR1
std::string const clang_inc_dir1_base(CLANG_INC_DIR1);
#else
std::string const clang_inc_dir1_base;
#endif
#ifdef CLANG_INC_DIR2
std::string const clang_inc_dir2_base(CLANG_INC_DIR2);
#else
std::string const clang_inc_dir2_base;
#endif
std::string const clang_inc_dir1("-I" + clang_inc_dir1_base);
std::string const clang_inc_dir2("-I" + clang_inc_dir2_base);
} // namespace derive_internal
