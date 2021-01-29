#ifndef COFFEE_MILL_CALC_PCA_MODE
#define COFFEE_MILL_CALC_PCA_MODE
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_calc_pca_usage() noexcept;
int mode_calc_pca(std::deque<std::string_view> args);

} // mill
#endif /* COFFEE_MILL_CALC_PCA_MODE */
