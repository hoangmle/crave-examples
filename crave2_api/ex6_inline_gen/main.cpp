#include <crave/ConstrainedRandom.hpp>

using crave::randv;
using crave::Generator;

int main (int argc , char *argv[]) {
    crave::init("crave.cfg");
	randv<int> x(NULL);
	randv<int> y(NULL);
	Generator gen;
	gen
		(x() >= 0 && y() >= 0)
		(x() + y() == 4)
		(x() * y() == 4);

	assert(gen.next());
	std::cout << x << " " << y << std::endl;

	return 0;
}
