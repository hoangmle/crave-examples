#include <crave/ConstrainedRandom.hpp>

using crave::rand_obj;
using crave::randv;
using crave::distribution;
using crave::weighted_range;

class item : public rand_obj {
public:
	item(rand_obj* parent = 0) : rand_obj(parent), src_addr(this), dest_addr(this) {
    src_addr.dist(
      distribution<uint>::create
        (weighted_range<uint>(0, 9))
        (weighted_range<uint>(90, 99))
    );
		constraint(dest_addr() % 4 == 0);
		constraint(dest_addr() <= reference(src_addr) + 3); 
	} 
     
	friend ostream& operator<<(ostream& os, item& it) { 
		os << it.src_addr << " " << it.dest_addr;
		return os; 
	}

	randv<uint> src_addr;
	randv<uint> dest_addr;
};

int main (int argc , char *argv[]) {
	crave::init("crave.cfg");
	item it;
	for (int i = 0; i < 20; i++) {
		it.next();
		std::cout << it << std::endl;
	}
	return 0;
}
