//  Agora RTC/MEDIA SDK
//
//  Created by Jay Zhang in 2020-04.
//  Copyright (c) 2020 Agora.io. All rights reserved.
//

#include <csignal>
#include <cstring>
#include <sstream>
#include <string>
#include <thread>

#include "IAgoraService.h"
#include "NGIAgoraRtcConnection.h"
#include "common/log.h"
#include "common/opt_parser.h"
#include "common/sample_common.h"
#include "common/sample_local_user_observer.h"

#define DEFAULT_VIDEO_FILE "received_video.h264"
#define DEFAULT_FILE_LIMIT (100 * 1024 * 1024)

struct SampleOptions {
  std::string appId;
  std::string channelId;
  std::string userId;
  std::string remoteUserId;
  std::string videoFile = DEFAULT_VIDEO_FILE;
  bool encryptionMode = false;
  std::string encryptionKey;
};

class H264FrameReceiver : public agora::rtc::IVideoEncodedImageReceiver {
 public:
  H264FrameReceiver(const std::string& outputFilePath)
      : outputFilePath_(outputFilePath),
        h264File_(nullptr),
        fileCount(0),
        fileSize_(0) {}

  bool OnEncodedVideoImageReceived(
      const uint8_t* imageBuffer, size_t length,
      const agora::rtc::EncodedVideoFrameInfo& videoEncodedFrameInfo) override;

 private:
  std::string outputFilePath_;
  FILE* h264File_;
  int fileCount;
  int fileSize_;
};

bool H264FrameReceiver::OnEncodedVideoImageReceived(
    const uint8_t* imageBuffer, size_t length,
    const agora::rtc::EncodedVideoFrameInfo& videoEncodedFrameInfo) {
  // Create new file to save received H264 frames
  if (!h264File_) {
    std::string fileName = (++fileCount > 1)
                               ? (outputFilePath_ + to_string(fileCount))
                               : outputFilePath_;
    if (!(h264File_ = fopen(fileName.c_str(), "w+"))) {
      AG_LOG(ERROR, "Failed to create received video file %s",
             fileName.c_str());
      return false;
    }
    AG_LOG(INFO, "Created file %s to save received H264 frames",
           fileName.c_str());
  }

  if (fwrite(imageBuffer, 1, length, h264File_) != length) {
    AG_LOG(ERROR, "Error writing h264 data: %s", std::strerror(errno));
    return false;
  }
  fileSize_ += length;

  // Close the file if size limit is reached
  if (fileSize_ >= DEFAULT_FILE_LIMIT) {
    fclose(h264File_);
    h264File_ = nullptr;
    fileSize_ = 0;
  }
  return true;
}

static bool exitFlag = false;
static void SignalHandler(int sigNo) { exitFlag = true; }

int main(int argc, char* argv[]) {
  SampleOptions options;
  opt_parser optParser;

  optParser.add_long_opt("token", &options.appId,
                         "The token for authentication");
  optParser.add_long_opt("channelId", &options.channelId, "Channel Id");
  optParser.add_long_opt("userId", &options.userId, "User Id / default is 0");
  optParser.add_long_opt("remoteUserId", &options.remoteUserId,
                         "The remote user to receive stream from");
  optParser.add_long_opt("videoFile", &options.videoFile, "Output video file");
  optParser.add_long_opt("encrypt", &options.encryptionMode,
                         "open the encrypt or not");
  optParser.add_long_opt("encryptionKey", &options.encryptionKey,
                         "the encryptionKey ");

  if ((argc <= 1) || !optParser.parse_opts(argc, argv)) {
    std::ostringstream strStream;
    optParser.print_usage(argv[0], strStream);
    std::cout << strStream.str() << std::endl;
    return -1;
  }

  if (options.appId.empty()) {
    AG_LOG(ERROR, "Must provide appId!");
    return -1;
  }

  if (options.channelId.empty()) {
    AG_LOG(ERROR, "Must provide channelId!");
    return -1;
  }

  if (options.encryptionMode == true) {
    if (options.encryptionKey == "") {
      AG_LOG(ERROR, "Must provide encryption key when encryption is enabled!");
      return -1;
    }
  }

  std::signal(SIGQUIT, SignalHandler);
  std::signal(SIGABRT, SignalHandler);
  std::signal(SIGINT, SignalHandler);

  // Create Agora service
  auto service = createAndInitAgoraService(false, true, true,false,options.appId.c_str());
  if (!service) {
    AG_LOG(ERROR, "Failed to creating Agora service!");
  }

  // Create Agora connection

  agora::rtc::RtcConnectionConfiguration ccfg;
  ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_AUDIENCE;
  ccfg.autoSubscribeVideo = false;

  agora::agora_refptr<agora::rtc::IRtcConnection> connection =
      service->createRtcConnection(ccfg);
  if (!connection) {
    AG_LOG(ERROR, "Failed to creating Agora connection!");
    return -1;
  }

  // Subcribe streams from all remote users or specific remote user
  agora::rtc::ILocalUser::VideoSubscriptionOptions subscriptionOptions;
  subscriptionOptions.encodedFrameOnly = true;
  if (options.remoteUserId.empty()) {
    AG_LOG(INFO, "Subscribe streams from all remote users");
    connection->getLocalUser()->subscribeAllVideo(subscriptionOptions);
  } else {
    connection->getLocalUser()->subscribeVideo(options.remoteUserId.c_str(),
                                               subscriptionOptions);
  }

  // open the encryptmode
  if (options.encryptionMode == true) {
    agora::rtc::EncryptionConfig Config;
    Config.encryptionMode = agora::rtc::SM4_128_ECB;
    Config.encryptionKey = options.encryptionKey.c_str();
    if (connection->enableEncryption(options.encryptionMode, Config) < 0) {
      AG_LOG(ERROR, "Failed to enable encryption!");
    }
    AG_LOG(INFO, "Enable encryption successfully");
  }
  // Connect to Agora channel
  if (connection->connect(options.appId.c_str(), options.channelId.c_str(),
                          options.userId.c_str())) {
    AG_LOG(ERROR, "Failed to connect to Agora channel!");
    return -1;
  }

  // Create local user observer
  auto localUserObserver =
      std::make_shared<SampleLocalUserObserver>(connection->getLocalUser());

  // Register h264 frame receiver to receive video stream
  auto h264FrameReceiver =
      std::make_shared<H264FrameReceiver>(options.videoFile);
  localUserObserver->setVideoEncodedImageReceiver(h264FrameReceiver.get());

  // Periodically check exit flag
  while (!exitFlag) {
    usleep(10000);
  }

  // Unregister video frame observers
  localUserObserver->unsetVideoFrameObserver();

  // Disconnect from Agora channel
  if (connection->disconnect()) {
    AG_LOG(ERROR, "Failed to disconnect from Agora channel!");
    return -1;
  }
  AG_LOG(INFO, "Disconnected from Agora channel successfully");

  // Destroy Agora connection and related resources
  localUserObserver.reset();
  h264FrameReceiver.reset();
  connection = nullptr;

  // Destroy Agora Service
  service->release();
  service = nullptr;

  return 0;
}
