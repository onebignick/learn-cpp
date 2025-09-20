#include <iostream>

// imagine that you have a function that can take in a variadic amount of arguments
// without parameter packs you would individually define the generic for each permutation

// void print_values() {
//   std::cout << std::endl;
// }
//
// template<typename T1>
// void print_values1(T1 t1) {
//   std::cout << t1 << std::endl;
// }
//
// template<typename T1, typename T2>
// void print_values2(T1 t1, T2 t2) {
//   std::cout << t1 << " " << t2 << std::endl;
// }

// now with parameter packs this becomes simpler
// in C++11 we will use recursive variadic templates

void print_values() {
  std::cout << std::endl;
}

template<typename L, typename... R>
void print_values(L left, R... right) {
  std::cout << left;

  // std::cout << sizeof...(R) << std::endl;
  if (sizeof...(R) > 0) { // this counts the remaining parameters in the parameter pack
    std::cout << ", ";
  }

  print_values(right...);
}

int main() {
  print_values();
  print_values(1, 2, 3); // size 2, 1, 0
  print_values(1, "string", 3);
}
