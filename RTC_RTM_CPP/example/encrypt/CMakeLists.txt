cmake_minimum_required(VERSION 2.4)
project(DefaultSamples)

# Common file parsers
file(GLOB FILE_PARSER_CPP_FILES
     "${PROJECT_SOURCE_DIR}/../common/file_parser/helper_h264_parser.cpp")

# Build sample_send_encrypted_h264
file(GLOB SAMPLE_SEND_ENCRYPTED_H264_CPP_FILES
     "${PROJECT_SOURCE_DIR}/sample_send_encrypted_h264.cpp"
     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
add_executable(sample_send_encrypted_h264
               ${SAMPLE_SEND_ENCRYPTED_H264_CPP_FILES} ${FILE_PARSER_CPP_FILES})

# Build sample_receive_decrypted_h264
file(GLOB SAMPLE_RECEIVE_DECRYPTED_H264_CPP_FILES
     "${PROJECT_SOURCE_DIR}/sample_receive_decrypted_h264.cpp"
     "${PROJECT_SOURCE_DIR}/../common/*.cpp")
add_executable(sample_receive_decrypted_h264
               ${SAMPLE_RECEIVE_DECRYPTED_H264_CPP_FILES})
