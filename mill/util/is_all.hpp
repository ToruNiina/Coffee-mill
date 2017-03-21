#ifndef COFFEE_MILL_VARIADIC_TMEPLATE
#define COFFEE_MILL_VARIADIC_TMEPLATE
#include "meta.hpp"

namespace mill
{

template<template<typename...> class, typename ...>
struct is_all;

template<template<typename> class condT, typename frontT, typename ... Ts>
struct is_all<condT, frontT, Ts...>
    : public _and_<condT<frontT>, is_all<condT, Ts...>>::type
{};

template<template<typename> class condT, typename lastT>
struct is_all<condT, lastT>
    : public condT<lastT>::type
{};

template<template<typename, typename> class condT, typename refT,
         typename frontT, typename ... Ts>
struct is_all<condT, refT, frontT, Ts...>
    : public _and_<condT<refT, frontT>, is_all<condT, refT, Ts...>>::type
{};

template<template<typename, typename> class condT, typename refT, typename lastT>
struct is_all<condT, refT, lastT>
    : public condT<refT, lastT>::type
{};

} // mill

#endif /* COFFEE_MILL_VARIADIC_TMEPLATE */
