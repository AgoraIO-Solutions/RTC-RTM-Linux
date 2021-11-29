set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

macro(SUBDIRLIST result curdir)
  file(
    GLOB children
    RELATIVE ${curdir}
    ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

find_path(third_party_path NAMES third-party PATHS ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/../../ NO_DEFAULT_PATH REQUIRED)
set(THIRD_PARTY ${third_party_path}/third-party)

find_library(LIB_SDK_RTC NAMES agora_rtc_sdk agora-rtc-sdk agora-rtsa-sdk PATHS ${CMAKE_SOURCE_DIR}/../agora_sdk/ NO_DEFAULT_PATH REQUIRED)
find_library(LIB_SDK_RTM NAMES agora_rtm_sdk PATHS ${CMAKE_SOURCE_DIR}/../agora_sdk/ NO_DEFAULT_PATH REQUIRED)

#target_link_libraries(rtc_rtm ${LIB_SDK_RTC} ${LIB_SDK_RTM} ${LIB_THREAD})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=.:../agora_sdk/:../../agora_sdk/:../../../agora_sdk/")
