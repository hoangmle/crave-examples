#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>
#include <boost/format.hpp>
#include <iostream>

using namespace crave;

CRAVE_BETTER_ENUM(kkind_e,SINGLE, INCR, WRAP4, INCR4, WRAP8, INCR8, WRAP16, INCR16);
CRAVE_BETTER_ENUM(ssize_e,BYTE, HALFWORD, WORD);

class ahb_burst : public crv_sequence_item {
 public:
  crv_variable<kkind_e> kkind;
  crv_variable<ssize_e> ssize;
  crv_variable<unsigned int> incr_length;
  crv_variable<unsigned int> num_transfers;
  crv_variable<unsigned int> nnum_bytes;
  crv_variable<unsigned int> address;
  crv_constraint con;
  crv_constraint word_aligned={crave::if_then(ssize() == ssize_e::WORD, address() % 4 == 0)};
  crv_constraint halfword_aligned={crave::if_then(ssize() == ssize_e::HALFWORD, address() % 2 == 0)};
  crv_constraint legal_incr_ssize={incr_length() > 0};
  crv_constraint single_xfer={crave::if_then(kkind() == kkind_e::SINGLE, num_transfers() == 1)};
  crv_constraint inc_xfer={crave::if_then(kkind() == kkind_e::INCR, num_transfers() == incr_length())};
  crv_constraint B4={crave::if_then(kkind() == kkind_e::WRAP4 || kkind() == kkind_e::INCR4, num_transfers() == 4)};
  crv_constraint B8={crave::if_then(kkind() == kkind_e::WRAP8 || kkind() == kkind_e::INCR8, num_transfers() == 8)};
  crv_constraint B16={crave::if_then(kkind() == kkind_e::WRAP16 || kkind() == kkind_e::INCR16, num_transfers() == 16)};

  ahb_burst(crave::crv_object_name){
    for (int i = 0; i < 3; i++) {
      con&={(if_then(i == ssize(), nnum_bytes() == (num_transfers() << i)))};
    }
  }

  void print() {
    std::cout << boost::format("address=0x%x, ") % address;
    std::cout << boost::format("kkind=%u[%x], ") % kkind % kkind;
    std::cout << boost::format("ssize=%u[%x], ") % ssize % ssize;
    std::cout << boost::format("num_transfers=%u[%x], ") % num_transfers % num_transfers;
    std::cout << boost::format("nnum_bytes=%u[%x], ") % nnum_bytes % nnum_bytes;
    std::cout << std::endl;
  }
};

class mem_burst_16 : public crv_sequence_item {
 public:
  ahb_burst bursts[16]={ahb_burst("0"),
                        ahb_burst("1"),
                        ahb_burst("2"),
                        ahb_burst("3"),
                        ahb_burst("4"),
                        ahb_burst("5"),
                        ahb_burst("6"),
                        ahb_burst("7"),
                        ahb_burst("8"),
                        ahb_burst("9"),
                        ahb_burst("10"),
                        ahb_burst("11"),
                        ahb_burst("12"),
                        ahb_burst("13"),
                        ahb_burst("14"),
                        ahb_burst("15")
                        };
  crv_variable<unsigned int> legal_size;
  crv_variable<unsigned int> bursts_nnum_bytes_acc[16];
  crv_constraint legal_size_con={dist(legal_size(), distribution<uint>::simple_range(1, 16))};
  crv_constraint con;
  mem_burst_16(crave::crv_object_name) {
    //for (int i = 0; i < 16; i++) 
    //this->add_obj_child(&bursts[i]);

    con&={(bursts[0].address() == 0)};
    for (int i = 1; i < 16; i++) {
      con&={(bursts[i].address() == bursts[i - 1].address() + bursts[i - 1].nnum_bytes())};
    }

    for (int i = 0; i < 16; i++) {
      con&={(if_then(bursts[i].kkind() == 1, bursts[i].incr_length() <= 16))};
    }

    con&={(bursts_nnum_bytes_acc[0]() == bursts[0].nnum_bytes())};
    for (int i = 1; i < 16; i++) {
      con&={(bursts_nnum_bytes_acc[i]() == bursts_nnum_bytes_acc[i - 1]() + bursts[i].nnum_bytes())};
    }

    for (int i = 0; i < 16; i++) {
      con&={(if_then(legal_size() - 1 == i, bursts_nnum_bytes_acc[i]() == 16))};
    }
  }
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");

  mem_burst_16 ahb_seq("item");

  ahb_burst ab("burst");

  for (int i = 0; i < 5; i++) {
    assert(ab.randomize());
    std::cout << boost::format("ab[%u]") % i << std::endl;
    ab.print();
  }

  std::cout << "-----------" << std::endl;

  for (int i = 0; i < 10; i++) {
    assert(ahb_seq.randomize());
    std::cout << boost::format("burst_size = %d") % ahb_seq.legal_size << std::endl;
    for (int j = 0; j < ahb_seq.legal_size; j++) {
      ahb_seq.bursts[j].print();
    }
    std::cout << std::endl << std::endl << std::endl << "********" << std::endl;
  }

  return 0;
}
