find_package(SystemC QUIET)
find_package(Threads REQUIRED)
include_directories(${SystemC_INCLUDE_DIR} ${metaSMT_INCLUDE_DIR})

add_executable(order1_experimental order1.cpp)
target_link_libraries(order1_experimental crave ${SystemC_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_THREAD_LIBRARY} ${GLOG_LIBRARY})
add_executable(order2_experimental order2.cpp)
target_link_libraries(order2_experimental crave ${SystemC_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_THREAD_LIBRARY} ${GLOG_LIBRARY})
add_executable(order3_experimental order3.cpp)
target_link_libraries(order3_experimental crave ${SystemC_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_THREAD_LIBRARY} ${GLOG_LIBRARY})

