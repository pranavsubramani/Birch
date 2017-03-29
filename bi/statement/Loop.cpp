/**
 * @file
 */
#include "bi/statement/Loop.hpp"

#include "bi/visitor/all.hpp"

#include <typeinfo>

bi::Loop::Loop(Expression* cond, Expression* braces, shared_ptr<Location> loc) :
    Statement(loc),
    Conditioned(cond),
    Braced(braces) {
  //
}

bi::Loop::~Loop() {
  //
}

bi::Statement* bi::Loop::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Statement* bi::Loop::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::Loop::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::Loop::dispatchDefinitely(const Statement& o) const {
  return o.definitely(*this);
}

bool bi::Loop::definitely(const Loop& o) const {
  return cond->definitely(*o.cond) && braces->definitely(*o.braces);
}

bool bi::Loop::dispatchPossibly(const Statement& o) const {
  return o.possibly(*this);
}

bool bi::Loop::possibly(const Loop& o) const {
  return cond->possibly(*o.cond) && braces->possibly(*o.braces);
}
