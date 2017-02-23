#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

#include <iostream>

using std::ostream;

using namespace crave;

CRAVE_BETTER_ENUM(car_type_enum, AUDI = 1, BMW = 2, MERCEDES = 3, VW = -1);

enum color_enum { RED, GREEN, BLUE };
CRAVE_EXPERIMENTAL_ENUM(color_enum, RED, GREEN, BLUE);

class my_crv_sequence_item : public crv_sequence_item {
 public:
  crv_variable<car_type_enum> car;
  crv_variable<color_enum> color;
  crv_variable<int> power;
  crv_variable<int> price;

  crv_constraint car_color{ if_then(car() == car_type_enum::AUDI, color() != GREEN),
                            if_then(car() == car_type_enum::BMW, color() != RED),
                            if_then(car() == car_type_enum::MERCEDES, color() != BLUE) };
  crv_constraint power_range{ dist(power(), distribution<int>::simple_range(80, 400)) };
  crv_constraint car_power{ if_then(car() == car_type_enum::BMW, power() >= 200) };
  crv_constraint price_range{ inside(price(), std::set<int>{ 20, 30, 40, 50, 60, 70, 80, 90, 100 }) };
  crv_constraint car_price{ if_then(car() == car_type_enum::MERCEDES, price() >= 40),
                            if_then(color() == RED, price() <= 40) };

  my_crv_sequence_item(crv_object_name) {}

  friend ostream& operator<<(ostream& os, my_crv_sequence_item& obj) {
    switch (obj.color) {
      case RED:
        os << "RED";
        break;
      case GREEN:
        os << "GREEN";
        break;
      case BLUE:
        os << "BLUE";
        break;
      default:
        os << "UNKNOWN(" << obj.color << ")";
    }
    os << " ";
    os << obj.car._to_string();
    os << " ";
    os << obj.power;
    os << " ";
    os << obj.price;
    return os;
  }
};

int main(int argc, char* argv[]) {
  crave::init("crave.cfg");
  my_crv_sequence_item obj("obj");
  for (int i = 0; i < 50; i++) {
    CHECK(obj.randomize());
    std::cout << obj << std::endl;
  }
  return 0;
}
