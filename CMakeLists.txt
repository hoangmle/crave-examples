find_package(Boost)
include_directories( ${crave_SOURCE_DIR}/src ${metaSMT_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${GLOG_INCLUDE_DIR})

find_library(PTHREAD pthread)

set(EXAMPLES coin_dist crave_functionality ex1_seed_dist ex2_constr_ref
               ex3_inh_soft_next ex4_constr_mng ex5_vec_constr ex6_inline_gen
               ex7_randv_enum ex8_make_expression sudoku sudoku_free)

foreach (ex ${EXAMPLES})
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ex}")
    add_executable(${ex} ${ex}/main.cpp)
    target_link_libraries(${ex} crave ${PTHREAD} ${GLOG_LIBRARY} )
  endif()
endforeach(ex)

if(SystemC_FOUND)
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ALU")
    include_directories(crave/src metaSMT/src)
    add_subdirectory(ALU)
  endif()
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/y86_err_injection")
	add_subdirectory(y86_err_injection)
  endif()
endif(SystemC_FOUND)
