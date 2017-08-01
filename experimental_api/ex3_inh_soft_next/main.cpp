#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

#include <iostream>

using std::ostream;

using namespace crave;

struct item : public crv_sequence_item {
  crv_variable<unsigned> src_addr;
  crv_variable<unsigned> dest_addr;

  crv_constraint hard_c{ src_addr() <= 20, dest_addr() <= 100 };
  crv_soft_constraint soft_c{ src_addr() % 4 == 0 };

  item(crv_object_name) {}

  friend ostream& operator<<(ostream& os, item& it) {
    os << it.src_addr << " " << it.dest_addr;
    return os;
  }
};

struct item_ext : public item {
  crv_constraint hard_c1{ src_addr() % 4 == 3,  // this constraint makes the soft constraint in item useless.
                          dest_addr() > reference(last_dest_addr) };

  // item_ext inherits all constraints of item
  item_ext(crv_object_name name) : item(name), last_dest_addr() {}

  bool randomize() override {
    // custom randomize() saves the generated value of dest_addr
    if (crv_sequence_item::randomize()) {
      last_dest_addr = dest_addr;
      return true;
    }
    return false;
  }

  unsigned last_dest_addr;
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  item it("it");
  for (int i = 0; i < 10; i++) {
    CHECK(it.randomize());
    std::cout << it << std::endl;
  }
  std::cout << "***********************" << std::endl;
  item_ext it_ext("it_ext");
  while (it_ext.randomize()) {
    std::cout << it_ext << std::endl;
  }
  return 0;
}
