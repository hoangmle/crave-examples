if ( NOT CRAVE_EXAMPLES_TIMEOUT )
  set(CRAVE_EXAMPLES_TIMEOUT 120)
endif()

set(Boost_NO_BOOST_CMAKE true)
set(Boost_NO_SYSTEM_PATHS true)
set(Boost_ADDITIONAL_VERSIONS "1.50.0")
find_package ( Boost )

include_directories( ${crave_SOURCE_DIR}/src ${metaSMT_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${GLOG_INCLUDE_DIR})

find_package(SystemC QUIET)

if(SystemC_FOUND)
  include_directories(${SystemC_INCLUDE_DIR})
endif(SystemC_FOUND)

function (add_ex_test ex cfile)
 add_executable(${ex} ${cfile})
 target_link_libraries(${ex} crave ${PTHREAD} ${GLOG_LIBRARY})
 add_test("${ex}" ${ex})
 set_tests_properties ("${ex}" PROPERTIES TIMEOUT ${CRAVE_EXAMPLES_TIMEOUT}) 
endfunction(add_ex_test)

add_subdirectory(crave2_api)

if (CRAVE_ENABLE_EXPERIMENTAL)
  add_subdirectory(experimental_api)  
endif()

