#ifndef COFFEE_MILL_IS_ALL
#define COFFEE_MILL_IS_ALL
#include <mill/util/meta.hpp>

namespace mill
{

template<template<typename...> class, typename ...>
struct is_all;

// is_all<Unary, Xs...>
template<template<typename> class F, typename X, typename ... Xs>
struct is_all<F, X, Xs...> :
conjunction<F<X>, typename is_all<F, Xs...>::type>::type {};
template<template<typename> class F, typename X>
struct is_all<F, X> : F<X>::type{};

// is_all<Binary, ARG1, Xs...>
template<template<typename, typename> class F, typename X, typename Y, typename ... Ys>
struct is_all<F, X, Y, Ys...> :
conjunction<F<X, Y>, typename is_all<F, X, Ys...>::type>::type{};
template<template<typename, typename> class F, typename X, typename Y>
struct is_all<F, X, Y> : F<X, Y>::type {};

} // mill
#endif /* COFFEE_MILL_IS_ALL */
