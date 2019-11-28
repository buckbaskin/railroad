/*
 * PartialFailureResult.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <memory>

#include "railroad/DefaultFailure.h"

namespace railroad {
namespace result {

template <typename FailureType>
class PartialFailureResult {
 public:
  PartialFailureResult() : impl_(nullptr) {}
  explicit PartialFailureResult(const FailureType& example)
      : PartialFailureResult(new FailureType(example)) {}
  explicit PartialFailureResult(FailureType* impl) : impl_(impl) {}

  explicit operator bool() const { return static_cast<bool>(impl_); }

  FailureType unpack() const { return *impl_; }

  template <typename F>
  friend std::ostream& operator<<(std::ostream& out,
                                  const PartialFailureResult<F>& pfr);

 private:
  std::shared_ptr<FailureType> impl_;
};

template <typename FailureType>
inline std::ostream& operator<<(std::ostream& out,
                                const PartialFailureResult<FailureType>& pfr) {
  out << "Failure?<>(";
  if (!static_cast<bool>(pfr.impl_)) {
    out << "null";
  } else {
    out << *(pfr.impl_);
  }
  out << ")";
  return out;
}

}  // namespace result
}  // namespace railroad
