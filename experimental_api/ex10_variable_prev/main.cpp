#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using namespace crave;

struct increment : public crv_sequence_item {
  crv_variable<unsigned> x { "x" };
  crv_constraint inc { "inc" };
  increment(crv_object_name) {
    inc = { x() == 2 * x.prev() + 1 };
  } 
};

int main (int argc , char *argv[]) {
  crave::init("crave.cfg");
  increment obj("obj");
  for (int i = 0; i < 40; i++) {
    assert(obj.randomize());
    std::cout << obj.x << std::endl;
  }
}
