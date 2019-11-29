/*
 * is_instantiation.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <type_traits>

namespace railroad {
template <template <typename...> class, typename...>
struct is_instantiation : public std::false_type {};

template <template <typename...> class U, typename... T>
struct is_instantiation<U, U<T...>> : public std::true_type {};
}  // namespace railroad
