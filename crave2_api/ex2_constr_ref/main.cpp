#include <crave/ConstrainedRandom.hpp>

#include <iostream>

using std::ostream;

using crave::rand_obj;
using crave::randv;
using crave::distribution;
using crave::dist;
using crave::range;
using crave::reference;
using crave::weighted_range;

class item : public rand_obj {
 public:
  item(rand_obj* parent = 0) : rand_obj(parent), dest_addr(this) {
    constraint(dest_addr() % 4 == 0);
    constraint(dest_addr() >= reference(src_addr) + 3);
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << it.src_addr << " " << it.dest_addr;
    return os;
  }

  unsigned src_addr;
  randv<unsigned> dest_addr;
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  item it;
  for (unsigned i = 0; i < 20; i++) {
    it.src_addr = i;
    CHECK(it.next());
    std::cout << it << std::endl;
  }
  return 0;
}
