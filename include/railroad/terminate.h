/*
 * terminations.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <optional>

namespace railroad {
namespace terminate {
namespace with {
template <typename InputSuccessType, typename InputFailureType>
class SuccessOnly : public ::railroad::abc::Callable1<
                        std::optional<InputSuccessType>,
                        Result<InputSuccessType, InputFailureType>> {
 public:
  virtual std::optional<InputSuccessType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    if (input.hasSuccess()) {
      return input.getSuccess().unpack();
    }
    return {};
  }
};

template <typename InputSuccessType, typename InputFailureType>
class FailureOnly : public ::railroad::abc::Callable1<
                        std::optional<InputFailureType>,
                        Result<InputSuccessType, InputFailureType>> {
 public:
  virtual std::optional<InputFailureType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    if (input.hasFailure()) {
      return input.getFailure().unpack();
    }
    return {};
  }
};
}  // namespace with

namespace unsafe {
template <typename InputSuccessType, typename InputFailureType>
class WithUnsafeSuccessOnly
    : public ::railroad::abc::Callable1<
          InputSuccessType, Result<InputSuccessType, InputFailureType>> {
 public:
  virtual InputSuccessType operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    return input.getSuccess().unpack();
  }
};

template <typename InputSuccessType, typename InputFailureType>
class WithUnsafeFailureOnly
    : public ::railroad::abc::Callable1<
          InputFailureType, Result<InputSuccessType, InputFailureType>> {
 public:
  virtual InputFailureType operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    return input.getFailure().unpack();
  }
};

}  // namespace unsafe
}  // namespace terminate
}  // namespace railroad
