#include <crave/experimental/SystemC.hpp>
#include <crave/ConstrainedRandom.hpp>
#include <systemc.h>
#include <boost/timer.hpp>
#include <crave/experimental/Experimental.hpp>

using crave::crv_sequence_item;
using crave::crv_constraint;
using crave::crv_variable;
using crave::crv_object_name;

using sc_dt::sc_bv;
using sc_dt::sc_uint;

struct ALU32 : public crv_sequence_item {
  crv_variable<sc_bv<2> > op;
  crv_variable<sc_uint<32> > a, b;

  crv_constraint c_add{ "add" };
  crv_constraint c_sub{ "sub" };
  crv_constraint c_mul{ "mul" };
  crv_constraint c_div{ "div" };

  ALU32(crv_object_name) {
    c_add = {(op() != 0x0) || (4294967295u >= a() + b()) };
    c_sub = {(op() != 0x1) || ((4294967295u >= a() - b()) && (b() <= a())) };
    c_mul = {(op() != 0x2) || (4294967295u >= a() * b()) };
    c_div = {(op() != 0x3) || (b() != 0) };
  }

  friend std::ostream& operator<<(std::ostream& o, ALU32 const& alu) {
    o << alu.op << ' ' << alu.a << ' ' << alu.b;
    return o;
  }
};

int sc_main(int argc, char** argv) {
  crave::init("crave.cfg");
  boost::timer timer;
  ALU32 c("ALU");
  CHECK(c.randomize());
  std::cout << "first: " << timer.elapsed() << "\n";
  for (int i = 0; i < 1000; ++i) {
    CHECK(c.randomize());
  }
  std::cout << "complete: " << timer.elapsed() << "\n";
  return 0;
}
