/*
 * FailurePipe.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <string>

#include "railroad/Result.h"
#include "railroad/abc.h"

namespace railroad {
namespace bind {

template <typename InputSuccessType, typename OutputSuccessType,
          typename InputFailureType = std::string,
          typename OutputFailureType = std::string>
class FailurePipe : public ::railroad::abc::Callable1<
                        Result<OutputSuccessType, OutputFailureType>,
                        Result<InputSuccessType, InputFailureType>> {
  using Callable =
      ::railroad::abc::Callable1<OutputFailureType, InputFailureType>;

 public:
  explicit FailurePipe(const Callable& impl) : impl_(impl) {}

  virtual Result<OutputSuccessType, OutputFailureType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    if (input.hasSuccess()) {
      return Result<OutputSuccessType, OutputFailureType>::Success(
          input.getSuccess());
    } else {
      return Result<OutputSuccessType, OutputFailureType>::Failure(
          input.getSuccess(), impl_(input.getFailure().unpack()));
    }
  }

 private:
  const Callable& impl_;
};

}  // namespace bind
}  // namespace railroad