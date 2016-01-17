#include <systemc.h>

#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>
#include <crave/experimental/SystemC.hpp>

using namespace crave;

struct sysc_cont : public crv_sequence_item {
  crv_variable<sc_int<5>> x { "x" };
  crv_variable<sc_uint<6>> y { "y" };
  crv_variable<sc_bv<7>> z { "z" };

  crv_constraint constr { "constr", {
    x() == 15,
    y() > 0 && y() % 16 == 0,
    (z() & 0xF) == 0xE
  } };

  sysc_cont(crv_object_name) { } 
};

int main (int argc , char *argv[]) {
  crave::init("crave.cfg");
  sysc_cont sc("sc");
  for (int i = 0; i < 40; i++) {
    assert(sc.randomize());
    std::cout << sc.x << " " << sc.y << " " << sc.z << std::endl;
  }
}

