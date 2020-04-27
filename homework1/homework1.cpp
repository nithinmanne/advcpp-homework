// homework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <type_traits>
#include <tuple>


template<typename, typename> struct Append;

template<template<typename...> typename TL, typename... Ts, typename... Us>
struct Append<TL<Ts...>, TL<Us...>>
{
    using type = TL<Ts..., Us...>;
};

template<typename T, typename U>
using Append_t = typename Append<T, U>::type;


template<typename, typename, typename> struct ReplaceAll;

template<template<typename...> typename TL, typename T, typename U>
struct ReplaceAll<TL<>, T, U> {
    using type = TL<>;
};

template<template<typename...> typename TL, typename... TL_Ts, typename T, typename U>
struct ReplaceAll<TL<T, TL_Ts...>, T, U> {
    using type = Append_t<TL<U>, typename ReplaceAll<TL<TL_Ts...>, T, U>::type>;
};

template<template<typename...> typename TL, typename TL_T, typename... TL_Ts, typename T, typename U>
struct ReplaceAll<TL<TL_T, TL_Ts...>, T, U> {
    using type = Append_t<TL<TL_T>, typename ReplaceAll<TL<TL_Ts...>, T, U>::type>;
};

template<typename TL, typename T, typename U>
using ReplaceAll_t = typename ReplaceAll<TL, T, U>::type;


template<typename> struct Reverse;

template<template<typename...> typename TL>
struct Reverse<TL<>> {
    using type = TL<>;
};

template<template<typename...> typename TL, typename T, typename... Ts>
struct Reverse<TL<T, Ts...>> {
    using type = Append_t<typename Reverse<TL<Ts...>>::type, TL<T>>;
};

template<typename TL>
using Reverse_t = typename Reverse<TL>::type;


int main()
{
    using std::is_same_v;
    using std::tuple;

    static_assert(is_same_v<ReplaceAll<tuple<char, int, char>, char, double>::type, tuple<double, int, double>>, "ReplaceAll::type Failure");

    using A = int;
    using B = double;
    static_assert(is_same_v<Reverse<tuple<A, B>>::type, tuple<B, A>>, "Reverse::type Failure");
}
