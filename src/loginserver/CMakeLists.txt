include_directories(.)

add_executable(EXEloginserver_${ARPG_BUILD_TYPE} ${COMMON_FILES} ${SRC_LOGINSERVER} ${ICONV_FILES})
#target_include_directories(EXEloginserver_${ARPG_BUILD_TYPE} PUBLIC ./)
target_link_libraries(EXEloginserver_${ARPG_BUILD_TYPE} ${LINK_LIB_COMMON})

target_precompile_headers(EXEloginserver_${ARPG_BUILD_TYPE} PRIVATE stdafx.h)

