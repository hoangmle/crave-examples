#include <crave/SystemC.hpp>
#include <crave/ConstrainedRandom.hpp>
#include <systemc.h>
#include <iostream>
#include <crave/experimental/Experimental.hpp>

class mod : public sc_module {
 public:
  mod(sc_module_name) {}

  void thread1();
  void thread2();  
  void thread3();
  
  crave::crv_variable<unsigned> x1;
  crave::crv_variable<unsigned> x2;
  crave::crv_variable<unsigned> x3;
  
  void print();
};

void mod::thread1() { x1.randomize(); }
void mod::thread2() { x2.randomize(); }
void mod::thread3() { x3.randomize(); }

void mod::print() { std::cout << x1 << " " << x2 << " " << x3 << std::endl; }

