if ( NOT CRAVE_EXAMPLES_TIMEOUT )
  set(CRAVE_EXAMPLES_TIMEOUT 120)
endif()

include_directories( ${crave_SOURCE_DIR}/src ${metaSMT_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${GLOG_INCLUDE_DIR})

if(SystemC_FOUND)
  include_directories(${SystemC_INCLUDE_DIR})
endif(SystemC_FOUND)

function (add_ex_test ex cfile)
 add_executable(${ex} ${cfile})
 target_link_libraries(${ex} crave ${CMAKE_THREAD_LIBS_INIT} ${GLOG_LIBRARY})
 add_test("${ex}" ${ex})
 set_tests_properties ("${ex}" PROPERTIES TIMEOUT ${CRAVE_EXAMPLES_TIMEOUT}) 
endfunction(add_ex_test)

add_subdirectory(crave2_api)

if (CRAVE_ENABLE_EXPERIMENTAL)
  add_subdirectory(experimental_api)  
endif()

