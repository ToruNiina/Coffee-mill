#ifndef COFFEE_MILL_COMMON_ATTRIBUTES_HPP
#define COFFEE_MILL_COMMON_ATTRIBUTES_HPP
#include <mill/math/Vector.hpp>
#include <utility>
#include <variant>
#include <string>
#include <vector>
#include <optional>
#include <cstdint>

namespace mill
{

enum class AttributeKind: std::size_t
{
    empty    = 0,
    boolean  = 1,
    integer  = 2,
    floating = 3,
    string   = 4,
    vector   = 5,
    array    = 6,
};

class Attribute
{
  public:
    using boolean_type  = bool;
    using integer_type  = std::int64_t;
    using floating_type = double;
    using string_type   = std::string;
    using vector_type   = Vector<double, 3>;
    using array_type    = std::vector<Attribute>;
    using storage_type  = std::variant<
            std::monostate,
            boolean_type,
            integer_type,
            floating_type,
            string_type,
            vector_type,
            array_type
        >;

  public:

    Attribute() = default;
    ~Attribute() = default;
    Attribute(Attribute const&) = default;
    Attribute(Attribute &&)     = default;
    Attribute& operator=(Attribute const&) = default;
    Attribute& operator=(Attribute &&)     = default;

    Attribute(bool arg): storage_(arg) {}

    template<typename T, std::enable_if_t<std::conjunction_v<
        std::is_integral<std::remove_reference_t<T>>, std::negation<
            std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, bool>>
        >, std::nullptr_t> = nullptr>
    Attribute(T arg): storage_(std::in_place_type<integer_type>, arg) {}

    template<typename T, std::enable_if_t<std::is_floating_point_v<
        std::remove_cv_t<std::remove_reference_t<T>>>, std::nullptr_t> = nullptr>
    Attribute(T arg): storage_(std::in_place_type<floating_type>, arg) {}

    Attribute(const string_type& arg): storage_(arg) {}
    Attribute(string_type&& arg): storage_(std::move(arg)) {}
    Attribute(const char* arg): storage_(string_type(arg)) {}

    Attribute(const vector_type& v): storage_(v) {}
    Attribute(vector_type&& v): storage_(std::move(v)){}

    Attribute(const array_type& a): storage_(a) {}
    Attribute(array_type&& a): storage_(std::move(a)){}

    Attribute(std::initializer_list<Attribute> attr)
        : storage_(array_type(std::move(attr)))
    {}

    Attribute& operator=(bool arg)
    {
        this->storage_ = arg;
        return *this;
    }

    template<typename T, std::enable_if_t<std::conjunction_v<
        std::is_integral<std::remove_reference_t<T>>, std::negation<
            std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, bool>>
        >, std::nullptr_t> = nullptr>
    Attribute& operator=(T arg)
    {
        this->storage_.template emplace<integer_type>(arg);
        return *this;
    }

    template<typename T, std::enable_if_t<std::is_floating_point_v<
        std::remove_cv_t<std::remove_reference_t<T>>>, std::nullptr_t> = nullptr>
    Attribute& operator=(T arg)
    {
        this->storage_.template emplace<floating_type>(arg);
        return *this;
    }

    Attribute& operator=(const string_type& arg)
    {
        this->storage_.template emplace<string_type>(arg);
        return *this;
    }
    Attribute& operator=(string_type&& arg)
    {
        this->storage_.template emplace<string_type>(std::move(arg));
        return *this;
    }
    Attribute& operator=(const char* arg)
    {
        this->storage_.template emplace<string_type>(arg);
        return *this;
    }

    Attribute& operator=(const vector_type& v)
    {
        storage_.template emplace<vector_type>(v);
        return *this;
    }
    Attribute& operator=(vector_type&& v)
    {
        storage_.template emplace<vector_type>(std::move(v));
        return *this;
    }

    Attribute& operator=(const array_type& a)
    {
        storage_.template emplace<array_type>(a);
        return *this;
    }
    Attribute& operator=(array_type&& a)
    {
        storage_.template emplace<array_type>(std::move(a));
        return *this;
    }

    Attribute& operator=(std::initializer_list<Attribute> a)
    {
        storage_.template emplace<array_type>(std::move(a));
        return *this;
    }

    template<typename T>
    void emplace(T&& arg)
    {
        storage_ = std::forward<T>(arg);
    }
    template<typename T, typename ... Ts>
    void emplace(Ts&& ... args)
    {
        storage_.template emplace<T>(std::forward<Ts>(args)...);
    }

    void swap(Attribute& other)
    {
        using std::swap;
        swap(this->storage_, other.storage_);
        return;
    }

    bool empty() const noexcept {return this->is_empty();}
    void clear() {this->storage_ = std::monostate{};}

    bool is_empty()    const noexcept {return 0u == storage_.index();}
    bool is_boolean()  const noexcept {return 1u == storage_.index();}
    bool is_integer()  const noexcept {return 2u == storage_.index();}
    bool is_floating() const noexcept {return 3u == storage_.index();}
    bool is_string()   const noexcept {return 4u == storage_.index();}
    bool is_vector()   const noexcept {return 5u == storage_.index();}
    bool is_array()    const noexcept {return 6u == storage_.index();}

    boolean_type  const& as_boolean()  const& {return std::get<1>(storage_);}
    integer_type  const& as_integer()  const& {return std::get<2>(storage_);}
    floating_type const& as_floating() const& {return std::get<3>(storage_);}
    string_type   const& as_string()   const& {return std::get<4>(storage_);}
    vector_type   const& as_vector()   const& {return std::get<5>(storage_);}
    array_type    const& as_array()    const& {return std::get<6>(storage_);}

    boolean_type  &      as_boolean()  &      {return std::get<1>(storage_);}
    integer_type  &      as_integer()  &      {return std::get<2>(storage_);}
    floating_type &      as_floating() &      {return std::get<3>(storage_);}
    string_type   &      as_string()   &      {return std::get<4>(storage_);}
    vector_type   &      as_vector()   &      {return std::get<5>(storage_);}
    array_type    &      as_array()    &      {return std::get<6>(storage_);}

    boolean_type  &&     as_boolean()  &&     {return std::get<1>(std::move(storage_));}
    integer_type  &&     as_integer()  &&     {return std::get<2>(std::move(storage_));}
    floating_type &&     as_floating() &&     {return std::get<3>(std::move(storage_));}
    string_type   &&     as_string()   &&     {return std::get<4>(std::move(storage_));}
    vector_type   &&     as_vector()   &&     {return std::get<5>(std::move(storage_));}
    array_type    &&     as_array()    &&     {return std::get<6>(std::move(storage_));}

    std::optional<boolean_type > try_boolean()  const noexcept {if(is_boolean() ){return std::make_optional(as_boolean() );} return std::nullopt;}
    std::optional<integer_type > try_integer()  const noexcept {if(is_integer() ){return std::make_optional(as_integer() );} return std::nullopt;}
    std::optional<floating_type> try_floating() const noexcept {if(is_floating()){return std::make_optional(as_floating());} return std::nullopt;}
    std::optional<string_type  > try_string()   const noexcept {if(is_string()  ){return std::make_optional(as_string()  );} return std::nullopt;}
    std::optional<vector_type  > try_vector()   const noexcept {if(is_vector()  ){return std::make_optional(as_vector()  );} return std::nullopt;}
    std::optional<array_type   > try_array()    const noexcept {if(is_array()   ){return std::make_optional(as_array()   );} return std::nullopt;}

    AttributeKind which() const noexcept
    {
        return static_cast<AttributeKind>(storage_.index());
    }

    template<typename T>
    T get() const {return std::get<T>(storage_);}

    template<typename T>
    std::optional<T> try_get() const noexcept
    {
        try
        {
            return std::get<T>(storage_);
        }
        catch(const std::exception&)
        {
            return std::nullopt;
        }
    }
    template<typename T>
    T const& get_or(const T& v) const noexcept
    {
        try
        {
            return std::get<T>(storage_);
        }
        catch(const std::exception&)
        {
            return v;
        }
    }

    storage_type&       storage() &       {return storage_;}
    storage_type const& storage()  const& {return storage_;}
    storage_type&&      storage() &&      {return std::move(storage_);}

  private:

    storage_type storage_;
};

template<typename F, typename ... Variants>
auto visit(F&& f, Variants&& ... vs)
{
    return std::visit(std::forward<F>(f), std::forward<Variants>(vs).storage() ...);
}
template<typename R, typename F, typename ... Variants>
R visit(F&& f, Variants&& ... vs)
{
    return std::visit(std::forward<F>(f), std::forward<Variants>(vs).storage() ...);
}

} // mill
#endif // COFFEE_MILL_COMMON_ATTRIBUTES_HPP
