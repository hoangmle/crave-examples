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

using crave::dist;
using crave::distribution;
using crave::reference;

struct ALU12 : public crv_sequence_item {
  crv_variable<sc_bv<2> > op;
  crv_variable<sc_uint<12> > a, b;

  crv_constraint c_dist{"dist"};
  crv_constraint c_add{"add"};
  crv_constraint c_sub{"sub"};
  crv_constraint c_mul{"mul"};
  crv_constraint c_div{"div"};

  ALU12(crv_object_name) {
    c_dist = {dist(op(), distribution<short>::simple_range(0, 3))};
    c_add = {(op() != 0x0) || (4095 >= a() + b())};
    c_sub = {(op() != 0x1) || ((4095 >= a() - b()) && (b() <= a()))};
    c_mul = {(op() != 0x2) || (4095 >= a() * b())};
    c_div = {(op() != 0x3) || (b() != 0)};
  }

  friend std::ostream& operator<<(std::ostream& o, ALU12 const& alu) {
    o << alu.op << ' ' << alu.a << ' ' << alu.b;
    return o;
  }
};

int sc_main(int argc, char** argv) {
  crave::init("crave.cfg");
  boost::timer timer;
  ALU12 c("ALU");
  c.randomize();
  std::cout << "first: " << timer.elapsed() << "\n";
  for (int i = 0; i < 1000; ++i) {
    c.randomize();
    std::cout << c << std::endl;
  }
  std::cout << "complete: " << timer.elapsed() << "\n";
  return 0;
}
