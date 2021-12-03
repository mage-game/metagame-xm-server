include_directories(.)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
	
	#set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -Wall -std=c++11 -fpermissive -D_DEBUG")
	#set(CMAKE_CXX_FLAGS_MINSIZEREL "-O0 -g -Wall -DNDEBUG -std=c++11 -fpermissive -w")
	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpermissive")
	#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O0 -g -Wall -DNDEBUG -std=c++11 -fpermissive -w")
endif(CMAKE_COMPILER_IS_GNUCXX)


add_executable(EXEdataaccess_${ARPG_BUILD_TYPE} ${COMMON_FILES} ${SRC_DATAACCESS} ${FASTDB_FILES} ${ICONV_FILES})
target_include_directories(EXEdataaccess_${ARPG_BUILD_TYPE} PUBLIC ./ ${DATAACCESS_INCLUDE})
target_link_libraries(EXEdataaccess_${ARPG_BUILD_TYPE} ${LINK_LIB_COMMON})

target_precompile_headers(EXEdataaccess_${ARPG_BUILD_TYPE} PRIVATE stdafx.h)
