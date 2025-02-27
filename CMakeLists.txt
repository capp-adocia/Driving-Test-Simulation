cmake_minimum_required(VERSION 3.12)
add_definitions(-DDEBUG)

project(OpenGL_Lecture)

set(CMAKE_CXX_STANDARD 17)

# 复制 assets 文件夹（包括子目录和文件）
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# 判断操作系统
if(WIN32)
    # 获取所有的 DLL 文件
    file(GLOB DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/bin/*.dll")
    # 将 DLL 文件复制到可执行文件所在目录
    foreach(dll_file ${DLL_FILES})
        get_filename_component(dll_name ${dll_file} NAME)
        file(COPY ${dll_file} DESTINATION ${CMAKE_BINARY_DIR})
        message(STATUS "Copied ${dll_name} to ${CMAKE_BINARY_DIR}")
    endforeach()
endif()

# 设置编译器警告等级
if(MSVC)
    # 对于 MSVC 编译器
    add_compile_options(/W4 /WX) # W4: 高警告级别，WX: 将警告当作错误
elseif(CMAKE_COMPILER_IS_GNUCXX)
    # 对于 GCC 或 Clang 编译器
    add_compile_options(-Wall -Wextra -Wpedantic -Wunused-variable -Werror)
endif()

# 包含第三方库
include_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include/physx
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include/SDL2
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib/Debug
)

# 添加子目录
set(SUBDIRECTORIES
    application
    render
    physics
    imgui
    core
    util
    resource
)

foreach(subdir ${SUBDIRECTORIES})
    add_subdirectory(${subdir})
endforeach()

# 生成目标文件
add_executable(OpenGL_PhysX_SDL2 "main.cpp" "glad.c")

# 设置库文件
set(LIBRARIES
    glfw3.lib
    assimp-vc142-mtd.lib
    zlibstaticd.lib
    SDL2d.lib
    SDL2maind.lib
    PhysX_64.lib
    PhysXCharacterKinematic_static_64.lib
    PhysXCommon_64.lib
    PhysXCooking_64.lib
    PhysXExtensions_static_64.lib
    PhysXFoundation_64.lib
    PhysXPvdSDK_static_64.lib
    PhysXTask_static_64.lib
    PhysXVehicle_static_64.lib
    PhysXVehicle2_static_64.lib
)

target_link_libraries(OpenGL_PhysX_SDL2
    PRIVATE
    ${LIBRARIES}
    app
    render
    physics
    imgui
    core
    util
    resource
)