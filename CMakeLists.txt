cmake_minimum_required(VERSION 2.6)
project(TEST)

set(CMAKE_C_FLAGS   "-g -Wall")
set(CMAKE_CXX_FLAGS "-g -Wall")
# add googletest path to project
add_subdirectory (${PROJECT_SOURCE_DIR}/googletest)

set(SRC_DIR        ${PROJECT_SOURCE_DIR}/src)
set(TEST_DIR       ${PROJECT_SOURCE_DIR}/test)
set(MOCK_DIR       ${PROJECT_SOURCE_DIR}/test/mock)

include_directories(${SRC_DIR} ${MOCK_DIR})

set(SRC_OBJS    ${SRC_DIR}/mail.c)
set(TEST_OBJS   ${TEST_DIR}/gtest_example.cc
                ${TEST_DIR}/gmock_example_c_version.cc)
set(MOCK_OBJS   ${MOCK_DIR}/mockNetwork.cc)

add_executable(runTests ${TEST_OBJS} ${SRC_OBJS} ${MOCK_OBJS})
target_link_libraries(runTests gmock)
