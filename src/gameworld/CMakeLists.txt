include_directories(.)

add_executable(EXEgameworld_${ARPG_BUILD_TYPE} ${COMMON_FILES} ${SRC_GAMEWORLD} ${ICONV_FILES})
target_include_directories(EXEgameworld_${ARPG_BUILD_TYPE} PUBLIC gameworld ${PROTOBUF_INCLUDE})
target_link_libraries(EXEgameworld_${ARPG_BUILD_TYPE} ${LINK_LIB_COMMON})

target_precompile_headers(EXEgameworld_${ARPG_BUILD_TYPE} PRIVATE stdafx.h)
