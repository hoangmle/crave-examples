#include <crave/ConstrainedRandom.hpp>

#include <iostream>

using std::ostream;
using crave::rand_obj;
using crave::randv;
using crave::distribution;
using crave::dist;
using crave::range;
using crave::weighted_range;

class item : public rand_obj {
 public:
  item(rand_obj* parent = 0) : rand_obj(parent), src_addr(this), dest_addr(this) {
    constraint(dist(src_addr(), distribution<unsigned>::create(range<unsigned>(0, 9))(range<unsigned>(90, 99))));
    constraint(dist(dest_addr(), distribution<unsigned>::create(weighted_range<unsigned>(0, 9, 60))(
                                     weighted_range<unsigned>(10, 19, 30))(weighted_range<unsigned>(100, 109, 10))));
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << it.src_addr << " " << it.dest_addr;
    return os;
  }

  randv<unsigned> src_addr;
  randv<unsigned> dest_addr;
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  crave::set_global_seed(1234567890);  // set the global seed for consistent results

  item it;

  for (int i = 0; i < 20; i++) {
    CHECK(it.next());
    std::cout << it << std::endl;
  }

  return 0;
}
