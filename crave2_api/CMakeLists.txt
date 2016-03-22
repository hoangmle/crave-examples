set(EXAMPLES 
  ex1_seed_dist 
  ex2_constr_ref
  ex3_inh_soft_next
  ex4_constr_mng 
  ex5_vec_constr
  ex6_inline_gen
  ex7_rand_enum
  ex8_make_expression
  sudoku
  sudoku_free
  sudoku_free_perf
  ahb_mem_burst
)

foreach (ex ${EXAMPLES})
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ex}")
    add_ex_test(${ex} ${ex}/main.cpp)
  endif()
endforeach(ex)

if(SystemC_FOUND)
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ALU")
    add_subdirectory(ALU)
  endif()
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/random_stability")
    add_subdirectory(random_stability)
  endif()  
endif(SystemC_FOUND)

