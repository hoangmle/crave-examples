#include <crave/ConstrainedRandom.hpp>
#include <vector>
#include <boost/timer/timer.hpp>

using crave::rand_obj;
using crave::randv;
using crave::inside;

class sudoku : public rand_obj {
public:
  randv<short> field[9][9];

	sudoku(rand_obj* parent = 0) : rand_obj(parent) {
    short numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
        constraint( inside(field[i][j](), numbers) );

		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				for (int k = j + 1; k < 9; k++)
         	constraint( field[i][j]() != field[i][k]() );

		for (int j = 0; j < 9; j++)
			for (int i = 0; i < 9; i++)
				for (int k = i + 1; k < 9; k++)
         	constraint( field[i][j]() != field[k][j]() );

		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				constraint
          ( field[i][j]() != field[(i + 1) % 3 + i - (i % 3)][ j                       ]() )
          ( field[i][j]() != field[(i + 2) % 3 + i - (i % 3)][ j                       ]() )

          ( field[i][j]() != field[ i                       ][(j + 1) % 3 + j - (j % 3)]() )
          ( field[i][j]() != field[(i + 1) % 3 + i - (i % 3)][(j + 1) % 3 + j - (j % 3)]() )
          ( field[i][j]() != field[(i + 2) % 3 + i - (i % 3)][(j + 1) % 3 + j - (j % 3)]() )

          ( field[i][j]() != field[ i                       ][(j + 2) % 3 + j - (j % 3)]() )
          ( field[i][j]() != field[(i + 1) % 3 + i - (i % 3)][(j + 2) % 3 + j - (j % 3)]() )
          ( field[i][j]() != field[(i + 2) % 3 + i - (i % 3)][(j + 2) % 3 + j - (j % 3)]() );
	}
};

template <unsigned N>
class sudoku_container : public rand_obj {
 public:
	sudoku_container(rand_obj* parent = 0) : rand_obj(parent) {
    for (unsigned i = 0; i < N; i++)
      list.push_back(new sudoku(this));
	}
	
 private:	
  std::vector<sudoku*> list;
};

int main (int argc, char *argv[]) {
  boost::timer::auto_cpu_timer t;
  crave::init("crave.cfg");
	sudoku_container<3> sc;
	for (int i = 0; i < 20; i++) {
    assert(sc.next());
	}
	return 0;
}
