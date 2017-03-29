/**
 * @file
 */
#include "bi/statement/Conditional.hpp"

#include "bi/visitor/all.hpp"

#include <typeinfo>

bi::Conditional::Conditional(Expression* cond, Expression* braces,
    Expression* falseBraces, shared_ptr<Location> loc) :
    Statement(loc),
    Conditioned(cond),
    Braced(braces),
    falseBraces(falseBraces) {
  /* pre-condition */
  assert(falseBraces);
}

bi::Conditional::~Conditional() {
  //
}

bi::Statement* bi::Conditional::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Statement* bi::Conditional::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::Conditional::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::Conditional::dispatchDefinitely(const Statement& o) const {
  return o.definitely(*this);
}

bool bi::Conditional::definitely(const Conditional& o) const {
  return cond->definitely(*o.cond) && braces->definitely(*o.braces)
      && falseBraces->definitely(*o.falseBraces);
}

bool bi::Conditional::dispatchPossibly(const Statement& o) const {
  return o.possibly(*this);
}

bool bi::Conditional::possibly(const Conditional& o) const {
  return cond->possibly(*o.cond) && braces->possibly(*o.braces)
      && falseBraces->possibly(*o.falseBraces);
}
