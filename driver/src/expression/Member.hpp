/**
 * @file
 */
#pragma once

#include "src/expression/Expression.hpp"
#include "src/common/Couple.hpp"

namespace birch {
/**
 * Membership operator expression.
 *
 * @ingroup expression
 */
class Member: public Expression, public Couple<Expression> {
public:
  /**
   * Constructor.
   *
   * @param left Left operand.
   * @param right Right operand.
   * @param loc Location.
   */
  Member(Expression* left, Expression* right, Location* loc =
      nullptr);

  /**
   * Destructor.
   */
  virtual ~Member();

  virtual bool isAssignable() const;
  virtual bool isMembership() const;

  virtual Expression* accept(Cloner* visitor) const;
  virtual Expression* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;
};
}
