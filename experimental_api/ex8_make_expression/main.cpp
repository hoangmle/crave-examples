#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using crave::crv_sequence_item;
using crave::crv_constraint;
using crave::crv_array;
using crave::crv_object_name;

template <int N>
struct item : public crv_sequence_item {
  item(crv_object_name) {
    arr_constr = {arr.sum() == 20};
    for (int i = 0; i < N; i++) arr_constr &= {10 > arr[i]()};
  }

  crv_array<unsigned, N> arr{"arr"};
  crv_constraint arr_constr{"arr_constr"};
};

int main(int argc, char *argv[]) {
  crave::init("crave.cfg");

  item<5> it("5_items");
  assert(it.randomize());
  for (int i = 0; i < 5; i++) std::cout << it.arr[i] << " ";
  std::cout << std::endl;

  item<15> it1("15_items");
  assert(it1.randomize());
  for (int i = 0; i < 15; i++) std::cout << it1.arr[i] << " ";
  std::cout << std::endl;

  return 0;
}
