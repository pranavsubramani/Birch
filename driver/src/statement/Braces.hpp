/**
 * @file
 */
#pragma once

#include "src/statement/Statement.hpp"
#include "src/common/Single.hpp"
#include "src/statement/EmptyStatement.hpp"

namespace birch {
/**
 * Statement in braces.
 *
 * @ingroup statement
 */
class Braces: public Statement, public Single<Statement> {
public:
  /**
   * Constructor.
   *
   * @param single Root statement in braces.
   * @param loc Location.
   */
  Braces(Statement* single, Location* loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~Braces();

  virtual Statement* strip();

  virtual Statement* accept(Cloner* visitor) const;
  virtual Statement* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;
};
}
