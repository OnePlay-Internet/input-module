add_executable(inputtest main.cpp)

target_include_directories(inputtest PRIVATE 
    ../include
    ../src)

target_link_libraries(inputtest PRIVATE inputmodule::inputmodule)