#include <crave/ConstrainedRandom.hpp>
#include <crave/experimental/Experimental.hpp>

using namespace crave;

struct irqmp_regs : public crv_sequence_item {
  crv_variable<unsigned> level_reg;
  crv_variable<unsigned> force_reg;

  crv_constraint level_reg_cstr{ level_reg() < (1 << 16), (level_reg() & 1) == 0 };
  crv_constraint force_reg_cstr{(force_reg() & 0xFFFF0001) == 0 };

  irqmp_regs(crv_object_name) {}
};

struct irqmp_covergroup : public crv_covergroup {
  crv_variable<unsigned> lr;
  crv_variable<unsigned> fr;

  crv_coverpoint fwd_lvl_1{ "fwd_lvl_1" };
  crv_coverpoint fwd_lvl_0{ "fwd_lvl_0" };

  expression forced_lvl_1() { return make_expression(fr() & lr()); }
  expression forced_lvl_0() { return make_expression(fr() & ~lr()); }

  irqmp_covergroup(crv_object_name) {
    for (int k = 1; k < 16; k++) {
      fwd_lvl_1.bins((((forced_lvl_1() >> k) & 1) == 1) && (forced_lvl_1() < (2 << k)));
      fwd_lvl_0.bins((((forced_lvl_0() >> k) & 1) == 1) && (forced_lvl_0() < (2 << k)) && (forced_lvl_1() == 0));
    }
  }
};

int main(int argc, char *argv[]) {
  crave::init("crave.cfg");
  irqmp_regs regs("regs");
  irqmp_covergroup cg("cg");
  cg.lr.bind(regs.level_reg);
  cg.fr.bind(regs.force_reg);
  for (int i = 0; i < 50000; i++) {
    assert(regs.randomize());
    cg.sample();
    if (cg.covered()) {
      std::cout << "#Iterations = " << (i + 1) << std::endl;
      break;
    }
    if (i == 500) regs.goal(cg);
  }
  cg.report();
}
