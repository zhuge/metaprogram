file(GLOB TESTS_SRC ./*.cpp)
add_executable(metaprogram_test ${TESTS_SRC})

target_include_directories(metaprogram_test PRIVATE ../thirdparty/Catch2/single_include)

# add test 
add_test(NAME metaprogram_test COMMAND metaprogram_test)