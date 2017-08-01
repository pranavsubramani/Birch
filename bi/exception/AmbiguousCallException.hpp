/**
 * @file
 */
#pragma once

#include "bi/exception/CompilerException.hpp"
#include "bi/expression/Call.hpp"

#include <list>

namespace bi {
/**
 * Ambiguous overloaded function call.
 *
 * @ingroup compiler_exception
 */
struct AmbiguousCallException: public CompilerException {
  /**
   * Constructor.
   */
  AmbiguousCallException(const Type* o, const std::list<Type*>& matches);
};
}
