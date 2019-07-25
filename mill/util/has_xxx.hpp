#ifndef COFFEE_MILL_HAS_XXX
#define COFFEE_MILL_HAS_XXX
#include <type_traits>

#ifndef COFFEE_MILL_HAS_MEMBER_TYPE
#define COFFEE_MILL_HAS_MEMBER_TYPE( name )                               \
    namespace detail {                                                    \
    struct has_ ## name ## _t_impl {                                      \
        template<class T> static std::true_type  check(typename T::name*);\
        template<class T> static std::false_type check(...);              \
    };                                                                    \
    }                                                                     \
    template<class T> struct has_t_ ## name :                             \
        decltype(detail::has_ ## name ## _t_impl::check<T>(nullptr)) {};  \
    /**/
#endif // HAS_MEMBER_TYPE

#ifndef COFFEE_MILL_HAS_STATIC_VALUE
#define COFFEE_MILL_HAS_STATIC_VALUE( name )                                 \
    namespace detail {                                                       \
    struct has_ ## name ## _v_impl {                                         \
        template<class T> static std::true_type  check(decltype(&T::name)*); \
        template<class T> static std::false_type check(...);                 \
    };                                                                       \
    }                                                                        \
    template<class T> struct has_v_ ## name :                                \
        decltype(detail::has_ ## name ## _v_impl::check<T>(nullptr)) {};     \
    /**/
#endif // HAS_STATIC_VALUE

namespace mill
{

COFFEE_MILL_HAS_MEMBER_TYPE(value_type) // mill::has_t_value_type
COFFEE_MILL_HAS_MEMBER_TYPE(type)       // mill::has_t_type
COFFEE_MILL_HAS_STATIC_VALUE(value)     // mill::has_v_value

static_assert(has_t_value_type<std::true_type>::value, "mill::has_t_value_type<true_type> failed!");
static_assert(has_t_type<std::true_type>::value,       "mill::has_t_type<true_type> failed!");
static_assert(has_v_value<std::true_type>::value,      "mill::has_v_value<true_type> failed!");
static_assert(has_t_value_type<int>::value == false,   "mill::has_t_value_type<int> failed!");
static_assert(has_t_type<int>::value == false,         "mill::has_t_type<int> failed!");
static_assert(has_v_value<int>::value == false,        "mill::has_v_value<int> failed!");

} // mill

#undef COFFEE_MILL_HAS_MEMBER_TYPE
#undef COFFEE_MILL_HAS_STATIC_VALUE
#endif// COFFEE_MILL_HAS_XXX
