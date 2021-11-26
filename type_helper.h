
#include <type_traits>

namespace {

// helper to test if a type T has a member variable a
template <typename T, typename = void>
struct has_a_member : std::false_type
{};

template <typename T>
struct has_a_member<T, decltype((void)T::a, void())> : std::true_type
{};

// helper to test if a type T has a member variable b
template <typename T, typename = void>
struct has_b_member : std::false_type
{};

template <typename T>
struct has_b_member<T, decltype((void)T::b, void())> : std::true_type
{};

// helper to test if a type T has a member variable c
template <typename T, typename = void>
struct has_c_member : std::false_type
{};

template <typename T>
struct has_c_member<T, decltype((void)T::c, void())> : std::true_type
{};

} // namespace
