#include "common.h"

class mod1 : public mod {
 public:
  SC_HAS_PROCESS(mod1);
  mod1(sc_module_name name) : mod(name) {
    SC_THREAD(thread1);
    SC_THREAD(thread2);
    SC_THREAD(thread3);
  }
};

int sc_main(int argc, char** argv) {
  crave::init("crave.cfg");
  crave::set_global_seed(123456789);
  mod1 m("my_module");
  sc_start();
  m.print();
  return 0;
}
