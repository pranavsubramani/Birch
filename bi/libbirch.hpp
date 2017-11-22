/**
 * @file
 *
 * Standard headers that should be included for all C++ files generated by
 * the compiler.
 */
#pragma once

#include "bi/lib/Span.hpp"
#include "bi/lib/Index.hpp"
#include "bi/lib/Range.hpp"
#include "bi/lib/Frame.hpp"
#include "bi/lib/View.hpp"
#include "bi/lib/Array.hpp"
#include "bi/lib/Sequence.hpp"
#include "bi/lib/Any.hpp"
#include "bi/lib/Object.hpp"
#include "bi/lib/Optional.hpp"
#include "bi/lib/Pointer.hpp"
#include "bi/lib/FiberState.hpp"
#include "bi/lib/Fiber.hpp"
#include "bi/lib/Eigen.hpp"
#include "bi/lib/EigenFunctions.hpp"
#include "bi/lib/EigenOperators.hpp"
#include "bi/lib/global.hpp"

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
 * Make a span.
 *
 * @ingroup library
 *
 * @param length Length.
 */
inline auto make_span(const size_t length) {
  return Span<mutable_value,1,mutable_value>(length);
}

/**
 * Make an index.
 *
 * @ingroup library
 *
 * @param index Index.
 */
inline auto make_index(const ptrdiff_t i) {
  return Index<mutable_value>(i);
}

/**
 * Make a range.
 *
 * @ingroup library
 *
 * @param start First index.
 * @param end Last index.
 */
inline auto make_range(const ptrdiff_t start, const ptrdiff_t end) {
  ptrdiff_t length = std::max(ptrdiff_t(0), end - start + 1);
  return Range<mutable_value,mutable_value,1>(start, length);
}

/**
 * Make a frame, no arguments.
 *
 * @ingroup library
 */
inline auto make_frame() {
  return EmptyFrame();
}

/**
 * Make a frame, single argument.
 *
 * @ingroup library
 */
template<size_t length_value, ptrdiff_t stride_value, size_t lead_value>
auto make_frame(const Span<length_value,stride_value,lead_value>& arg) {
  auto tail = make_frame();
  auto head = arg;
  return NonemptyFrame<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a frame, single argument.
 *
 * @ingroup library
 */
inline auto make_frame(const size_t arg) {
  return make_frame(make_span(arg));
}

/**
 * Make a frame, multiple arguments.
 *
 * @ingroup library
 */
template<size_t length_value, ptrdiff_t stride_value, size_t lead_value,
    class ... Args>
auto make_frame(const Span<length_value,stride_value,lead_value>& arg,
    Args ... args) {
  return make_frame(make_frame(arg), args...);
}

/**
 * Make a frame, multiple arguments.
 *
 * @ingroup library
 */
template<class ... Args>
auto make_frame(const size_t arg, Args ... args) {
  return make_frame(make_frame(arg), args...);
}

/**
 * Make a frame, tail and single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head, size_t length_value, ptrdiff_t stride_value,
    size_t lead_value>
auto make_frame(const NonemptyFrame<Tail,Head>& frame,
    const Span<length_value,stride_value,lead_value>& arg) {
  auto tail = frame;
  auto head = arg;
  return NonemptyFrame<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a frame, tail and single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head>
auto make_frame(const NonemptyFrame<Tail,Head>& frame, const size_t arg) {
  return make_frame(frame, make_span(arg));
}

/**
 * Make a frame, tail and multiple arguments.
 *
 * @ingroup library
 */
template<class Tail, class Head, class Arg, class ... Args>
auto make_frame(const NonemptyFrame<Tail,Head>& tail, const Arg& arg,
    Args ... args) {
  return make_frame(make_frame(tail, arg), args...);
}

/**
 * Make a frame, tail and multiple arguments.
 *
 * @ingroup library
 */
template<class Tail, class Head, class ... Args>
auto make_frame(const NonemptyFrame<Tail,Head>& tail, const size_t arg,
    Args ... args) {
  return make_frame(make_frame(tail, make_span(arg)), args...);
}

/**
 * Make a view, no arguments.
 *
 * @ingroup library
 */
inline auto make_view() {
  return EmptyView();
}

/**
 * Make a view, single argument.
 *
 * @ingroup library
 */
template<ptrdiff_t offset_value, size_t length_value, ptrdiff_t stride_value>
auto make_view(const Range<offset_value,length_value,stride_value>& arg) {
  auto tail = make_view();
  auto head = arg;
  return NonemptyView<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a view, single argument.
 *
 * @ingroup library
 */
template<ptrdiff_t offset_value>
auto make_view(const Index<offset_value>& arg) {
  auto tail = make_view();
  auto head = arg;
  return NonemptyView<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a view, single argument.
 *
 * @ingroup library
 */
inline auto make_view(const ptrdiff_t arg) {
  return make_view(make_index(arg));
}

/**
 * Make a view, multiple arguments.
 *
 * @ingroup library
 */
template<ptrdiff_t offset_value, size_t length_value, ptrdiff_t stride_value,
    class ... Args>
auto make_view(const Range<offset_value,length_value,stride_value>& arg,
    Args ... args) {
  return make_view(make_view(arg), args...);
}

/**
 * Make a view, multiple arguments.
 *
 * @ingroup library
 */
template<ptrdiff_t offset_value, class ... Args>
auto make_view(const Index<offset_value>& arg, Args ... args) {
  return make_view(make_view(arg), args...);
}

/**
 * Make a view, multiple arguments.
 *
 * @ingroup library
 */
template<class ... Args>
auto make_view(const ptrdiff_t arg, Args ... args) {
  return make_view(make_index(arg), args...);
}

/**
 * Make a view, tail plus single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head, ptrdiff_t offset_value, size_t length_value,
    ptrdiff_t stride_value>
auto make_view(const NonemptyView<Tail,Head>& view,
    const Range<offset_value,length_value,stride_value>& arg) {
  auto tail = view;
  auto head = arg;
  return NonemptyView<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a view, tail plus single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head, ptrdiff_t offset_value>
auto make_view(const NonemptyView<Tail,Head>& view,
    const Index<offset_value>& arg) {
  auto tail = view;
  auto head = arg;
  return NonemptyView<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a view, tail plus single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head>
auto make_view(const NonemptyView<Tail,Head>& view, const ptrdiff_t arg) {
  return make_view(view, make_index(arg));
}

/**
 * Make a view, tail plus multiple arguments.
 *
 * @ingroup library
 */
template<class Tail, class Head, class Arg, class ... Args>
auto make_view(const NonemptyView<Tail,Head>& tail, Arg arg, Args ... args) {
  return make_view(make_view(tail, arg), args...);
}

/**
 * Make an array.
 *
 * @ingroup library
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
 * @ingroup library
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
 * @ingroup library
 *
 * @tparam PointerType Pointer type.
 * @tparam Args Constructor parameter types.
 *
 * @param args Constructor arguments.
 *
 * @return Pointer to the object.
 */
template<class PointerType, class ... Args>
PointerType make_object(Args ... args) {
  using ValueType = typename PointerType::value_type;
  auto raw = new (GC) ValueType(args...);
  return PointerType(raw);
}

/**
 * Shallow-copy an object.
 *
 * @ingroup library
 *
 * @tparam PointerType Pointer type.
 *
 * @param o The object to copy.
 *
 * @return Pointer to the new object.
 */
template<class ValueType>
ValueType* copy_object(ValueType* o) {
  return new (GC) ValueType(*o);
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
  Fiber<YieldType> fiber(false);
  FiberState<YieldType>* state = new (GC) StateType(args...);
  fiber.state = state;
  return fiber;
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
  Fiber<YieldType> fiber(true);
  fiber.swap();
  FiberState<YieldType>* state = new (GC) StateType(args...);
  fiber.state = state;
  fiber.swap();
  return fiber;
}

}
