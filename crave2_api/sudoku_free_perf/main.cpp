#include <crave/ConstrainedRandom.hpp>
#include <vector>
#include <boost/timer.hpp>

using crave::rand_obj;
using crave::randv;
using crave::inside;

class sudoku : public rand_obj {
 public:
  randv<short> field[9][9];

  sudoku(rand_obj* parent = 0) : rand_obj(parent) {
    short numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (int i = 0; i < 9; i++)
      for (int j = 0; j < 9; j++) constraint(inside(field[i][j](), numbers));

    for (int i = 0; i < 9; i++)
      for (int j = 0; j < 9; j++)
        for (int k = j + 1; k < 9; k++) constraint(field[i][j]() != field[i][k]());

    for (int j = 0; j < 9; j++)
      for (int i = 0; i < 9; i++)
        for (int k = i + 1; k < 9; k++) constraint(field[i][j]() != field[k][j]());

    for (int i = 0; i < 9; i++)
      for (int j = 0; j < 9; j++)
        constraint(field[i][j]() != field[(i + 1) % 3 + i - (i % 3)][j]())(field[i][j]() !=
                                                                           field[(i + 2) % 3 + i - (i % 3)][j]())

            (field[i][j]() != field[i][(j + 1) % 3 + j - (j % 3)]())(
                field[i][j]() != field[(i + 1) % 3 + i - (i % 3)][(j + 1) % 3 + j - (j % 3)]())(
                field[i][j]() != field[(i + 2) % 3 + i - (i % 3)][(j + 1) % 3 + j - (j % 3)]())

            (field[i][j]() != field[i][(j + 2) % 3 + j - (j % 3)]())(
                field[i][j]() != field[(i + 1) % 3 + i - (i % 3)][(j + 2) % 3 + j - (j % 3)]())(
                field[i][j]() != field[(i + 2) % 3 + i - (i % 3)][(j + 2) % 3 + j - (j % 3)]());
  }
};

template <unsigned N, bool ToBeFailed>
class sudoku_container : public rand_obj {
 public:
  sudoku_container(rand_obj* parent = 0) : rand_obj(parent) {
    for (unsigned i = 0; i < N; i++) {
      list.push_back(new sudoku(this));
      if (ToBeFailed)
        constraint(x[i]() != x[i]());
      else
        constraint(x[i]() == x[i]());
    }
  }

 private:
  std::vector<sudoku*> list;
  randv<unsigned> x[N];
};

int main(int argc, char* argv[]) {
  boost::timer timer;
  crave::init("crave.cfg");
  sudoku_container<9, true> sc;
  sc.constraint.enable_multithreading();
  for (int i = 0; i < 20; i++) {
    std::cout << (sc.next() ? "solved" : "failed") << std::endl;
  }
  std::cout << "complete: " << timer.elapsed() << std::endl;
  return 0;
}
