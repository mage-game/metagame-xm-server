include_directories(.)

add_executable(EXEcrossserver_${ARPG_BUILD_TYPE} ${COMMON_FILES} ${SRC_CROSSSERVER} ${ICONV_FILES})
target_include_directories(EXEcrossserver_${ARPG_BUILD_TYPE} PUBLIC crossserver)
target_link_libraries(EXEcrossserver_${ARPG_BUILD_TYPE} ${LINK_LIB_COMMON})

target_precompile_headers(EXEcrossserver_${ARPG_BUILD_TYPE} PRIVATE stdafx.h)

