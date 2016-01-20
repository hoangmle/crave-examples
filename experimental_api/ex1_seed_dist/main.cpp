#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using crave::crv_sequence_item;
using crave::crv_constraint;
using crave::crv_variable;
using crave::crv_object_name;
using crave::make_distribution;

using crave::dist;
using crave::range;
using crave::weighted_range;

class item : public crv_sequence_item {
 public:
  item(crv_object_name) {
    c_src_addr_range = {dist(src_addr(), make_distribution(range<uint>(0, 9), range<uint>(90, 99)))};
    c_dest_addr_range = {
        dist(dest_addr(), make_distribution(weighted_range<uint>(0, 9, 60), weighted_range<uint>(10, 19, 30),
                                            weighted_range<uint>(100, 109, 10)))};
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << it.src_addr << " " << it.dest_addr;
    return os;
  }

  crv_constraint c_src_addr_range{"src_addr_range"};
  crv_constraint c_dest_addr_range{"dest_addr_range"};
  crv_variable<unsigned> src_addr;
  crv_variable<unsigned> dest_addr;
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  crave::set_global_seed(1234567890);  // set the global seed for consistent results

  item it("single_packet");

  for (int i = 0; i < 20; i++) {
    it.randomize();
    std::cout << it << std::endl;
  }

  return 0;
}
