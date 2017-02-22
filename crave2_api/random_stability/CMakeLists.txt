add_executable(order1 order1.cpp)
target_link_libraries(order1 ${PROJECT_NAME} ${ALL_EXTERNAL_LIBS})
add_executable(order2 order2.cpp)
target_link_libraries(order2 ${PROJECT_NAME} ${ALL_EXTERNAL_LIBS})
add_executable(order3 order3.cpp)
target_link_libraries(order3 ${PROJECT_NAME} ${ALL_EXTERNAL_LIBS})

