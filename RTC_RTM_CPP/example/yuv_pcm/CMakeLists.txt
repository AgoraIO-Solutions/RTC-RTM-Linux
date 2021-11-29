cmake_minimum_required(VERSION 2.4)
project(DefaultSamples)

# Common file parsers
file(GLOB FILE_PARSER_CPP_FILES
     "${PROJECT_SOURCE_DIR}/../common/file_parser/helper_h264_parser.cpp"
     "${PROJECT_SOURCE_DIR}/../common/file_parser/helper_aac_parser.cpp")

# Build sample_send_yuv_pcm
#file(GLOB SAMPLE_SEND_YUV_PCM_CPP_FILES
#     "${PROJECT_SOURCE_DIR}/sample_send_yuv_pcm.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(sample_send_yuv_pcm ${SAMPLE_SEND_YUV_PCM_CPP_FILES})

# Build RTM demo 
#file(GLOB RTMSERVERDEMO_FILES
#     "${PROJECT_SOURCE_DIR}/rtmServerDemo.cpp"
#     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
#add_executable(rtmServerDemo ${RTMSERVERDEMO_FILES})
#target_link_libraries(rtmServerDemo ${LIB_SDK_RTC} ${LIB_SDK_RTM} ${LIB_THREAD})

# Build sample_receive_yuv_pcm
file(GLOB SAMPLE_RECEIVE_YUV_PCM_CPP_FILES
     "${PROJECT_SOURCE_DIR}/sample_receive_yuv_pcm.cpp"
     "${PROJECT_SOURCE_DIR}/sample_linkedin.cpp"
     "${PROJECT_SOURCE_DIR}/rtmServerDemo.cpp"
     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
add_executable(sample_receive_yuv_pcm ${SAMPLE_RECEIVE_YUV_PCM_CPP_FILES})
target_link_libraries(sample_receive_yuv_pcm ${LIB_SDK_RTC} ${LIB_SDK_RTM} ${LIB_THREAD})
