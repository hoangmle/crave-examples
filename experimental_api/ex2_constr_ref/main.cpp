#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using crave::crv_sequence_item;
using crave::crv_constraint;
using crave::crv_variable;
using crave::crv_object_name;

using crave::rand_obj;
using crave::randv;
using crave::distribution;
using crave::dist;
using crave::range;
using crave::reference;
using crave::weighted_range;

class item : public crv_sequence_item {
 public:
  item(crv_object_name) : src_addr(0) {
    c_dest_aligned(dest_addr() % 4 == 0);
    c_dest_bigger_src(dest_addr() >= reference(src_addr) + 3);
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << it.src_addr << " " << it.dest_addr;
    return os;
  }

  uint src_addr;
  crv_variable<unsigned> dest_addr;

  crv_constraint c_dest_aligned{"dest_aligned"};
  crv_constraint c_dest_bigger_src{"dest_bigger_src"};
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  item it("single_packet");
  for (uint i = 0; i < 20; i++) {
    it.src_addr = i;
    it.randomize();
    std::cout << it << std::endl;
  }
  return 0;
}
