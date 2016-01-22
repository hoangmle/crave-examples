#include <systemc.h>

#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>
#include <crave/experimental/SystemC.hpp>

using namespace crave;

struct sysc_cont : public crv_sequence_item {
  crv_variable<sc_int<5>> x{ "x" };
  crv_variable<sc_uint<6>> y{ "y" };
  crv_variable<sc_bv<7>> z{ "z" };

  sc_uint<5> t = 13;

  crv_constraint constr{ "constr" };

  sysc_cont(crv_object_name) {
    constr = { dist(x(), make_distribution(range<int>(5, 8))), y() > 0, y() % reference(t) == 0, y() != y(prev),
               (z() & 0xF) == 0xE };
  }
};

int main(int argc, char *argv[]) {
  crave::init("crave.cfg");
  sysc_cont sc("sc");
  for (int i = 0; i < 40; i++) {
    assert(sc.randomize());
    std::cout << sc.x << " " << sc.y << " " << sc.z << std::endl;
    if (i == 20) {
      std::cout << std::endl;
      sc.t = 17;
    }
  }
  return 0;
}
