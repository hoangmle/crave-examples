#include <crave/ConstrainedRandom.hpp>
#include <iomanip>

using crave::rand_obj;
using crave::randv;
using crave::reference;
using crave::distribution;
using crave::dist;
using crave::bitslice;

class item : public rand_obj {
 public:
  item(rand_obj* parent = 0)
      : rand_obj(parent), src_addr(this), dest_addr(this), data(this), x(this), y(this), z(this) {
    constraint(dist(src_addr(), distribution<uint>::simple_range(0, 0xFE)));
    constraint(dest_addr() <= src_addr());
    constraint("neg_data", -16 < data() && data() < 0);
    constraint("pos_data", 16 > data() && data() > 0);
    constraint(bitslice(10, 3, x()) == 0xFF);
    constraint(x() > 123456);
    constraint((y() ^ 0x7FFF) == 0);
    constraint((z() >> 4) == 0x000F);
    constraint.printDotGraph(std::cout);
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << it.src_addr << " " << it.dest_addr << " " << it.data << " " << it.x << " " << it.y << " " << it.z;
    return os;
  }

  randv<uint> src_addr;
  randv<uint> dest_addr;
  randv<short> data;
  randv<int> x;
  randv<short> y, z;
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  item it;
  assert(!it.next());  // unsatisfiable constraints (data < 0) and (data > 0)

  std::cout << std::hex << std::showbase << std::internal;

  it.disable_constraint("neg_data");
  for (int i = 0; i < 10; i++) {
    it.next();
    std::cout << it << std::endl;
  }

  std::cout << "***********************" << std::endl;

  it.enable_constraint("neg_data");
  it.disable_constraint("pos_data");
  for (int i = 0; i < 10; i++) {
    it.next();
    std::cout << it << std::endl;
  }

  return 0;
}
