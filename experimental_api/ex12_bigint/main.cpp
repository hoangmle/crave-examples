#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>
#include <crave/experimental/SystemC.hpp>

#include <iostream>

using namespace std;
using namespace crave;

class item : public crv_sequence_item {
 public:
  item(crv_object_name) {
    c_src_addr_range = { dist(src_addr(), make_distribution(range<uint512_t>(0, 15), range<uint512_t>(64, 79))) };
    c_dest_addr_range = { dist(dest_addr(),
                               make_distribution(weighted_range<uint128_t>(0, 9, 60), weighted_range<uint128_t>(10, 19, 30),
                                                 weighted_range<uint128_t>(std::numeric_limits<uint128_t>::max() - 9, std::numeric_limits<uint128_t>::max(), 10))) };
    c_payload = { (payload() & uint128_t("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000")) == 0 };
    c_bv = { bitslice(71, 8, bv()) == 0,
             bitslice(99, 92, bv()) == 0xFF };
  }

  friend ostream& operator<<(ostream& os, item& it) {
    os << it.src_addr << " " << it.dest_addr << " " << std::hex << it.payload << " " << it.bv << std::dec;
    return os;
  }

  crv_constraint c_src_addr_range{ "src_addr_range" };
  crv_constraint c_dest_addr_range{ "dest_addr_range" };
  crv_constraint c_payload{ "payload" };
  crv_constraint c_bv{ "bv" };

  crv_variable<uint512_t> src_addr;
  crv_variable<uint128_t> dest_addr;
  crv_variable<unsigned_int<160>> payload;
  crv_variable<sc_dt::sc_bv<100>> bv;
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  crave::set_global_seed(123456789);  // set the global seed for consistent results

  item it("single_packet");

  for (int i = 0; i < 20; i++) {
    CHECK(it.bv.randomize());
    std::cout << it << std::endl;
  }

  for (int i = 0; i < 20; i++) {
    CHECK(it.randomize());
    std::cout << it << std::endl;
  }

  return 0;
}
