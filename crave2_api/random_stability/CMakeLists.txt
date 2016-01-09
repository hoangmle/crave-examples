find_package(Threads REQUIRED)

add_executable(order1 order1.cpp)
target_link_libraries(order1 crave ${SystemC_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_THREAD_LIBRARY} ${GLOG_LIBRARY})
add_executable(order2 order2.cpp)
target_link_libraries(order2 crave ${SystemC_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_THREAD_LIBRARY} ${GLOG_LIBRARY})
add_executable(order3 order3.cpp)
target_link_libraries(order3 crave ${SystemC_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_THREAD_LIBRARY} ${GLOG_LIBRARY})

