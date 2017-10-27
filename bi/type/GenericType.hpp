/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Reference.hpp"
#include "bi/expression/Generic.hpp"

namespace bi {
/**
 * Generic type.
 *
 * @ingroup compiler_type
 */
class GenericType: public Type,
    public Named,
    public Reference<Generic> {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param loc Location.
   * @param assignable Is this type assignable?
   * @param target Target.
   */
  GenericType(Name* name, Location* loc = nullptr,
      const bool assignable = false, Generic* target = nullptr);

  /**
   * Constructor.
   *
   * @param target Target.
   */
  GenericType(Generic* target);

  /**
   * Destructor.
   */
  virtual ~GenericType();

  virtual bool isBasic() const;
  virtual bool isClass() const;
  virtual bool isArray() const;
  virtual bool isFunction() const;
  virtual bool isFiber() const;

  virtual Basic* getBasic() const;
  virtual Class* getClass() const;

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual FunctionType* resolve(Argumented* o);
  virtual void resolveConstructor(Argumented* o);

  using Type::definitely;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const AliasType& o) const;
  virtual bool definitely(const ArrayType& o) const;
  virtual bool definitely(const BasicType& o) const;
  virtual bool definitely(const ClassType& o) const;
  virtual bool definitely(const EmptyType& o) const;
  virtual bool definitely(const FiberType& o) const;
  virtual bool definitely(const FunctionType& o) const;
  virtual bool definitely(const GenericType& o) const;
  virtual bool definitely(const OptionalType& o) const;
  virtual bool definitely(const TupleType& o) const;
  virtual bool definitely(const TypeList& o) const;
};
}