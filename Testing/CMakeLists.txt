


# just the copied build from the labyrinths library, and modified to interface
add_library(GeneratorTesting INTERFACE)

#message(${CMAKE_SOURCE_DIR} "source dir")

target_include_directories(GeneratorTesting INTERFACE ${CMAKE_SOURCE_DIR}/include/)

#add_library(LabyrinthsTesting ../src/main.cpp ../src/Labyrinths.cpp )
add_library(LabyrinthsTesting  ${CMAKE_SOURCE_DIR}/src/Labyrinths.cpp )
target_link_libraries(LabyrinthsTesting GeneratorTesting)

#set_target_properties(GeneratorTesting LabyrinthsTesting PROPERTIES CXX_STANDARD 17)


find_package(GTest CONFIG REQUIRED)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# add_library(TestLib INTERFACE)
# target_include_directories(TestLib INTERFACE ../include/)



add_executable(myTest ${CMAKE_SOURCE_DIR}/Testing/LabyrinthPositionTest.cpp)
target_link_libraries(myTest LabyrinthsTesting GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)


set_target_properties(GeneratorTesting LabyrinthsTesting myTest PROPERTIES CXX_STANDARD 17)

add_test(myTest myTest)

# MACRO(CREATE_TEST test_name)
#     set(binary_name "${test_name}TestBIN")
#     #add_executable(${binary_name} EXCLUDE_FROM_ALL "${test_name}Test.cpp")
#     add_executable(${binary_name} ${CMAKE_SOURCE_DIR}/Testing/${test_name}Test.cpp )
#     target_link_libraries(${binary_name} PRIVATE LabyrinthsTesting GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

#     message("vars " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} " name " ${binary_name} "orig dir" ${CMAKE_SOURCE_DIR}/Testing/${test_name}Test.cpp)
#     #add_test("${test_name}TestBIN" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${binary_name})
#     add_test("${test_name}TestBIN" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${binary_name})
#     #ADD_DEPENDENCIES(tests ${binary_name})
# ENDMACRO()

# CREATE_TEST(LabyrinthPosition)

# find_package(GTest CONFIG REQUIRED)
#     target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

#     add_test(AllTestsInMain main)