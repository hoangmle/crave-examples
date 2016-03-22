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

/**
 * ALU:
 * complete enumerated there are:
 * ADD 0x0: 136
 * SUB 0x1: 136
 * MUL 0x2:  76
 * DIV 0x3: 240
 * valid assignments.
 */
struct ALU4 : public crv_sequence_item {
  crv_variable<sc_bv<2> > op;
  crv_variable<sc_uint<4> > a, b;

  crv_constraint c_add{ "add" };
  crv_constraint c_sub{ "sub" };
  crv_constraint c_mul{ "mul" };
  crv_constraint c_div{ "div" };

  ALU4(crv_object_name) {
    c_add = {(op() != (unsigned char)0x0) || ((unsigned char)15 >= a() + b()) };
    c_sub = {(op() != (unsigned char)0x1) || (((unsigned char)15 >= a() - b()) && (b() <= a())) };
    c_mul = {(op() != (unsigned char)0x2) || ((unsigned char)15 >= a() * b()) };
    c_div = {(op() != (unsigned char)0x3) || (b() != (unsigned char)0) };
  }

  friend std::ostream& operator<<(std::ostream& o, ALU4 const& alu) {
    o << alu.op << ' ' << alu.a << ' ' << alu.b;
    return o;
  }
};

int sc_main(int argc, char** argv) {
  crave::init("./crave.cfg");
  boost::timer timer;
  ALU4 c("ALU");
  assert(c.randomize());
  std::cout << "first: " << timer.elapsed() << "\n";
  for (int i = 0; i < 1000; ++i) {
    assert(c.randomize());
    std::cout << i << ": " << c << std::endl;
  }
  std::cout << "complete: " << timer.elapsed() << "\n";
  return 0;
}
