cmake_minimum_required(VERSION 2.4)

project(AgoraSDKDemo)

include(${CMAKE_SOURCE_DIR}/scripts/env.cmake)
include(${CMAKE_SOURCE_DIR}/scripts/os.cmake)
include(${CMAKE_SOURCE_DIR}/scripts/check.cmake)

if("${MEM_CHECK}" STREQUAL "true")
  set(sanitizer -fsanitize=address)
endif()

set(AGORA_MEDIA_SDK_DIR ${CMAKE_SOURCE_DIR}/../agora_sdk)
set(AGORA_MEDIA_SDK_HEADER_DIR ${CMAKE_SOURCE_DIR}/../agora_sdk)

# Set common flags
set(CMAKE_DEBUG_POSTFIX "")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -Wno-unused-command-line-argument ${sanitizer}")

if("${LICENSE_CHECK}" STREQUAL "y")
  message("license check is enabled")
  add_definitions(-DLICENSE_CHECK)
endif()

# include header path
include_directories(
  "${THIRD_PARTY}/include"
  "${THIRD_PARTY}/http_parser/include"
  "${THIRD_PARTY}/json_parser/include"
  "${THIRD_PARTY}/opusfile_parser/include"
  "${CMAKE_SOURCE_DIR}"
  "${AGORA_MEDIA_SDK_HEADER_DIR}/include")

# Link common libs
link_directories(${AGORA_MEDIA_SDK_DIR}/lib/${MACHINE}
	${AGORA_MEDIA_SDK_DIR}/lib
	${AGORA_MEDIA_SDK_DIR}
	${THIRD_PARTY}/http_parser/lib/${MACHINE}
	${THIRD_PARTY}/opusfile_parser/lib/${MACHINE}
	)

#link_libraries(${LIB_SDK_RTC} ${LIB_SDK_RTM} ${LIB_THREAD})
#link_libraries(${LIB_THREAD})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/out/${MACHINE})

# Build apps
#subdirlist(SUBDIRS ${CMAKE_SOURCE_DIR})
set(TARGETDIR "yuv_pcm")
if(EXISTS ${CMAKE_SOURCE_DIR}/${TARGETDIR}/CMakeLists.txt)
   add_subdirectory(${TARGETDIR})
endif()
