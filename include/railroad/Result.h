/*
 * Result.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include "railroad/PartialFailureResult.h"
#include "railroad/PartialSuccessResult.h"

namespace railroad {

using result::PartialFailureResult;
using result::PartialSuccessResult;

template <typename SuccessType, typename FailureType>
class Result {
 public:
  Result() = delete;
  Result(const PartialSuccessResult<SuccessType>& success,
         const PartialFailureResult<FailureType>& failure)
      : success_(success), failure_(failure) {}
  Result(const SuccessType& success, const FailureType& failure)
      : success_(success), failure_(failure) {}

  bool hasSuccess() const { return static_cast<bool>(success_); }
  PartialSuccessResult<SuccessType> getSuccess() const { return success_; }
  bool hasFailure() const { return static_cast<bool>(failure_); }
  PartialFailureResult<FailureType> getFailure() const { return failure_; }

  static Result<SuccessType, FailureType> Success(
      const PartialSuccessResult<SuccessType>& happy) {
    return {happy, PartialFailureResult<FailureType>{}};
  }
  static Result<SuccessType, FailureType> Success(
      const PartialSuccessResult<SuccessType>& happy,
      const PartialFailureResult<FailureType>& sad) {
    return {happy, sad};
  }
  static Result<SuccessType, FailureType> Success(const SuccessType& happy) {
    return Success(PartialSuccessResult<SuccessType>{happy},
            PartialFailureResult<FailureType>{});
  }
  static Result<SuccessType, FailureType> Success(
      const SuccessType& happy, const PartialFailureResult<FailureType>& sad) {
    return {PartialSuccessResult<SuccessType>{happy}, sad};
  }

  static Result<SuccessType, FailureType> Failure(
      const PartialFailureResult<FailureType>& sad) {
    return {PartialSuccessResult<SuccessType>{}, sad};
  }
  static Result<SuccessType, FailureType> Failure(const FailureType& sad) {
    return {PartialSuccessResult<SuccessType>{},
            PartialFailureResult<FailureType>{sad}};
  }

 private:
  PartialSuccessResult<SuccessType> success_;
  PartialFailureResult<FailureType> failure_;
};

}  // namespace railroad
