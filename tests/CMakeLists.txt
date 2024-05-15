include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        testYadroTask
        testYadroTask.cpp
        ../InputParser.cpp ../InputParser.hpp ../TableScheduler.cpp ../TableScheduler.hpp ../Event.hpp ../Logger.hpp ../Logger.cpp ../InvalidFormatException.hpp
)

target_link_libraries(
        testYadroTask
        GTest::gtest_main
)

target_include_directories(testYadroTask PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(testYadroTask)