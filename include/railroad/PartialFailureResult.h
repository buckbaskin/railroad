/*
 * PartialFailureResult.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <memory>

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

 private:
  std::shared_ptr<FailureType> impl_;
};

}  // namespace result
}  // namespace railroad
