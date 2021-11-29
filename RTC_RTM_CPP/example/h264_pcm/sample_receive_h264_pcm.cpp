//  Agora RTC/MEDIA SDK
//
//  Created by Jay Zhang in 2020-04.
//  Copyright (c) 2020 Agora.io. All rights reserved.
//

//   ******************         --------         *************         ------------       *********        **********         ************       ------------        ***********       --------
//  {SDK::capture video}  ==>  |raw data|  ==>  {SDK::encode B}  ==>  |encoded data| ==> {SDK::send}  ==> {AGORA::VOS}  ==>  {SDK::receive} ==> |encoded data|  ==> {SDK::decode} ==> |raw data|
//   ******************         --------         *************         ------------       *********        **********         ************       ------------        ***********       --------
//                                                                                                                  sample receive h264(this sample)      

//This sample will show how to use the SDK to receive the encoded_Video to the Agora_channel
//The SDK receive the encoded_Video though the observer class and callback function 
//You should  create the Agora_service,Agora_connection and then connect the channel.
//You also should implement and create the IVideoEncodedImageReceiver(class) and localUserObserver.
//Connect to channel and register LocalUserObserver .
//When the peer start send the video frame , the callback function localUserObserver::onUserVideoTrackSubscribed() will be triggered,
//which could give you the peer’s remote UID and remote video track.
//And then you can register your IVideoEncodedImageReceiver though the function remote_video_track::VideoEncodedImageReceiver 
//The callback function IVideoEncodedImageReceiver::OnEncodedVideoImageReceived could give you the video data.

// the SDKapi call flows:
// * first *  implement the IVideoEncodedImageReceiver class and localUserObserver class
// service = createAndInitAgoraService()
//     connection = service->createRtcConnection()
//        local_user = connection->->getLocalUser()
//         connection->connect()
//             local_user_->registerLocalUserObserver(localUserObserver);
// if( peer strat send video ) SampleLocalUserObserver::onUserVideoTrackSubscribed (IRemoteVideoTrack){
//    remote_video_track_->registerVideoEncodedImageReceiver(IVideoEncodedImageReceiver)
//}

//The details can be found in this sample and common/sample_local_user_observer.h



//audio
//   ******************         --------         *************         ------------       *********        **********         ************       ------------        ***********       --------
//  {SDK::capture video}  ==>  |raw data|  ==>  {SDK::encode B}  ==>  |encoded data| ==> {SDK::send}  ==> {AGORA::VOS}  ==>  {SDK::receive} ==> |encoded data|  ==> {SDK::decode} ==> |raw data|
//   ******************         --------         *************         ------------       *********        **********         ************       ------------        ***********       --------
//                        sample send pcm                              sample send opus                                                                                            sample receive pcm（this sample）
//                                                                                                                                                                                 callback function                                          


//This sample will show how to use the SDK to receive the encoded_Audio to the Agora_channel
//The SDK receive the encoded_Video though the observer class and callback function 
//You should  create the Agora_service,Agora_connection and then connect the channel.
//You also should implement and create the IAudioFrameObserver(class) and localUserObserver.
//Connect to channel , register LocalUserObserver and IAudioFrameObserver.
//There are 4 callback functions could give you the audio data in different scene:

//  suppose the channel has three BROADCASTERs A,B and C(yourself),all of them send audio to the channel
//bool onPlaybackAudioFrame(AudioFrame& audioFrame) override { return true; }; audioFrame = A_audio+B_audio
// bool onRecordAudioFrame(AudioFrame& audioFrame) override { return true; };  audioFrame = C_audio
//bool onMixedAudioFrame(AudioFrame& audioFrame) override { return true; };  audioFrame = A_audio + B_audio + C_audio
// bool onPlaybackAudioFrameBeforeMixing(unsigned int uid, AudioFrame& audioFrame); audioFrame is A_audio or B_audio you can distinguish it by diff_uid

// the SDKapi call flows:
//service = createAndInitAgoraService()
//     connection = service->createRtcConnection()
//        local_user = connection->->getLocalUser()
//         connection->connect()
//             local_user_->registerLocalUserObserver(localUserObserver);
//             local_user_->registerAudioFrameObserver(IAudioFrameObserver);
  

//The details can be found in this sample and common/sample_local_user_observer.h

//The destruct order of all the class can be find in the main function end.

// Wish you have a great experience with Agora_SDK!


#include <csignal>
#include <cstring>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>

#include "IAgoraService.h"
#include "NGIAgoraRtcConnection.h"
#include "common/log.h"
#include "common/opt_parser.h"
#include "common/sample_common.h"
#include "common/sample_local_user_observer.h"

#define DEFAULT_SAMPLE_RATE (16000)
#define DEFAULT_NUM_OF_CHANNELS (1)
#define DEFAULT_AUDIO_FILE "received_audio.pcm"
#define DEFAULT_VIDEO_FILE "received_video.h264"
#define DEFAULT_FILE_LIMIT (100 * 1024 * 1024)
#define STREAM_TYPE_HIGH "high"
#define STREAM_TYPE_LOW "low"

struct SampleOptions {
  std::string appId;
  std::string channelId;
  std::string userId;
  std::string remoteUserId;
  std::string streamType = STREAM_TYPE_HIGH;
  std::string audioFile = DEFAULT_AUDIO_FILE;
  std::string videoFile = DEFAULT_VIDEO_FILE;

  struct {
    int sampleRate = DEFAULT_SAMPLE_RATE;
    int numOfChannels = DEFAULT_NUM_OF_CHANNELS;
  } audio;
};

class PcmFrameObserver : public agora::media::IAudioFrameObserver {
 public:
  PcmFrameObserver(const std::string& outputFilePath)
      : outputFilePath_(outputFilePath),
        pcmFile_(nullptr),
        fileCount(0),
        fileSize_(0) {}

  bool onPlaybackAudioFrame(AudioFrame& audioFrame) override { return true; };

  bool onRecordAudioFrame(AudioFrame& audioFrame) override { return true; };

  bool onMixedAudioFrame(AudioFrame& audioFrame) override { return true; };

  bool onPlaybackAudioFrameBeforeMixing(unsigned int uid,
                                        AudioFrame& audioFrame) override;

 private:
  std::string outputFilePath_;
  FILE* pcmFile_;
  int fileCount;
  int fileSize_;
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

bool PcmFrameObserver::onPlaybackAudioFrameBeforeMixing(
    unsigned int uid, AudioFrame& audioFrame) {
  // Create new file to save received PCM samples
  if (!pcmFile_) {
    std::string fileName = (++fileCount > 1)
                               ? (outputFilePath_ + to_string(fileCount))
                               : outputFilePath_;
    if (!(pcmFile_ = fopen(fileName.c_str(), "w"))) {
      AG_LOG(ERROR, "Failed to create received audio file %s",
             fileName.c_str());
      return false;
    }
    AG_LOG(INFO, "Created file %s to save received PCM samples",
           fileName.c_str());
  }

  // Write PCM samples
  size_t writeBytes =
      audioFrame.samplesPerChannel * audioFrame.channels * sizeof(int16_t);
  if (fwrite(audioFrame.buffer, 1, writeBytes, pcmFile_) != writeBytes) {
    AG_LOG(ERROR, "Error writing decoded audio data: %s", std::strerror(errno));
    return false;
  }
  fileSize_ += writeBytes;

  // Close the file if size limit is reached
  if (fileSize_ >= DEFAULT_FILE_LIMIT) {
    fclose(pcmFile_);
    pcmFile_ = nullptr;
    fileSize_ = 0;
  }
  return true;
}

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
  optParser.add_long_opt("audioFile", &options.audioFile, "Output audio file");
  optParser.add_long_opt("videoFile", &options.videoFile, "Output video file");
  optParser.add_long_opt("sampleRate", &options.audio.sampleRate,
                         "Sample rate for received audio");
  optParser.add_long_opt("numOfChannels", &options.audio.numOfChannels,
                         "Number of channels for received audio");
  optParser.add_long_opt("streamtype", &options.streamType, "the stream  type");

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

  std::signal(SIGQUIT, SignalHandler);
  std::signal(SIGABRT, SignalHandler);
  std::signal(SIGINT, SignalHandler);

  // Create Agora service
  auto service = createAndInitAgoraService(false, true, true);
  if (!service) {
    AG_LOG(ERROR, "Failed to creating Agora service!");
  }

  // Create Agora connection
  agora::rtc::AudioSubscriptionOptions audioSubOpt;
  audioSubOpt.bytesPerSample = agora::rtc::TWO_BYTES_PER_SAMPLE;
  audioSubOpt.numberOfChannels = options.audio.numOfChannels;
  audioSubOpt.sampleRateHz = options.audio.sampleRate;

  agora::rtc::RtcConnectionConfiguration ccfg;
  ccfg.clientRoleType = agora::rtc::CLIENT_ROLE_AUDIENCE;
  ccfg.audioSubscriptionOptions = audioSubOpt;
  ccfg.autoSubscribeAudio = false;
  ccfg.autoSubscribeVideo = false;
  ccfg.enableAudioRecordingOrPlayout =
      false;  // Subscribe audio but without playback

  agora::agora_refptr<agora::rtc::IRtcConnection> connection =
      service->createRtcConnection(ccfg);
  if (!connection) {
    AG_LOG(ERROR, "Failed to creating Agora connection!");
    return -1;
  }

  // Subcribe streams from all remote users or specific remote user
  agora::rtc::ILocalUser::VideoSubscriptionOptions subscriptionOptions;
  subscriptionOptions.encodedFrameOnly = true;
  if (options.streamType == STREAM_TYPE_HIGH) {
    subscriptionOptions.type = agora::rtc::VIDEO_STREAM_HIGH;
  } else if(options.streamType==STREAM_TYPE_LOW){
    subscriptionOptions.type = agora::rtc::VIDEO_STREAM_LOW;
  } else{
    AG_LOG(ERROR, "It is a error stream type");
    return -1;
  }
  if (options.remoteUserId.empty()) {
    AG_LOG(INFO, "Subscribe streams from all remote users");
    connection->getLocalUser()->subscribeAllAudio();
    connection->getLocalUser()->subscribeAllVideo(subscriptionOptions);
  } else {
    connection->getLocalUser()->subscribeAudio(options.remoteUserId.c_str());
    connection->getLocalUser()->subscribeVideo(options.remoteUserId.c_str(),
                                               subscriptionOptions);
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

  // Register audio frame observer to receive audio stream
  auto pcmFrameObserver = std::make_shared<PcmFrameObserver>(options.audioFile);
  if (connection->getLocalUser()->setPlaybackAudioFrameBeforeMixingParameters(
          options.audio.numOfChannels, options.audio.sampleRate)) {
    AG_LOG(ERROR, "Failed to set audio frame parameters!");
    return -1;
  }
  localUserObserver->setAudioFrameObserver(pcmFrameObserver.get());

  // Register h264 frame receiver to receive video stream
  auto h264FrameReceiver =
      std::make_shared<H264FrameReceiver>(options.videoFile);
  localUserObserver->setVideoEncodedImageReceiver(h264FrameReceiver.get());

  // Start receiving incoming media data
  AG_LOG(INFO, "Start receiving audio & video data ...");

  // Periodically check exit flag
  while (!exitFlag) {
    usleep(10000);
  }

  // Unregister audio & video frame observers
  localUserObserver->unsetAudioFrameObserver();
  localUserObserver->unsetVideoFrameObserver();

  // Disconnect from Agora channel
  if (connection->disconnect()) {
    AG_LOG(ERROR, "Failed to disconnect from Agora channel!");
    return -1;
  }
  AG_LOG(INFO, "Disconnected from Agora channel successfully");

  // Destroy Agora connection and related resources
  localUserObserver.reset();
  pcmFrameObserver.reset();
  h264FrameReceiver.reset();
  connection = nullptr;

  // Destroy Agora Service
  service->release();
  service = nullptr;

  return 0;
}
