/**
 * @file
 */
#if ENABLE_LAZY_DEEP_CLONE
#include "libbirch/LazyMemo.hpp"

#include "libbirch/SwapClone.hpp"
#include "libbirch/SwapContext.hpp"

libbirch::LazyMemo::LazyMemo() {
  //
}

libbirch::LazyMemo::LazyMemo(LazyMemo* parent) {
  assert(parent);
  m.copy(parent->m);
}

libbirch::LazyMemo::~LazyMemo() {
  //
}

libbirch::LazyAny* libbirch::LazyMemo::get(LazyAny* o) {
  if (this == o->getContext()) {
    return o;
  } else {
    LazyAny* prev = nullptr;
    LazyAny* next = o;
    do {
      prev = next;
      next = m.get(prev, prev);
    } while (next != prev);
    if (this != next->getContext()) {
      next = copy(next);
    }
    return next;
  }
}

libbirch::LazyAny* libbirch::LazyMemo::pull(LazyAny* o) {
  if (this == o->getContext()) {
    return o;
  } else {
    LazyAny* prev = nullptr;
    LazyAny* next = o;
    do {
      prev = next;
      next = m.get(prev, prev);
    } while (next != prev);
    return next;
  }
}

libbirch::LazyAny* libbirch::LazyMemo::copy(LazyAny* o) {
  /* for a lazy deep clone there is no risk of infinite recursion, but
   * there may be thread contention if two threads access the same object
   * and both trigger a lazy clone simultaneously; in this case multiple
   * new objects may be made but only one thread can be successful in
   * inserting an object into the map; a shared pointer is used to
   * destroy any additional objects */
  SwapClone swapClone(true);
  SwapContext swapContext(this);
  assert(o->isFrozen());
  SharedPtr<LazyAny> cloned = o->clone_();
  // ^ use shared to clean up if beaten by another thread
  auto result = m.put(o, cloned.get());
  if (this->isFrozen()) {
    result->freeze();
  }
  return result;
}

#endif
