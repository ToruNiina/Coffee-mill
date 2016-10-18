#ifndef COFFEE_MILL_VARIADIC_TMEPLATE
#define COFFEE_MILL_VARIADIC_TMEPLATE
#include <type_traits>

namespace mill
{

template<template<typename T1, typename T2> class T_cond,
         typename T_ref, typename T_front, typename ... T_args>
struct is_all
{
    constexpr static bool value = T_cond<T_ref, T_front>::value &&
        is_all<T_cond, T_ref, T_args...>::value;
};

template<template<typename T1, typename T2> class T_cond,
         typename T_ref, typename T_last>
struct is_all<T_cond, T_ref, T_last>
{
    constexpr static bool value = T_cond<T_ref, T_last>::value;
};



} // mill

#endif /* COFFEE_MILL_VARIADIC_TMEPLATE */
