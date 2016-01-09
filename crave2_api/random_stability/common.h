#include <crave/SystemC.hpp>
#include <crave/ConstrainedRandom.hpp>
#include <systemc.h>
#include <iostream>

class mod : public sc_module {
 public:
  mod(sc_module_name) {}

  void thread1();
  void thread2();  
  void thread3();
  
  crave::randv<unsigned> x1;
  crave::randv<unsigned> x2;
  crave::randv<unsigned> x3;
  
  void print();
};

void mod::thread1() { x1.next(); }
void mod::thread2() { x2.next(); }
void mod::thread3() { x3.next(); }

void mod::print() { std::cout << x1 << " " << x2 << " " << x3 << std::endl; }

