#include <crave/ConstrainedRandom.hpp>

using crave::rand_obj;
using crave::randv;
using crave::reference;

template<int N>
struct item : public rand_obj {

  crave::expression sum() {
    crave::expression result = crave::make_expression(arr[0]());
    for (int i = 1; i < N; i++)
      result = crave::make_expression(result + arr[i]());
    return result;  
  }

	item(rand_obj* parent = 0) : rand_obj(parent) {
    for (int i = 0; i < N; i++)
      constraint(10 > arr[i]());
	  constraint(sum() == 20);
	}   

	randv<uint> arr[N];
};

int main (int argc , char *argv[]) {
  crave::init("crave.cfg");

	item<5> it;
  assert(it.next());
  for (int i = 0; i < 5; i++)
    std::cout << it.arr[i] << " ";
  std::cout << std::endl;  

	item<15> it1;
  assert(it1.next());
  for (int i = 0; i < 15; i++)
    std::cout << it1.arr[i] << " ";
  std::cout << std::endl;  

	return 0;
}
