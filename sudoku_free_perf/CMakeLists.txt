find_package(Boost QUIET COMPONENTS timer)
if(Boost_FOUND)
  add_executable(sudoku_free_perf main.cpp)
  target_link_libraries(sudoku_free_perf crave ${PTHREAD} ${GLOG_LIBRARY} ${Boost_TIMER_LIBRARY})
endif(Boost_FOUND)

