#ifndef COFFEE_MILL_META
#define COFFEE_MILL_META
#include <type_traits>
#include <mill/util/has_xxx.hpp>

namespace mill
{

// and
template<typename ... Ts>
struct conjunction : public std::true_type {};
template<typename T>
struct conjunction<T> : public T
{
    static_assert(has_v_value<T>::value, "conjunction requires parameter T"
        " to have static bool value named as `value`.");
    static_assert(has_t_type<T>::value,  "conjunction requires parameter T"
        " to have member type named as `type`.");
    static_assert(has_t_type<T>::value,  "conjunction requires parameter T"
        " to have member type named as `value_type`.");
};
template<typename T, typename ... Ts>
struct conjunction<T, Ts...> : public std::conditional<bool(T::value),
    conjunction<Ts...>, T>{
    static_assert(has_v_value<T>::value, "conjunction requires parameter T"
        " to have static bool value named as `value`.");
    static_assert(has_t_type<T>::value,  "conjunction requires parameter T"
        " to have member type named as `type`.");
    static_assert(has_t_type<T>::value,  "conjunction requires parameter T"
        " to have member type named as `value_type`.");
};

// or
template<typename ... Ts>
struct disjunction : public std::false_type {};
template<typename T>
struct disjunction<T> : public T
{
    static_assert(has_v_value<T>::value, "disjunction requires parameter T"
        " to have static bool value named as `value`.");
    static_assert(has_t_type<T>::value,  "disjunction requires parameter T"
        " to have member type named as `type`.");
    static_assert(has_t_type<T>::value,  "disjunction requires parameter T"
        " to have member type named as `value_type`.");
};
template<typename T, typename ... Ts>
struct disjunction<T, Ts...> : public std::conditional<bool(T::value),
    T, disjunction<Ts...>>{
    static_assert(has_v_value<T>::value, "disjunction requires parameter T"
        " to have static bool value named as `value`.");
    static_assert(has_t_type<T>::value,  "disjunction requires parameter T"
        " to have member type named as `type`.");
    static_assert(has_t_type<T>::value,  "disjunction requires parameter T"
        " to have member type named as `value_type`.");
};

template<typename T>
struct negation : public std::integral_constant<bool, !bool(T::value)> {};

}//mill
#endif// COFFEE_MILL_META
