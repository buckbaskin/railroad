/*
 * DefaultFailure.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <iostream>

namespace railroad {

class DefaultFailure {
 public:
  explicit DefaultFailure(bool failed = false) : failed_(failed) {}
  explicit operator bool() const { return failed_; }

 private:
  bool failed_ = false;
};

inline std::ostream& operator<<(std::ostream& out, const DefaultFailure& df) {
  out << "DefaultFailure(";
  if (static_cast<bool>(df)) {
    out << "failing";
  } else {
    out << "passing";
  }
  out << ")";
  return out;
}

}  // namespace railroad
