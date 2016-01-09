set(EXAMPLES ex7_rand_enum ex9_cdg barn_drawer)

foreach (ex ${EXAMPLES})
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ex}")
    add_executable(${ex}_experimental ${ex}/main.cpp)
    target_link_libraries(${ex}_experimental crave ${PTHREAD} ${GLOG_LIBRARY})
  endif()
endforeach(ex)

if(SystemC_FOUND)
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/y86_err_injection")
	  add_subdirectory(y86_err_injection)
  endif()
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/y86_igen")
    add_subdirectory(y86_igen)
  endif()
endif(SystemC_FOUND)

