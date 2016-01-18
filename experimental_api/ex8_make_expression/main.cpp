#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using crave::crv_sequence_item;
using crave::crv_constraint;
using crave::crv_variable;
using crave::crv_object_name;

using crave::rand_obj;
using crave::randv;
using crave::reference;

template<int N>
struct item : public crv_sequence_item {

  crave::expression sum() {
    crave::expression result = crave::value_to_expression(0);
    for (int i = 0; i < N; i++)
      result = crave::make_expression(result + arr[i]());
    return result;  
  }

	item(crv_object_name) {
    for (int i = 0; i < N; i++)
      c_arr_el_lte_10(10 > arr[i]());
      c_sum_eq_20(sum() == 20);
	}   

	crv_variable<unsigned> arr[N];
	crv_constraint c_arr_el_lte_10 { "arr_el_lte_10" };
	crv_constraint c_sum_eq_20 { "sum_eq_20" };
};

int main (int argc , char *argv[]) {
  crave::init("crave.cfg");

	item<5> it("5_items");
  assert(it.randomize());
  for (int i = 0; i < 5; i++)
    std::cout << it.arr[i] << " ";
  std::cout << std::endl;  

	item<15> it1("15_items");
  assert(it1.randomize());
  for (int i = 0; i < 15; i++)
    std::cout << it1.arr[i] << " ";
  std::cout << std::endl;  

	return 0;
}
