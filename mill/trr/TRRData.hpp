#ifndef COFFEE_MILL_TRR_DATA_HPP
#define COFFEE_MILL_TRR_DATA_HPP
#include <array>
#include <vector>
#include <optional>
#include <cstdint>

namespace mill
{

struct TRRHeader
{
    // magic number is 0x07C9 (1993)
    std::int32_t version;
    std::array<char, 80> title;
    std::int32_t ir_size;
    std::int32_t e_size;
    std::int32_t box_size;  // 0 if not exists.
    std::int32_t vir_size;
    std::int32_t pres_size;
    std::int32_t top_size;
    std::int32_t sym_size;
    std::int32_t x_size;
    std::int32_t v_size;
    std::int32_t f_size;
    std::int32_t natoms;
    std::int32_t step;
    std::int32_t nre;
    float        t;
    float        lambda;
};

template<typename coordT>
struct TRRSnapshot
{
    TRRHeader                            header;
    std::optional<std::array<coordT, 3>> box;
    std::optional<std::array<coordT, 3>> virial;
    std::optional<std::array<coordT, 3>> pressure;
    std::optional<std::vector<coordT>>   positions;
    std::optional<std::vector<coordT>>   velocities;
    std::optional<std::vector<coordT>>   forces;
};

} // mill
#endif//COFFEE_MILL_TRR_DATA_HPP
