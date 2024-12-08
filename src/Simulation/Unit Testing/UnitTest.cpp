#include <iostream>
#include <cmath>
#include "vector.h"

void test_vector_constructor_and_magnitute() {
    Vector v(3, 4, 0);
    assert_equal(5.0, v.magnitude(), "Magnitude of (3, 4, 0)");
}

int main() {
    test_vector_constructor_and_magnitute();

    std::cout << "\nSummary: " << passed_tests << "/" << total_tests << " tests passed.\n";
    return (total_tests == passed_tests) ? 0 : 1;
}