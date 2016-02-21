#include "crave/ConstrainedRandom.hpp"

#include <boost/format.hpp>
#include <iostream>

using crave::rand_obj;
using crave::randv;
using crave::rand_vec;
using crave::dist;
using crave::distribution;
using crave::inside;
using crave::reference;
using crave::placeholder;
using crave::if_then;
using crave::if_then_else;
using crave::foreach;

enum kkind_e { SINGLE, INCR, WRAP4, INCR4, WRAP8, INCR8, WRAP16, INCR16 };
CRAVE_ENUM(kkind_e, (SINGLE)(INCR)(WRAP4)(INCR4)(WRAP8)(INCR8)(WRAP16)(INCR16));

enum ssize_e { BYTE, HALFWORD, WORD };
CRAVE_ENUM(ssize_e, (BYTE)(HALFWORD)(WORD));

class ahb_burst : public rand_obj {
 public:
  randv<kkind_e> kkind;
  randv<ssize_e> ssize;
  randv<unsigned int> incr_length;
  randv<unsigned int> num_transfers;
  randv<unsigned int> nnum_bytes;
  randv<unsigned int> address;

  ahb_burst(rand_obj* parent = 0)
      : rand_obj(parent),
        kkind(this),
        ssize(this),
        incr_length(this),
        num_transfers(this),
        nnum_bytes(this),
        address(this) {

    constraint("word_aligned", if_then(ssize() == WORD, address() % 4 == 0));
    constraint("halfword_aligned", if_then(ssize() == HALFWORD, address() % 2 == 0));

    constraint("legal_incr_ssize", incr_length() > 0);

    constraint("single_xfer", if_then(kkind() == SINGLE, num_transfers() == 1));
    constraint("inc_xfer", if_then(kkind() == INCR, num_transfers() == incr_length()));
    constraint("4B", if_then(kkind() == WRAP4 || kkind() == INCR4, num_transfers() == 4));
    constraint("8B", if_then(kkind() == WRAP8 || kkind() == INCR8, num_transfers() == 8));
    constraint("16B", if_then(kkind() == WRAP16 || kkind() == INCR16, num_transfers() == 16));

    for (int i = 0; i < 3; i++) {
      constraint(if_then(i == ssize(), nnum_bytes() == (num_transfers() << i)));
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

class mem_burst_16 : public rand_obj {
 public:
  ahb_burst bursts[16];
  randv<unsigned int> legal_size;
  randv<unsigned int> bursts_nnum_bytes_acc[16];

  mem_burst_16(rand_obj* parent = 0) : rand_obj(parent), legal_size(this) {
    for (int i = 0; i < 16; i++) 
      this->add_obj_child(&bursts[i]);

    constraint("legal_size", dist(legal_size(), distribution<uint>::simple_range(1, 16)));

    constraint(bursts[0].address() == 0);
    for (int i = 1; i < 16; i++) {
      constraint(bursts[i].address() == bursts[i - 1].address() + bursts[i - 1].nnum_bytes());
    }

    for (int i = 0; i < 16; i++) {
      constraint(if_then(bursts[i].kkind() == 1, bursts[i].incr_length() <= 16));
    }

    constraint(bursts_nnum_bytes_acc[0]() == bursts[0].nnum_bytes());
    for (int i = 1; i < 16; i++) {
      constraint(bursts_nnum_bytes_acc[i]() == bursts_nnum_bytes_acc[i - 1]() + bursts[i].nnum_bytes());
    }

    for (int i = 0; i < 16; i++) {
      constraint(if_then(legal_size() - 1 == i, bursts_nnum_bytes_acc[i]() == 16));
    }
  }
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");

  mem_burst_16 ahb_seq;

  ahb_burst ab;

  for (int i = 0; i < 5; i++) {
    assert(ab.next());
    std::cout << boost::format("ab[%u]") % i << std::endl;
    ab.print();
  }

  std::cout << "-----------" << std::endl;

  for (int i = 0; i < 10; i++) {
    assert(ahb_seq.next());
    std::cout << boost::format("burst_size = %d") % ahb_seq.legal_size << std::endl;
    for (int j = 0; j < ahb_seq.legal_size; j++) {
      ahb_seq.bursts[j].print();
    }
    std::cout << std::endl << std::endl << std::endl << "********" << std::endl;
  }

  return 0;
}
