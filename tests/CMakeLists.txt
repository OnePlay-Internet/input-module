add_executable(inputtest main.cpp)

target_include_directories(inputtest PRIVATE 
    ../include
    ../src
    ../third-party)

target_link_libraries(inputtest PRIVATE inputmodule ${Boost_LIBRARIES})