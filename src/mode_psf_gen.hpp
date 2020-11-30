#ifndef COFFEE_MILL_PSF_GEN_HPP
#define COFFEE_MILL_PSF_GEN_HPP
#include <string_view>
#include <deque>

namespace mill
{

const char* mode_psf_gen_usage() noexcept;
int mode_psf_gen(std::deque<std::string_view> args);

} // mill
#endif  //COFFEE_MILL_PSF_GEN
