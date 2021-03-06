
cmake_minimum_required(VERSION 3.1)
project(moonlight-common-c LANGUAGES C)

SET(CMAKE_C_STANDARD 11)

find_package(OpenSSL 1.0.2 REQUIRED)

aux_source_directory(src SRC_LIST)
aux_source_directory(enet SRC_LIST)
aux_source_directory(reedsolomon SRC_LIST)

if(MSVC)
  add_compile_options(/W4 /wd4100 /wd4232)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1 -D_CRT_NONSTDC_NO_DEPRECATE=1)
  link_libraries(ws2_32.lib qwave.lib winmm.lib)
else()
  add_compile_options(-Wall -Wextra -Wno-unused-parameter -Werror)
endif()

string(TOUPPER "x${CMAKE_BUILD_TYPE}" BUILD_TYPE)
if("${BUILD_TYPE}" STREQUAL "XDEBUG")
	add_definitions(-DLC_DEBUG)
else()
	add_definitions(-DNDEBUG)
endif()

add_definitions(-DHAS_SOCKLEN_T)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/enet/include
  ${CMAKE_CURRENT_SOURCE_DIR}/reedsolomon
  ${OPENSSL_INCLUDE_DIR})

link_libraries(${OPENSSL_CRYPTO_LIBRARY})

add_library(moonlight-common-c SHARED ${SRC_LIST})
