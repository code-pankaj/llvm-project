// RUN: %clang_cc1 -std=c++20 -fsyntax-only -verify %s

// Issue #170015: Don't crash when comparison category constants have incorrect types
// This test verifies that clang doesn't crash when std::partial_ordering has
// constants defined as primitive types instead of the comparison category type.

namespace std {
  struct partial_ordering {
    signed char value;

    constexpr partial_ordering(signed char v) : value(v) {}

    constexpr bool operator==(partial_ordering other) const { return value == other.value; }
    constexpr bool operator!=(partial_ordering other) const { return value != other.value; }
    constexpr bool operator<(partial_ordering other) const { return value < other.value; }
    constexpr bool operator>(partial_ordering other) const { return value > other.value; }
    constexpr bool operator<=(partial_ordering other) const { return value <= other.value; }
    constexpr bool operator>=(partial_ordering other) const { return value >= other.value; }

    // The constants should be of type partial_ordering, not signed char.
    // This is invalid but should not crash the compiler.
    static constexpr signed char less = -1;
    static constexpr signed char equivalent = 0;
    static constexpr signed char greater = 1;
    static constexpr signed char unordered = -128;
  };
}

void test() {
  (void) (3.0f <=> 3.5f); // expected-error {{invalid operands to binary expression ('float' and 'float')}}
}
