# Find libusb
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

# Find spdlog
find_package(spdlog REQUIRED)

# Find all test source files
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

# Find all source files, excluding main.cpp
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRCS ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Iterate through each test source file
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${SRCS})

    # Link libraries
    target_link_libraries(${TEST_NAME} PRIVATE ${LIBUSB_LIBRARIES})
    target_include_directories(${TEST_NAME} PRIVATE ${LIBUSB_INCLUDE_DIRS})

    # Include directories
    target_link_libraries(${TEST_NAME} PRIVATE spdlog::spdlog)
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

    # Install the test binaries
    install(TARGETS ${TEST_NAME} DESTINATION bin/tests)
endforeach()
