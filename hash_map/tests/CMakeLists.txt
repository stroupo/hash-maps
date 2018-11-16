find_package(doctest REQUIRED)

add_executable(main_test doctest_main.cc tests.cc)
target_link_libraries(main_test PRIVATE doctest::doctest)

add_test(main_test main_test)