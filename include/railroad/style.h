/*
 * style.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>

#include "railroad/DefaultFailure.h"
#include "railroad/DefaultSuccess.h"
#include "railroad/Result.h"
#include "railroad/bind.h"
#include "railroad/is_instantiation.h"

namespace railroad {
template <typename OutputType, typename HiddenType, typename InputType>
std::function<OutputType(InputType)> operator>>(
    std::function<HiddenType(InputType)> inner,
    std::function<OutputType(HiddenType)> outer) {
  return [inner, outer](InputType input) { return outer(inner(input)); };
}

/*
This is failing because:
"couldn't deduce template parameter ‘OutputFailureType’"

On a Partial S->S call

Need to evaluate how this is templated generically, or specialize to match each
type of bindr call
*/
template <typename InnerFunc, typename OuterFunc>
auto operator>>=(InnerFunc inner, OuterFunc outer)
    -> decltype(inner >> bindr(outer)) {
  static_assert(is_instantiation<std::function, InnerFunc>::value,
                "Need InnerFunc to be an instantiation of std::func");
  static_assert(is_instantiation<std::function, OuterFunc>::value,
                "Need OuterFunc to be an instantiation of std::func");
  return inner >> bindr(outer);
}

}  // namespace railroad
