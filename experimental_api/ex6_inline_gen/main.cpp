#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using crave::crv_constraint;
using crave::crv_variable;
using crave::randv;
using crave::Generator;

int main(int argc, char *argv[]) {
  crave::init("crave.cfg");
  crv_variable<int> x;
  crv_variable<int> y;
  Generator gen;
  gen(x() >= 0 && y() >= 0)(x() + y() == 4)(x() * y() == 4);

  assert(gen.next());
  std::cout << x << " " << y << std::endl;

  return 0;
}
