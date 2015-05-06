#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

#include <bitset>
 
struct my_covergroup : public crave::crv_covergroup {
  CRV_VARIABLE(unsigned, x);
  CRV_VARIABLE(unsigned, y);

  CRV_COVERPOINT(cp1);
  CRV_COVERPOINT(cp2);
  CRV_COVERPOINT(cp3);
  CRV_COVERPOINT(cp4);
   
  my_covergroup(crave::crv_object_name) { 
    for (int i = 0; i < 16; i++) {
      cp1.bins( (x() >> i) == 1 );
      cp2.bins( ((y() >> i) & 1) != 0 );
      cp3.bins( ((x() >> i) == 1) && (((y() >> i) & 1) != 0) );
      cp4.bins( bitcount(x()) == i );
    }  
  }
};

struct irqmp_item : public crave::crv_sequence_item {
  CRV_VARIABLE(unsigned, pending); 
  CRV_VARIABLE(unsigned, priority); 
  
  CRV_CONSTRAINT(pending_constr);
  CRV_CONSTRAINT(priority_constr);

	irqmp_item(crave::crv_object_name) { 
	  pending_constr = { crave::dist(pending(), crave::distribution<unsigned>::simple_range(0, 1 << 16)) }; 
	  priority_constr = { crave::dist(priority(), crave::distribution<unsigned>::simple_range(0, 1 << 16)) };
	}   

  void print() {
    std::cout << "pending  = " << pending << " # " << std::bitset<32>(pending) << std::endl;
    std::cout << "priority = " << priority << " # " << std::bitset<32>(priority) << std::endl;
  }
};

template<int N>
struct item : public crave::crv_sequence_item {
  CRV_VARIABLE(unsigned, x); // crv_variable<unsigned> x { "x" };
  CRV_VARIABLE(unsigned, y); // crv_variable<unsigned> y { "y" };
  CRV_ARRAY(unsigned, N, arr); // crv_array<unsigned, N> arr { "arr" };
  
  CRV_CONSTRAINT(sum_arr, arr.sum() == 20); // crv_constraint sum_arr { "sum_arr", arr.sum() == 20 };
  CRV_CONSTRAINT(x_constr, { x() > (1 << N), onehot(x()) });
  CRV_CONSTRAINT(y_constr); //crv_constraint y_constr { "y_constr" };
  CRV_CONSTRAINT(elem_constr);

	item(crave::crv_object_name) { 
    y_constr = { y() > arr.sum(), bitcount(y()) == N, y() % 2 == 1 };
    for (int i = 0; i < N; i++)
      elem_constr(0 < arr[i]() && arr[i]() < 10);
	}   

  void print() {
    std::cout << "arr = ";
    for (int i = 0; i < N; i++)
      std::cout << " " << arr[i];
    std::cout << std::endl;
    std::cout << "x = " << x << " # " << std::bitset<64>(x) << std::endl;
    std::cout << "y = " << y << " # " << std::bitset<64>(y) << std::endl;
  }

};

int main (int argc , char *argv[]) {
  crave::init("crave.cfg");

	item<5> it("item5");
  assert(it.randomize());
  it.print(); 

	item<15> it1("item15");
  assert(it1.randomize());
  it1.print();

  irqmp_item ii("irqmp_item");

  my_covergroup cg("cg");
  
  cg.x.bind(ii.pending);
  cg.y.bind(ii.priority);
  
  ii.goal(cg);
  
  for (int i = 0; i < 100; i++) {
    assert(ii.randomize());    
    cg.sample();
    if (cg.covered()) {
      std::cout << "#Iterations = " << (i + 1) << std::endl;
      break;
    }
  }

  cg.report();

	return 0;
}
