#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using namespace crave;

struct base_obj : public crv_sequence_item {
  crv_variable<unsigned> x { "x" };
  crv_variable<unsigned> y { "y" };

  crv_constraint c1 { "c1" };
  crv_constraint c2 { "c2" };

  base_obj(crv_object_name) {
    c1 = { x() <= 10 };
    c2 = { 10 < y() && y() <= 20 };
  } 
};

struct derived_obj : public base_obj {
  crv_variable<unsigned> z { "z" };

  derived_obj(crv_object_name name) : base_obj(name) { } 
};

int main (int argc , char *argv[]) {
  crave::init("crave.cfg");
  derived_obj obj("obj");
  for (int i = 0; i < 10; i++) {
    assert(obj.randomize());
    std::cout << obj.x << " " << obj.y << " " << obj.z << std::endl;
  }
  std::cout << std::endl;
  for (int i = 0; i < 10; i++) {
    assert(obj.randomize_with(obj.x() % 5 == 0, obj.y() % 5 == 0, obj.x() + obj.y() == obj.z()));
    std::cout << obj.x << " " << obj.y << " " << obj.z << std::endl;
  }
}
