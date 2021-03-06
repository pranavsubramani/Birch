/**
 * @file
 */
#include "src/expression/Nil.hpp"

#include "src/visitor/all.hpp"

birch::Nil::Nil(Location* loc) :
    Expression(loc) {
  //
}

birch::Nil::~Nil() {
  //
}

birch::Expression* birch::Nil::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

birch::Expression* birch::Nil::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void birch::Nil::accept(Visitor* visitor) const {
  visitor->visit(this);
}
