/*
 * feed.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>

#include "railroad/DefaultFailure.h"
#include "railroad/DefaultSuccess.h"
#include "railroad/Result.h"

namespace railroad {
namespace helpers {

template <typename InputType,
          typename InputFailureType = ::railroad::DefaultFailure>
Result<InputType, InputFailureType> feedSuccess(InputType input) {
  return Success<InputType, InputFailureType>(input);
}

template <typename InputType = ::railroad::DefaultSuccess,
          typename InputFailureType>
Result<InputType, InputFailureType> feedFailure(InputFailureType input) {
  return Failure<InputType, InputFailureType>(input);
}

}  // namespace helpers
}  // namespace railroad
