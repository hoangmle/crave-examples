#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>
#include <iomanip>

using crave::crv_sequence_item;
using crave::crv_constraint;
using crave::crv_variable;
using crave::crv_object_name;

using crave::reference;
using crave::distribution;
using crave::dist;
using crave::bitslice;

class item : public crv_sequence_item {
 public:
  item(crv_object_name) {
    c_src_range(dist(src_addr(), distribution<uint>::simple_range(0, 0xFE)));

    c_dest_leq_src(dest_addr() <= src_addr());
    c_neg_data(-16 < data() && data() < 0);
    c_pos_data(16 > data() && data() > 0);
    c_x_constraints(bitslice(10, 3, x()) == 0xFF);
    c_x_constraints(x() > 123456);
    c_y_constraints((y() ^ 0x7FFF) == 0);
    c_z_constraints((z() >> 4) == 0x000F);
    // TODO: Ist das die Entsprechung?
    // constraint.printDotGraph(std::cout);
    this->print_object_hierarchy(1);
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << it.src_addr << " " << it.dest_addr << " " << it.data << " " << it.x << " " << it.y << " " << it.z;
    return os;
  }

  crv_variable<unsigned> src_addr;
  crv_variable<unsigned> dest_addr;
  crv_variable<short> data;
  crv_variable<int> x;
  crv_variable<short> y, z;

  crv_constraint c_src_range{"src_range"};
  crv_constraint c_dest_leq_src{"dest_leq_src"};
  crv_constraint c_neg_data{"neg_data"};
  crv_constraint c_pos_data{"pos_data"};
  crv_constraint c_x_constraints{"x_constraints"};
  crv_constraint c_y_constraints{"y_constraints"};
  crv_constraint c_z_constraints{"z_constraints"};
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  item it("single_packet");
  assert(!it.randomize());  // unsatisfiable constraints (data < 0) and (data > 0)

  std::cout << std::hex << std::showbase << std::internal;

  // TODO: Ich fände den alten Weg eleganter, über den Namen und nicht über den constraint zu gehen.
  //      Wozu vergebe ich denn sonst Namen?
  it.c_neg_data.deactivate();
  // it.disable_constraint("neg_data");
  for (int i = 0; i < 10; i++) {
    it.randomize();
    std::cout << it << std::endl;
  }

  std::cout << "***********************" << std::endl;

  it.c_neg_data.activate();
  it.c_pos_data.deactivate();
  ;
  for (int i = 0; i < 10; i++) {
    it.randomize();
    std::cout << it << std::endl;
  }

  return 0;
}
