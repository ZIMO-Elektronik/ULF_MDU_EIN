include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(ULF_MDU_EINTests ${SRC})

sanitize(address,undefined,leak,bounds)

target_common_warnings(ULF_MDU_EINTests PRIVATE)

cpmaddpackage(URI "gh:google/googletest#main" OPTIONS "INSTALL_GTEST OFF")

# Generate number of fuzz test iterations
string(
  RANDOM
  LENGTH 4
  ALPHABET 0123456789 FUZZ_REPETITIONS)

# Remove leading zeros which C interprets as octal values
math(EXPR FUZZ_REPETITIONS ${FUZZ_REPETITIONS})

target_compile_definitions(ULF_MDU_EINTests
                           PRIVATE FUZZ_REPETITIONS=${FUZZ_REPETITIONS}u)

target_link_libraries(ULF_MDU_EINTests PRIVATE ULF::MDU_EIN GTest::gtest_main
                                               GTest::gmock)

gtest_discover_tests(ULF_MDU_EINTests)
