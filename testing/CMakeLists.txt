# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.
enable_testing()


add_subdirectory(./lib/googletest)
set(gtest_SOURCE_DIR ./lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include)
message("gtest_SOURCE_DIR is set to: ${gtest_SOURCE_DIR}")
message("Include directories are: ${gtest_SOURCE_DIR}/include")

set(TEST_SOURCE_FILES
        Mainframe.cpp
)
add_executable(UnitTest
        RunAllTest.cpp
        ${TEST_SOURCE_FILES})

target_link_libraries(UnitTest
        GTest::gtest
        pthread
)

find_package(wxWidgets REQUIRED COMPONENTS core base)

include_directories(${wxWidgets_INCLUDE_DIRS})

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(UnitTest ${wxWidgets_LIBRARIES})
