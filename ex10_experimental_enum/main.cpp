#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using namespace crave;

enum car_type_enum {
  AUDI = 1,
  BMW = 2,
  MERCEDES = 3,
  VW = -1
};
CRAVE_EXPERIMENTAL_ENUM(car_type_enum, (AUDI)(BMW)(MERCEDES)(VW));

enum color_enum {
  RED,
  GREEN,
  BLUE
};
CRAVE_EXPERIMENTAL_ENUM(color_enum, (RED)(GREEN)(BLUE));

class my_crv_sequence_item : public crv_sequence_item {
public:
  CRV_VARIABLE(car_type_enum, car);
  CRV_VARIABLE(color_enum, color);
  CRV_VARIABLE(int, power);
  CRV_VARIABLE(int, price);

  CRV_CONSTRAINT(car_color, {
    if_then(car() == AUDI, color() != GREEN),
    if_then(car() == BMW, color() != RED),
    if_then(car() == MERCEDES, color() != BLUE) 
  });
  CRV_CONSTRAINT(power_range, dist(power(), distribution<int>::simple_range(80, 400)));
  CRV_CONSTRAINT(car_power, if_then(car() == BMW, power() >= 200));  
  CRV_CONSTRAINT(price_range, inside(price(), std::set<int>{ 20, 30, 40, 50, 60, 70, 80, 90, 100 }));
  CRV_CONSTRAINT(car_price, {
    if_then(car() == MERCEDES, price() >= 40),
    if_then(color() == RED, price() <= 40) 
  });

  my_crv_sequence_item(crv_object_name) { }

  friend ostream& operator<<(ostream& os, my_crv_sequence_item& obj) {
    switch (obj.color) {
      case RED: os << "RED"; break;
      case GREEN: os << "GREEN"; break;
      case BLUE: os << "BLUE"; break;
      default: os << "UNKNOWN(" << obj.color << ")";
    }
    os << " ";
    switch (obj.car) {
      case AUDI: os << "AUDI"; break;
      case BMW: os << "BMW"; break;
      case MERCEDES: os << "MERCEDES"; break;
      case VW: os << "VW"; break;
      default: os << "UNKNOWN(" << obj.car << ")";
    }
    os << " ";
    os << obj.power;
    os << " ";
    os << obj.price;
     return os;
  }
};

int main (int argc , char *argv[]) {
  crave::init("crave.cfg");
  my_crv_sequence_item obj("obj");
  for (int i = 0; i < 50; i++) {
    assert(obj.randomize());
    std::cout << obj << std::endl;
  }
  return 0;
}
