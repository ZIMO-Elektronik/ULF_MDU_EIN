include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(ULF_MDU_EINTests ${SRC})

sanitize(address,undefined)

target_common_warnings(ULF_MDU_EINTests PRIVATE)

cpmaddpackage(URI "gh:google/googletest#main" OPTIONS "INSTALL_GTEST OFF")

target_link_libraries(ULF_MDU_EINTests PRIVATE GTest::gtest_main GTest::gmock)

gtest_discover_tests(ULF_MDU_EINTests)
