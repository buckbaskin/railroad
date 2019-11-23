/*
 * terminate.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>
#include <optional>

#include "railroad/DefaultFailure.h"
#include "railroad/DefaultSuccess.h"
#include "railroad/Result.h"

namespace railroad {
namespace helpers {
template <typename OutputType,
          typename OutputFailureType = ::railroad::DefaultFailure>
std::optional<OutputType> terminateSuccess(
    Result<OutputType, OutputFailureType> input) {
  if (input.hasSuccess()) {
    return input.getSuccess();
  }
  return {};
}

template <typename OutputType = ::railroad::DefaultSuccess,
          typename OutputFailureType>
std::optional<OutputFailureType> terminateFailure(
    Result<OutputType, OutputFailureType> input) {
  if (input.hasSuccess()) {
    return {};
  } else if (input.hasFailure()) {
    return input.getFailure();
  }
  return {};
}
}  // namespace helpers

namespace unsafe {

template <typename OutputType,
          typename OutputFailureType = ::railroad::DefaultFailure>
OutputType terminateSuccess(Result<OutputType, OutputFailureType> input) {
  return input.getSuccess();
}

template <typename OutputType = ::railroad::DefaultSuccess,
          typename OutputFailureType>
OutputFailureType terminateFailure(
    Result<OutputType, OutputFailureType> input) {
  return input.getFailure();
}

}  // namespace unsafe
}  // namespace railroad
