cmake_minimum_required(VERSION 3.10)
project(teamtalk-imcore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDK 目录（protobuf 等依赖）
set(SDK_DIR ${CMAKE_SOURCE_DIR}/../.sdk)

# 设置安装目录为 teamtalk/.sdk/teamtalk-imcore
set(CMAKE_INSTALL_PREFIX "${SDK_DIR}/teamtalk-imcore" CACHE PATH "Install directory" FORCE)

########################################################################
# 收集源文件和头文件
file(GLOB_RECURSE PROJECT_HEADERS "include/*.h" "include/*.hpp")
file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp" "src/*.cc" "src/*.c")

########################################################################
# 平台差异化
if(WIN32)
    message(STATUS "Configuring for Windows")
    add_definitions(
        -DWIN32
        -D_WINDOWS
        -D_CRT_SECURE_NO_WARNINGS
        -D_WINSOCK_DEPRECATED_NO_WARNINGS
    )
    if(MSVC)
        add_compile_options(/W3 /utf-8)
    endif()
    set(PLATFORM_LIBS ws2_32)
else()
    message(STATUS "Configuring for Linux")
    add_definitions(
        -DLINUX
        -D_UNIX
        -D_REENTRANT
        -D_FILE_OFFSET_BITS=64
    )
    add_compile_options(-g -W -Wall)
    set(PLATFORM_LIBS pthread dl)
endif()

########################################################################
# 头文件路径
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/teamtalk/imcore/ttidl
    ${CMAKE_SOURCE_DIR}/src
    ${SDK_DIR}/protobuf/include                 # protobuf 头文件
    ${SDK_DIR}/spdlog/include                   # spdlog 头文件
    ${SDK_DIR}/jsoncpp/include                  # jsoncpp 头文件
    ${SDK_DIR}/sqlite3/include                  # sqlite3 头文件
)

# 链接库路径
link_directories(
    ${SDK_DIR}/protobuf/lib                     # protobuf 库
    ${SDK_DIR}/jsoncpp/lib                      # jsoncpp 库
    ${SDK_DIR}/sqlite3/lib                      # sqlite3 库
)

########################################################################
# 构建动态库
add_library(teamtalk-imcore SHARED
    ${PROJECT_SOURCES}
)

# 输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# 链接依赖库
target_link_libraries(teamtalk-imcore 
    protobuf-lite
    jsoncpp
    sqlite3
    ${PLATFORM_LIBS}
)

########################################################################
# 安装规则
# 安装动态库
install(TARGETS teamtalk-imcore
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING 
    PATTERN "*.h" 
    PATTERN "*.hpp"
)

########################################################################
# 构建信息
message(STATUS "==============================================")
message(STATUS "teamtalk-imcore build configuration:")
message(STATUS "  Platform     : ${CMAKE_SYSTEM_NAME}")
message(STATUS "  C++ Standard : ${CMAKE_CXX_STANDARD}")
message(STATUS "  SDK Path     : ${SDK_DIR}")
message(STATUS "  Protobuf     : ${SDK_DIR}/protobuf")
message(STATUS "  Install Path : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Source Files : ${CMAKE_CURRENT_SOURCE_DIR}/src")
message(STATUS "  Header Files : ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "==============================================")
