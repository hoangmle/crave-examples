#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>
#include <crave/experimental/Array.hpp>

using crave::crv_sequence_item;
using crave::crv_constraint;
using crave::crv_variable;
using crave::crv_object_name;
using crave::crv_array;

using crave::rand_obj;
using crave::randv;
using crave::rand_vec;
using crave::placeholder;
using crave::foreach;
using crave::unique;
using crave::if_then;
using crave::if_then_else;

#define IF_THEN(a, b) !(a) || (b)
#define IF_THEN_ELSE(a, b, c) (!(a) || (b)) && ((a) || (c))

class item : public crv_sequence_item {
 public:
  item(crv_object_name) : _i() {
    c_tmp_range(5 <= tmp() && tmp() <= 10);

    c_src_conditions(src_addr_vec().size() == tmp());
    c_src_conditions(foreach (src_addr_vec(), src_addr_vec()[_i] < 0xFF));
    c_src_conditions(foreach (src_addr_vec(), src_addr_vec()[_i] % 4 == 0));
    c_src_conditions(unique(src_addr_vec()));

    c_dest_conditions(dest_addr_vec().size() == tmp());
    c_dest_conditions(foreach (dest_addr_vec(), if_then(_i == 0, dest_addr_vec()[_i] < 0xF)));
    c_dest_conditions(foreach (dest_addr_vec(), dest_addr_vec()[_i] == dest_addr_vec()[_i - 1] + 8));
    c_dest_conditions(unique(dest_addr_vec()));

    c_data_conditions(data_vec().size() == 2 * tmp());
    c_data_conditions(foreach (data_vec(), if_then_else(_i % 2 == 0, data_vec()[_i] <= 10,
                                                        data_vec()[_i] == data_vec()[_i - 1] * data_vec()[_i - 1])));
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << "src_addr_vec = ";
    for (uint i = 0; i < it.src_addr_vec.size(); i++) os << it.src_addr_vec[i] << " ";
    os << std::endl;
    os << "dest_addr_vec = ";
    for (uint i = 0; i < it.dest_addr_vec.size(); i++) os << it.dest_addr_vec[i] << " ";
    os << std::endl;
    os << "data_vec = ";
    for (uint i = 0; i < it.data_vec.size(); i++) os << it.data_vec[i] << " ";
    os << std::endl;
    return os;
  }

  // TODO: Vektorvariablen gehen noch nicht mit variabler Größe
  // crv_array<uint> src_addr_vec;
  // crv_array<uint> dest_addr_vec;
  // crv_array<uint> data_vec;
  crv_variable<unsigned> tmp;
  placeholder _i;

  crv_constraint c_tmp_range{"tmp_range"};
  crv_constraint c_src_conditions{"src_conditions"};
  crv_constraint c_dest_conditions{"dest_conditions"};
  crv_constraint c_data_conditions{"data_conditions"};
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  item it("single_packet");

  // it.constraint.printDotGraph(std::cout);

  for (int i = 0; i < 10; i++) {
    it.randomize();
    std::cout << it << std::endl;
  }

  return 0;
}
