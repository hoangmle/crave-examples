#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

#include <iostream>

using std::ostream;

using crave::crv_sequence_item;
using crave::crv_constraint;
using crave::crv_variable;
using crave::crv_object_name;
using crave::crv_vector;

using crave::rand_obj;
using crave::randv;
using crave::rand_vec;
using crave::_i;
using crave::foreach;
using crave::unique;
using crave::if_then;
using crave::if_then_else;

class item : public crv_sequence_item {
 public:
  item(crv_object_name) {
    c_tmp_range = { 5 <= tmp(), tmp() <= 10 };

    c_src_conditions = { src_addr_vec().size() == tmp(), foreach (src_addr_vec(), src_addr_vec()[_i] < 0xFF),
                         foreach (src_addr_vec(), src_addr_vec()[_i] % 4 == 0), unique(src_addr_vec()) };

    c_dest_conditions = { dest_addr_vec().size() == tmp(),
                          foreach (dest_addr_vec(), if_then(_i == 0, dest_addr_vec()[_i] < 0xF)),
                          foreach (dest_addr_vec(), dest_addr_vec()[_i] == dest_addr_vec()[_i - 1] + 8),
                          unique(dest_addr_vec()) };

    c_data_conditions = { data_vec().size() == 2 * tmp(),
                          foreach (data_vec(),
                                   if_then_else(_i % 2 == 0, data_vec()[_i] <= 10,
                                                data_vec()[_i] == data_vec()[_i - 1] * data_vec()[_i - 1])) };
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << "src_addr_vec = ";
    for (unsigned i = 0; i < it.src_addr_vec.size(); i++) os << it.src_addr_vec[i] << " ";
    os << std::endl;
    os << "dest_addr_vec = ";
    for (unsigned i = 0; i < it.dest_addr_vec.size(); i++) os << it.dest_addr_vec[i] << " ";
    os << std::endl;
    os << it.data_vec.name() << " = ";
    for (unsigned i = 0; i < it.data_vec.size(); i++) os << it.data_vec[i] << " ";
    os << std::endl;
    return os;
  }

  crv_vector<unsigned> src_addr_vec;
  crv_vector<unsigned> dest_addr_vec;
  crv_vector<unsigned> data_vec{ "data_vec" };
  crv_variable<unsigned> tmp;

  crv_constraint c_tmp_range{ "tmp_range" };
  crv_constraint c_src_conditions{ "src_conditions" };
  crv_constraint c_dest_conditions{ "dest_conditions" };
  crv_constraint c_data_conditions{ "data_conditions" };
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  item it("single_packet");

  for (int i = 0; i < 10; i++) {
    CHECK(it.randomize());
    std::cout << it << std::endl;
  }

  return 0;
}
