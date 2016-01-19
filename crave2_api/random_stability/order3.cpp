#include "common.h"

class mod3 : public mod {
 public:
  SC_HAS_PROCESS(mod3);
  mod3(sc_module_name name) : mod(name) {
    SC_THREAD(thread4);
    SC_THREAD(thread2);
    SC_THREAD(thread3);
    SC_THREAD(thread1);
  }

  crave::randv<unsigned> x4;

  void thread4() { x4.next(); }
};

int sc_main(int argc, char** argv) {
  crave::init("crave.cfg");
  crave::set_global_seed(123456789);
  mod3 m("my_module");
  sc_start();
  m.print();
  return 0;
}
