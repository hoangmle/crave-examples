#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using namespace crave;

struct item : public crv_sequence_item {
  crv_variable<unsigned> x { "x" };
  crv_variable<unsigned> y { "y" };

  crv_constraint constr { "constr" };

  item(crv_object_name) {
    constr = { 
      x() == 2 * x.prev() + 1,
      y() <= 100,
      y() > y.prev(),
      y() <= y.prev() + 2 
    };
  } 
};

int main (int argc , char *argv[]) {
  crave::init("crave.cfg");
  item obj("obj");
  for (int i = 0; i < 40; i++) {
    assert(obj.randomize());
    // x should reach a fixpoint, y should be increasing
    std::cout << obj.x << " " << obj.y << std::endl;
  }
}
