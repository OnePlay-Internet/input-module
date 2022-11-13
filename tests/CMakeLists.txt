add_executable(inputtest main.cpp)

target_include_directories(inputtest PRIVATE 
    ../include
    ../src
    ${libmatoya_ROOT_DIR}/src)

target_link_libraries(inputtest PRIVATE 
    inputmodule::inputmodule 
    ${libmatoya}
    bcrypt.lib d3d11.lib d3d9.lib hid.lib uuid.lib dxguid.lib opengl32.lib ws2_32.lib user32.lib gdi32.lib xinput9_1_0.lib ole32.lib shell32.lib windowscodecs.lib shlwapi.lib imm32.lib winmm.lib winhttp.lib secur32.lib crypt32.lib advapi32.lib userenv.lib)