cmake_minimum_required(VERSION 3.8)

project(ug03_cn)

string(TOLOWER ${CMAKE_BUILD_TYPE} ARPG_BUILD_TYPE) 
message("app build type:" ${ARPG_BUILD_TYPE})
  
if(MSVC)
  ADD_DEFINITIONS(/W3 /wd"4996" /wd"4819" /wd"4091" /wd"4099" /MP /Gm-)
 # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
 # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -Wall -std=c++11 -D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -Wall -DNDEBUG -std=c++11")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -g -Wall -DNDEBUG -std=c++11")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -Wall -DNDEBUG -std=c++11")  
endif(MSVC)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
set(DATAACCESS_INCLUDE ${PROJECT_SOURCE_DIR}/3rdpart/fastdb ${PROJECT_SOURCE_DIR}/src/dataaccess/datalayer)
set(PROTOBUF_INCLUDE ${PROJECT_SOURCE_DIR}/3rdpart/protobuf)

if(MSVC)
  if(CMAKE_CL_64)
    include_directories(${PROJECT_SOURCE_DIR}/3rdpart/mysqlconnector/v64_win/include)
    link_directories(${PROJECT_SOURCE_DIR}/lib/msvc_x64/x64-Debug)
    link_directories(${PROJECT_SOURCE_DIR}/3rdpart/mysqlconnector/v64_win/lib)
  else()
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/3rdpart/mysqlconnector/v32/include)
    link_directories(${PROJECT_SOURCE_DIR}/lib/msvc_x86/x86-Debug)
    link_directories(${PROJECT_SOURCE_DIR}/3rdpart/mysqlconnector/v32/lib)
  endif()
else()  
  if(${CMAKE_BUILD_TYPE} STREQUAL "debug")	
	include_directories(${PROJECT_SOURCE_DIR}/3rdpart/mysqlconnector/v64_linux/include)
	link_directories(${PROJECT_SOURCE_DIR}/lib/linux_x64/Linux-Debug)
	link_directories(${PROJECT_SOURCE_DIR}/3rdpart/mysqlconnector/v64_linux/lib/)
  else()
	include_directories(${PROJECT_SOURCE_DIR}/3rdpart/mysqlconnector/v64_linux/include)
	link_directories(${PROJECT_SOURCE_DIR}/lib/linux_x64/Linux-Release)
	link_directories(${PROJECT_SOURCE_DIR}/3rdpart/mysqlconnector/v64_linux/lib/)
  endif()
endif(MSVC)

if(MSVC)
  set(LINK_LIB_COMMON config.dll network.dll rmi.dll log.dll gameserver.lib utility.lib basicnetwork.lib common.lib libmysql.dll)
  #SET(LINK_LIB_FASTDB fastdb.lib)
  #set(LINK_LIB_PROTOBUF)
else()
  set(LINK_LIB_COMMON pthread rt libconfig.so libnetwork.so librmi.so liblog.so libgameserver.a libutility.a libbasicnetwork.a libcommon.a libmysql.so libmysqlclient.so)
  #SET(LINK_LIB_FASTDB libfastdb.a)
  #set(LINK_LIB_PROTOBUF)
endif(MSVC)

file(GLOB_RECURSE COMMON_FILES "src/servercommon/*.cpp" "src/servercommon/*.h" "src/servercommon/*.hpp")
file(GLOB_RECURSE FASTDB_FILES "3rdpart/fastdb/*.cpp" "3rdpart/fastdb/*.h")
file(GLOB_RECURSE SRC_GAMEWORLD "src/gameworld/*.cpp" "src/gameworld/*.hpp" "src/gameworld/*.h")
file(GLOB_RECURSE SRC_GATEWAY "src/gateway/*.cpp" "src/gateway/*.hpp" "src/gateway/*.h")
file(GLOB_RECURSE SRC_LOGINSERVER "src/loginserver/*.cpp" "src/loginserver/*.hpp" "src/loginserver/*.h")
file(GLOB_RECURSE SRC_DATAACCESS "src/dataaccess/*.cpp" "src/dataaccess/*.hpp" "src/dataaccess/*.h")
file(GLOB_RECURSE SRC_CROSSSERVER "src/crossserver/*.cpp" "src/crossserver/*.hpp" "src/crossserver/*.h")

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_subdirectory(src/dataaccess)
add_subdirectory(src/loginserver)
add_subdirectory(src/gateway)
add_subdirectory(src/gameworld)
add_subdirectory(src/crossserver)

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "EXEgameworld_${ARPG_BUILD_TYPE}" )
