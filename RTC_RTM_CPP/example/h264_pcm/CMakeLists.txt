cmake_minimum_required(VERSION 2.4)
project(DefaultSamples)

# Common file parsers
file(GLOB FILE_PARSER_CPP_FILES
     "${PROJECT_SOURCE_DIR}/../common/file_parser/helper_h264_parser.cpp")

# Build sample_send_h264_pcm
file(GLOB SAMPLE_SEND_H264_PCM_CPP_FILES
     "${PROJECT_SOURCE_DIR}/sample_send_h264_pcm.cpp"
     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
add_executable(sample_send_h264_pcm ${SAMPLE_SEND_H264_PCM_CPP_FILES}
                                    ${FILE_PARSER_CPP_FILES})

# Build sample_receive_h264_pcm
file(GLOB SAMPLE_RECEIVE_H264_PCM_CPP_FILES
     "${PROJECT_SOURCE_DIR}/sample_receive_h264_pcm.cpp"
     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
add_executable(sample_receive_h264_pcm ${SAMPLE_RECEIVE_H264_PCM_CPP_FILES})
