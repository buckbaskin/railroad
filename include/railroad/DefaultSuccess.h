/*
 * DefaultSuccess.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <iostream>

namespace railroad {

class DefaultSuccess {
 public:
  explicit DefaultSuccess(bool passed = false) : passed_(passed) {}
  explicit operator bool() const { return passed_; }

 private:
  bool passed_ = false;
};

inline std::ostream& operator<<(std::ostream& out, const DefaultSuccess& df) {
  out << "DefaultSuccess(";
  if (static_cast<bool>(df)) {
    out << "passing";
  } else {
    out << "failing";
  }
  out << ")";
  return out;
}

}  // namespace railroad
