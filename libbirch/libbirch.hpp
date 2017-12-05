/**
 * @file
 *
 * Standard headers that should be included for all C++ files generated by
 * the compiler.
 */
#pragma once

#include "libbirch/Span.hpp"
#include "libbirch/Index.hpp"
#include "libbirch/Range.hpp"
#include "libbirch/Frame.hpp"
#include "libbirch/View.hpp"
#include "libbirch/Array.hpp"
#include "libbirch/Sequence.hpp"
#include "libbirch/Any.hpp"
#include "libbirch/Object.hpp"
#include "libbirch/SharedPointer.hpp"
#include "libbirch/Optional.hpp"
#include "libbirch/FiberState.hpp"
#include "libbirch/Fiber.hpp"
#include "libbirch/Eigen.hpp"
#include "libbirch/EigenFunctions.hpp"
#include "libbirch/EigenOperators.hpp"
#include "libbirch/global.hpp"

#include "libubjpp/libubjpp.hpp"

#include "boost/math/special_functions/beta.hpp"
///@todo Replace the above with STL version under C++17.

#include <getopt.h>

#include <random>
#include <algorithm>
#include <utility>
#include <functional>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <cassert>

namespace bi {

namespace type {
/*
 * Basic types.
 */
using Boolean_ = bool;
using Real64_ = double;
using Real32_ = float;
using Integer64_ = int64_t;
using Integer32_ = int32_t;
using String_ = std::string;
using File_ = FILE*;
}

/**
 * Default array for `D` dimensions.
 */
template<class T, int D>
using DefaultArray = Array<T,typename DefaultFrame<D>::type>;

/**
 * Default view for `D`-dimensional indexing of a single element.
 */
template<int D>
struct DefaultView {
  typedef NonemptyView<Index<>,typename DefaultView<D - 1>::type> type;
};
template<>
struct DefaultView<0> {
  typedef EmptyView type;
};

/**
 * Make a range.
 *
 * @ingroup libbirch
 *
 * @param start First index.
 * @param end Last index.
 */
Range<> make_range(const ptrdiff_t start, const ptrdiff_t end);

/**
 * Make a frame, no arguments.
 *
 * @ingroup libbirch
 */
EmptyFrame make_frame();

/**
 * Make a frame, single argument.
 *
 * @ingroup libbirch
 */
NonemptyFrame<Span<>,EmptyFrame> make_frame(const size_t arg);

/**
 * Make a frame, multiple arguments.
 *
 * @ingroup libbirch
 */
template<class ... Args>
auto make_frame(const size_t arg, Args ... args) {
  auto tail = make_frame(args...);
  auto head = Span<>(arg, tail.volume());
  return NonemptyFrame<decltype(head),decltype(tail)>(head, tail);
}

/**
 * Make a view, no arguments.
 *
 * @ingroup libbirch
 */
EmptyView make_view();

/**
 * Make a view, single argument.
 *
 * @ingroup libbirch
 */
template<ptrdiff_t offset_value, size_t length_value>
auto make_view(const Range<offset_value,length_value>& arg) {
  auto head = arg;
  auto tail = make_view();
  return NonemptyView<decltype(head),decltype(tail)>(head, tail);
}

/**
 * Make a view, single argument.
 *
 * @ingroup libbirch
 */
NonemptyView<Index<>,EmptyView> make_view(const ptrdiff_t arg);

/**
 * Make a view, multiple arguments.
 *
 * @ingroup libbirch
 */
template<ptrdiff_t offset_value, size_t length_value, class ... Args>
auto make_view(const Range<offset_value,length_value>& arg, Args ... args) {
  auto head = arg;
  auto tail = make_view(args...);
  return NonemptyView<decltype(head),decltype(tail)>(head, tail);
}

/**
 * Make a view, multiple arguments.
 *
 * @ingroup libbirch
 */
template<class ... Args>
auto make_view(const ptrdiff_t arg, Args ... args) {
  auto head = Index<mutable_value>(arg);
  auto tail = make_view(args...);
  return NonemptyView<decltype(head),decltype(tail)>(head, tail);
}

/**
 * Make an array.
 *
 * @ingroup libbirch
 *
 * @tparam Type Value type.
 * @tparam Frame Frame type.
 * @tparam Args Constructor parameter types.
 *
 * @param frame Frame.
 * @param args Constructor arguments.
 *
 * @return The array.
 */
template<class Type, class Frame, class ... Args>
auto make_array(const Frame& frame, Args ... args) {
  return Array<Type,Frame>(frame, args...);
}

/**
 * Make a sequence.
 *
 * @ingroup libbirch
 *
 * @tparam Type Value type.
 *
 * @param values Values.
 *
 * @return The sequence.
 */
template<class Type>
auto make_sequence(const std::initializer_list<Type> values) {
  return Sequence<Type>(values);
}

/**
 * Make an object.
 *
 * @ingroup libbirch
 *
 * @tparam SharedPointerType SharedPointer type.
 * @tparam Args Constructor parameter types.
 *
 * @param args Constructor arguments.
 *
 * @return SharedPointer to the object.
 */
template<class SharedPointerType, class ... Args>
SharedPointerType make_object(Args ... args) {
  using ValueType = typename SharedPointerType::value_type;
  auto raw = new ValueType(args...);
  return SharedPointerType(raw);
}

/**
 * Shallow-copy an object.
 *
 * @ingroup libbirch
 *
 * @tparam SharedPointerType SharedPointer type.
 *
 * @param o The object to copy.
 *
 * @return SharedPointer to the new object.
 */
template<class ValueType>
ValueType* copy_object(ValueType* o) {
  return new ValueType(*o);
}

/**
 * Make a fiber.
 *
 * @tparam YieldType The yield type of the fiber.
 * @tparam StateType The state type of the fiber.
 * @tparam Args Fiber state constructor parameter types.
 *
 * @param args Fiber state constructor arguments.
 *
 * For a member fiber, the first argument should be a raw pointer to the
 * containing object.
 */
template<class YieldType, class StateType, class ... Args>
Fiber<YieldType> make_fiber(Args ... args) {
  return Fiber<YieldType>(new StateType(args...), false);
}

/**
 * Make a closed fiber.
 *
 * @tparam YieldType The yield type of the fiber.
 * @tparam StateType The state type of the fiber.
 * @tparam Args Fiber state constructor parameter types.
 *
 * @param args Fiber state constructor arguments.
 *
 * For a member fiber, the first argument should be a raw pointer to the
 * containing object.
 */
template<class YieldType, class StateType, class ... Args>
Fiber<YieldType> make_closed_fiber(Args ... args) {
  return Fiber<YieldType>(new StateType(args...), true);
}

/**
 * Cast an object.
 */
template<class To, class From>
Optional<SharedPointer<To>> dynamic_pointer_cast(
    const SharedPointer<From>& from) {
  return Optional<SharedPointer<To>>(from.template dynamic_pointer_cast<To>());
}

/**
 * Cast an object.
 */
template<class To, class From>
Optional<SharedPointer<To>> dynamic_pointer_cast(
    const Optional<SharedPointer<From>>& from) {
  if (from.query()) {
    return dynamic_pointer_cast<SharedPointer<To>>(from.get());
  } else {
    return Optional<SharedPointer<To>>();
  }
}

}
