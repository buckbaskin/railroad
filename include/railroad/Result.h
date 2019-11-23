/*
 * Result.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <iostream>

#include "railroad/PartialFailureResult.h"
#include "railroad/PartialSuccessResult.h"

namespace railroad {

using result::PartialFailureResult;
using result::PartialSuccessResult;

template <typename SuccessType, typename FailureType>
class Result {
 public:
  Result() = default;
  Result(const PartialSuccessResult<SuccessType>& success,
         const PartialFailureResult<FailureType>& failure)
      : success_(success), failure_(failure) {}
  Result(const SuccessType& success, const FailureType& failure)
      : success_(success), failure_(failure) {}

  bool hasSuccess() const { return static_cast<bool>(success_); }
  PartialSuccessResult<SuccessType> getSuccessPartial() const {
    return success_;
  }
  SuccessType getSuccess() const { return success_.unpack(); }
  bool hasFailure() const { return static_cast<bool>(failure_); }
  PartialFailureResult<FailureType> getFailurePartial() const {
    return failure_;
  }
  FailureType getFailure() const { return failure_.unpack(); }

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
  static Result<SuccessType, FailureType> Failure(
      const PartialSuccessResult<SuccessType>& happy,
      const PartialFailureResult<FailureType>& sad) {
    return {happy, sad};
  }
  static Result<SuccessType, FailureType> Failure(const FailureType& sad) {
    return Failure(PartialSuccessResult<SuccessType>{},
                   PartialFailureResult<FailureType>{sad});
  }
  static Result<SuccessType, FailureType> Failure(
      const PartialSuccessResult<SuccessType>& happy, const FailureType& sad) {
    return {happy, PartialFailureResult<FailureType>{sad}};
  }

  template <typename S, typename F>
  friend std::ostream& operator<<(std::ostream& out, const Result<S, F>& r);

 private:
  PartialSuccessResult<SuccessType> success_;
  PartialFailureResult<FailureType> failure_;
};

template <typename SuccessType, typename FailureType>
std::ostream& operator<<(std::ostream& out,
                         const Result<SuccessType, FailureType>& r) {
  out << "Result<>(" << r.success_ << ", " << r.failure_ << ")";
  return out;
}

}  // namespace railroad
