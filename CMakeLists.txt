set(Boost_NO_BOOST_CMAKE true)
set(Boost_NO_SYSTEM_PATHS true)
set(Boost_ADDITIONAL_VERSIONS "1.50.0")
find_package ( Boost )

include_directories( ${crave_SOURCE_DIR}/src ${metaSMT_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${GLOG_INCLUDE_DIR})

find_package(SystemC QUIET)

if(SystemC_FOUND)
  include_directories(${SystemC_INCLUDE_DIR})
endif(SystemC_FOUND)

add_subdirectory(crave2_api)

if (CRAVE_ENABLE_EXPERIMENTAL)
  add_subdirectory(experimental_api)  
endif()

