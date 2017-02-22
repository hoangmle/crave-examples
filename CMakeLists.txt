if ( NOT CRAVE_EXAMPLES_TIMEOUT )
  set(CRAVE_EXAMPLES_TIMEOUT 120)
endif()

include_directories(${crave_SOURCE_DIR}/src)

function (add_ex_test ex cfile)
 add_executable(${ex} ${cfile})
 target_link_libraries(${ex} ${PROJECT_NAME} ${ALL_EXTERNAL_LIBS})
 add_test("${ex}" ${ex})
 set_tests_properties ("${ex}" PROPERTIES TIMEOUT ${CRAVE_EXAMPLES_TIMEOUT}) 
endfunction(add_ex_test)

add_subdirectory(crave2_api)

if (CRAVE_ENABLE_EXPERIMENTAL)
  add_subdirectory(experimental_api)  
endif()

