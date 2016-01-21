#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using crave::crv_variable;

int main(int argc, char *argv[]) {
  crave::init("crave.cfg");
  crv_variable<int> x;
  crv_variable<int> y;

  assert( crave::solve(x() >= 0, y() >= 0, x() + y() == 4, x() * y() == 4) );

  std::cout << x << " " << y << std::endl;

  return 0;
}
