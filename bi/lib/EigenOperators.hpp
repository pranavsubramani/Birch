/**
 * @file
 */
#pragma once

#include "bi/lib/Eigen.hpp"
#include "bi/lib/Array.hpp"

#include <type_traits>

#define UNARY_OPERATOR(op) \
  template<class Type, class Frame> \
  auto operator op(const bi::Array<Type,Frame>& x) { \
    return op x.toEigen(); \
  }

UNARY_OPERATOR(+)
UNARY_OPERATOR(-)

#define BINARY_OPERATOR(op) \
  template<class Type1, class Frame1, class Type2, class Frame2> \
  auto operator op(const bi::Array<Type1,Frame1>& x, \
      const bi::Array<Type2,Frame2>& y) { \
    return x.toEigen() op y.toEigen(); \
  } \
  \
  template<class EigenType1, class Type2, class Frame2> \
  auto operator op(const EigenType1& x, const bi::Array<Type2,Frame2>& y) { \
    return x op y.toEigen(); \
  } \
  \
  template<class Type1, class Frame1, class EigenType2> \
  auto operator op(const bi::Array<Type1,Frame1>& x, const EigenType2& y) { \
    return x.toEigen() op y; \
  }

BINARY_OPERATOR(+)
BINARY_OPERATOR(-)
BINARY_OPERATOR(*)
BINARY_OPERATOR(/)
