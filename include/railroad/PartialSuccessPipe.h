/*
 * PartialSuccessPipe.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include "railroad/DefaultFailure.h"
#include "railroad/PartialSuccessResult.h"
#include "railroad/Result.h"
#include "railroad/abc.h"

namespace railroad {
namespace bind {

template <typename InputSuccessType, typename OutputSuccessType,
          typename InputFailureType = ::railroad::DefaultFailure,
          typename OutputFailureType = ::railroad::DefaultFailure>
class PartialSuccessPipe : public ::railroad::abc::Callable1<
                               Result<OutputSuccessType, OutputFailureType>,
                               Result<InputSuccessType, InputFailureType>> {
  using Callable =
      ::railroad::abc::Callable1<PartialSuccessResult<OutputSuccessType>,
                                 PartialSuccessResult<InputSuccessType>>;

 public:
  explicit PartialSuccessPipe(const Callable& impl) : impl_(impl) {}

  virtual Result<OutputSuccessType, OutputFailureType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    if (input.hasSuccess()) {
      return Result<OutputSuccessType, OutputFailureType>::Success(
          impl_(input.getSuccessPartial()), input.getFailurePartial());
    } else {
      return Result<OutputSuccessType, OutputFailureType>::Failure(
          input.getFailurePartial());
    }
  }

 private:
  const Callable& impl_;
};

}  // namespace bind
}  // namespace railroad
