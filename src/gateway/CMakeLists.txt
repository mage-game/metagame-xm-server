include_directories(.)

add_executable(EXEgateway_${ARPG_BUILD_TYPE} ${COMMON_FILES} ${SRC_GATEWAY} ${ICONV_FILES})
#target_include_directories(EXEgateway_${ARPG_BUILD_TYPE} PUBLIC ./)
target_link_libraries(EXEgateway_${ARPG_BUILD_TYPE} ${LINK_LIB_COMMON})

target_precompile_headers(EXEgateway_${ARPG_BUILD_TYPE} PRIVATE stdafx.h)
