#-------------------------------------------------------------------------------
# Basic Http Server
#
# SPDX-License-Identifier: GPL-3.0

include(FetchContent)
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(basic_http_server)

target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/app/include src)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

set(gen_dir ${ZEPHYR_BINARY_DIR}/include/generated/)

generate_inc_file_for_target(app src/public_html/response_small.html.bin ${gen_dir}/response_small.html.bin.inc)
generate_inc_file_for_target(app src/public_html/response_big.html.bin   ${gen_dir}/response_big.html.bin.inc)
generate_inc_file_for_target(app src/public_html/response_404.html.bin   ${gen_dir}/response_404.html.bin.inc)

FetchContent_Declare(llhttp
  URL "https://github.com/nodejs/llhttp/archive/refs/tags/release/v8.1.0.tar.gz")

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "")
FetchContent_MakeAvailable(llhttp)
#
get_target_property(LLHTTP_HEADER llhttp_static PUBLIC_HEADER)
get_filename_component(LLHTTP_HEADER_DIR ${LLHTTP_HEADER} DIRECTORY)
## Link with the llhttp_static target
target_link_libraries(app PUBLIC llhttp_static)
target_include_directories(app PUBLIC ${LLHTTP_HEADER_DIR})
