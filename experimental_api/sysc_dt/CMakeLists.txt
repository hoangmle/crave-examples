find_package(Threads REQUIRED)

add_executable(sysc_dt_experimental main.cpp)
target_link_libraries(sysc_dt_experimental crave ${SystemC_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${GLOG_LIBRARY})

