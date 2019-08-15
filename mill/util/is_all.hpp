#ifndef COFFEE_MILL_UTIL_IS_ALL_HPP
#define COFFEE_MILL_UTIL_IS_ALL_HPP
#include <type_traits>

namespace mill
{

template<template<typename...> typename, typename ...>
struct is_all;

// is_all<Unary, Xs...>
template<template<typename> typename F, typename ... Xs>
struct is_all<F, Xs...> : std::bool_constant<std::conjunction_v<F<Xs>...>>{};

// is_all<Binary, ARG1, Xs...>
template<template<typename, typename> typename F, typename X, typename ... Ys>
struct is_all<F, X, Ys...> : std::bool_constant<std::conjunction_v<F<X, Ys>...>>{};

template<template<typename...> class F, typename ... Xs>
inline constexpr bool is_all_v = is_all<F, Xs...>::value;

// tests ...

static_assert(is_all_v<std::is_integral, int, unsigned int, long long>);
static_assert(std::negation_v<is_all<std::is_integral, int, double>>);

} // mill
#endif /* COFFEE_MILL_IS_ALL */
