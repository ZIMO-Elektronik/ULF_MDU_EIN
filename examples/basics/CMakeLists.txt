file(GLOB_RECURSE SRC *.cpp)
add_executable(ULF_MDU_EINExamples ${SRC})

sanitize(address,undefined)

target_common_warnings(ULF_MDU_EINExamples PRIVATE)
target_common_errors(ULF_MDU_EINExamples PRIVATE)

target_link_libraries(ULF_MDU_EINExamples PRIVATE ULF::MDU_EIN)
