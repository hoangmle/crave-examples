#include <crave/ConstrainedRandom.hpp>

using crave::rand_obj;
using crave::randv;
using crave::rand_vec;
using crave::_i;
using crave::foreach;
using crave::unique;
using crave::if_then;
using crave::if_then_else;

class item : public rand_obj {
 public:
  item(rand_obj* parent = 0) : rand_obj(parent), src_addr_vec(this), dest_addr_vec(this), data_vec(this), tmp(this) {
    constraint(5 <= tmp() && tmp() <= 10);

    constraint(src_addr_vec().size() == tmp());
    constraint(foreach (src_addr_vec(), src_addr_vec()[_i] < 0xFF));
    constraint(foreach (src_addr_vec(), src_addr_vec()[_i] % 4 == 0));
    constraint(unique(src_addr_vec()));

    constraint(dest_addr_vec().size() == tmp());
    constraint(foreach (dest_addr_vec(), if_then(_i == 0, dest_addr_vec()[_i] < 0xF)));
    constraint(foreach (dest_addr_vec(), dest_addr_vec()[_i] == dest_addr_vec()[_i - 1] + 8));
    constraint(unique(dest_addr_vec()));

    constraint(data_vec().size() == 2 * tmp());
    constraint(foreach (data_vec(), if_then_else(_i % 2 == 0, data_vec()[_i] <= 10,
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

  rand_vec<uint> src_addr_vec;
  rand_vec<uint> dest_addr_vec;
  rand_vec<uint> data_vec;
  randv<uint> tmp;
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  item it;

  for (int i = 0; i < 10; i++) {
    it.next();
    std::cout << it << std::endl;
  }

  return 0;
}
