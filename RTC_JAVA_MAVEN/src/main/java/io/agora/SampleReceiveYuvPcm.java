package io.agora.mainClass;

import java.io.File;
import java.util.Scanner;
import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.BufferedOutputStream;
import java.io.FileNotFoundException;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.WritableByteChannel;

import sun.misc.*;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.HelpFormatter;

import io.agora.rtc.AgoraService;
import io.agora.rtc.AgoraServiceConfig;
import io.agora.rtc.AgoraRtcConn;
import io.agora.rtc.AgoraLocalUser;
import io.agora.rtc.Out;
import io.agora.rtc.AgoraMediaNodeFactory;
import io.agora.rtc.AudioSubscriptionOptions;
import io.agora.rtc.RtcConnConfig;
import io.agora.rtc.VideoSubscriptionOptions;
import io.agora.rtc.DefaultLocalUserObserver;
import io.agora.rtc.DefaultAudioFrameObserver;
import io.agora.rtc.AgoraLocalAudioTrack;
import io.agora.rtc.LocalAudioStats;
import io.agora.rtc.AgoraRemoteAudioTrack;
import io.agora.rtc.RemoteAudioTrackStats;
import io.agora.rtc.IAudioFrameObserver;
import io.agora.rtc.AudioFrame;
import io.agora.rtc.AccountInfo;

import common.SampleCommon;
import common.SampleConnectionObserver;
import common.SampleLocalUserObserver;

public class SampleReceiveYuvPcm {

    public static int CLIENT_ROLE_BROADCASTER = 1;
    public static int CLIENT_ROLE_AUDIENCE = 2;

    // Video stream types.
    public static int VIDEO_STREAM_HIGH = 0;
    public static int VIDEO_STREAM_LOW = 1;

    public static int exitFlag = 0;

    public static void main(String[] args) throws Exception {
        System.out.printf("SampleReceiveYuvPcm main begin\n");

        // options
        String appId = "";
        String channelId = "";
        String channelId1 = "";
        String userId = "bot";
        String remoteUserId = "";
        String remoteUserId1 = "";
        String streamType = "high";
        String audioFile = "received_audio.pcm";
        String audioFile1 = "received_audio1.pcm";
        String videoFile = "received_video.yuv";
	String proxyOn= "true";
        int sampleRate = 48000;
        int numOfChannels = 1;
        int ret = 0;

        // Command Line Parser
        CommandLineParser parser = new BasicParser();
        Options options = new Options();
        options.addOption("token", true, "[must] The token for authentication");
        options.addOption("channelId", true, "[must] Channel Id");
        options.addOption("channelId1", true, "[must] Channel Id1");
        options.addOption("sampleRate", true, "[optional] Sample rate for received audio");
        options.addOption("numOfChannels", true, "[optional] Number of channels for received audio");
        try {
            CommandLine commandLine = parser.parse(options, args);
            if (args.length <= 1) {
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp("SampleReceiveYuvPcm", options);
                System.out.println();
                return;
            }

            if (!commandLine.hasOption("token")) {
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp("SampleReceiveYuvPcm", options);
                System.out.println();
                return;
            }
            appId = commandLine.getOptionValue("token");

            if (!commandLine.hasOption("channelId")) {
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp("SampleReceiveYuvPcm", options);
                System.out.println();
                return;
            }
            channelId = commandLine.getOptionValue("channelId");

            if (!commandLine.hasOption("channelId1")) {
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp("SampleReceiveYuvPcm", options);
                System.out.println();
                return;
            }
            channelId1 = commandLine.getOptionValue("channelId1");

            if (commandLine.hasOption("sampleRate")) {
                sampleRate = Integer.parseInt(commandLine.getOptionValue("sampleRate"));
            }

            if (commandLine.hasOption("numOfChannels")) {
                numOfChannels = Integer.parseInt(commandLine.getOptionValue("numOfChannels"));
            }
        } catch (ParseException exp) {
            HelpFormatter formatter = new HelpFormatter();
            formatter.printHelp("SampleReceiveYuvPcm", options);
            System.out.println();
            return;
        }
        System.out.printf("args appId=%s channelId=%s channelId1=%s sampleRate=%d numOfChannels=%d \n", appId,
                channelId,
                channelId1, sampleRate,
                numOfChannels);

        // handle signal
        SampleSignalHandler.listenTo("ABRT");
        SampleSignalHandler.listenTo("INT");

        // Create Agora service
        AgoraService service = SampleCommon.createAndInitAgoraService(0, 1, 1, appId);
        if (null == service) {
            System.out.printf("createAndInitAgoraService fail\n");
            return;
        }

        // Create Agora connection
        AudioSubscriptionOptions audioSubOpt = new AudioSubscriptionOptions();
        audioSubOpt.setBytesPerSample(2 * numOfChannels);
        audioSubOpt.setNumberOfChannels(numOfChannels);
        audioSubOpt.setSampleRateHz(sampleRate);

        RtcConnConfig ccfg = new RtcConnConfig();
        ccfg.setClientRoleType(CLIENT_ROLE_AUDIENCE);
        ccfg.setAudioSubsOptions(audioSubOpt);
        ccfg.setAutoSubscribeAudio(1);
        ccfg.setAutoSubscribeVideo(0);
        ccfg.setEnableAudioRecordingOrPlayout(0); // Subscribe audio but without playback
        ccfg.setAudioRecvMediaPacket(0);

        AgoraRtcConn conn = service.agoraRtcConnCreate(ccfg);
        if (conn == null) {
            System.out.printf("AgoraService.agoraRtcConnCreate fail\n");
            return;
        }

	// enable cloud proxy
        SampleCommon.setCloudProxy(conn, proxyOn);

        // Subcribe streams from all remote users or specific remote user
        AgoraLocalUser localUser = conn.getLocalUser();
        conn.registerObserver(new SampleConnectionObserver(localUser, remoteUserId));

        SampleLocalUserObserver localUserObserver = new SampleLocalUserObserver(localUser);
        PcmFrameObserver pcmFrameObserver = new PcmFrameObserver(conn, audioFile);
        ret = localUser.setPlaybackAudioFrameBeforeMixingParameters(numOfChannels, sampleRate);
        if (ret != 0) {
            System.out.printf("setPlaybackAudioFrameBeforeMixingParameters fail ret=%d\n", ret);
            return;
        }

        // NOTE: aware the order, without setPlaybackAudioFrameBeforeMixingParameters(),
        // observer cannot be registered
        localUserObserver.setAudioFrameObserver(pcmFrameObserver);

        // Connect to Agora channel
        conn.connect(appId, channelId, userId);

        AgoraRtcConn conn1 = service.agoraRtcConnCreate(ccfg);
        if (conn1 == null) {
            System.out.printf("AgoraService.agoraRtcConnCreate fail\n");
            return;
        }

	// enable cloud proxy
        SampleCommon.setCloudProxy(conn1, proxyOn);

        // Subcribe streams from all remote users or specific remote user
        AgoraLocalUser localUser1 = conn1.getLocalUser();
        conn1.registerObserver(new SampleConnectionObserver(localUser1, remoteUserId1));

        SampleLocalUserObserver localUserObserver1 = new SampleLocalUserObserver(localUser1);
        PcmFrameObserver pcmFrameObserver1 = new PcmFrameObserver(conn1, audioFile1);
        ret = localUser1.setPlaybackAudioFrameBeforeMixingParameters(numOfChannels, sampleRate);
        if (ret != 0) {
            System.out.printf("setPlaybackAudioFrameBeforeMixingParameters fail ret=%d\n", ret);
            return;
        }

        // NOTE: aware the order, without setPlaybackAudioFrameBeforeMixingParameters(),
        // observer cannot be registered
        localUserObserver1.setAudioFrameObserver(pcmFrameObserver1);

        // Connect to Agora channel
        conn1.connect(appId, channelId1, userId);

        // Periodically check exit flag
        while (0 == exitFlag) {
            Thread.sleep(10);
        }

        // Unregister audio & video frame observers
        localUserObserver.unsetAudioFrameObserver();
        localUserObserver1.unsetAudioFrameObserver();

        // Disconnect from Agora channel
        ret = conn.disconnect();
        if (ret != 0) {
            System.out.printf("conn.disconnect fail ret=%d\n", ret);
        }
        System.out.printf("Disconnected from Agora channel successfully\n");
	// disable cloud proxy
	proxyOn = "false";
        SampleCommon.setCloudProxy(conn, proxyOn);

        conn.destroy();

        // Disconnect from Agora channel
        ret = conn1.disconnect();
        if (ret != 0) {
            System.out.printf("conn1.disconnect fail ret=%d\n", ret);
        }
        System.out.printf("Disconnected from Agora channel1 successfully\n");
	// disable cloud proxy
        SampleCommon.setCloudProxy(conn1, proxyOn);

        conn1.destroy();

        // Destroy Agora Service
        service.destroy();
    }

    public static class PcmFrameObserver extends DefaultAudioFrameObserver {
        public PcmFrameObserver(AgoraRtcConn agora_rtc_conn, String outputFilePath) {
	    rtc_conn_ = agora_rtc_conn;
            outputFilePath_ = outputFilePath;
            pcmFile_ = null;
            channel_ = null;
            fileCount = 0;
            fileSize_ = 0;
	    AccountInfo userAccountInfo_;
        }

        @Override
        public int onRecordAudioFrame(AgoraLocalUser agora_local_user, AudioFrame frame) {
            System.out.println("onRecordAudioFrame");
            return 1;
        }

        @Override
        public int onPlaybackAudioFrame(AgoraLocalUser agora_local_user, AudioFrame frame) {
            System.out.println("onPlaybackAudioFrame");
            return 1;
        }

        @Override
        public int onMixedAudioFrame(AgoraLocalUser agora_local_user, AudioFrame frame) {
            System.out.println("onMixedAudioFrame");
            return 1;
        }

        @Override
        public int onPlaybackAudioFrameBeforeMixing(AgoraLocalUser agora_local_user, int uid, AudioFrame audioFrame) {
	    userAccountInfo_ = rtc_conn_.getUserInfoByUid(uid);
            System.out.println(
                    "onPlaybackAudioFrameBeforeMixing" + outputFilePath_ + "channel->" + channel_ + "uid->" + uid
                            + " audioFrame->" + audioFrame + "stringUID: " + userAccountInfo_.getUserAccount());
            // Create new file to save received PCM samples
            if (pcmFile_ == null) {
                String fileName = (++fileCount > 1)
                        ? (outputFilePath_ + fileCount)
                        : outputFilePath_;

                try {
                    pcmFile_ = new FileOutputStream(fileName);
                    channel_ = Channels.newChannel(pcmFile_);
                    System.out.printf("Created file %s to save received PCM samples",
                            fileName);
                } catch (FileNotFoundException e) {
                    System.out.println("Open file fail");
                }
            }

            // Write PCM samples
            int writeBytes = audioFrame.getSamplesPerChannel() * audioFrame.getChannels() * 2;
            try {
                ByteBuffer buffer = audioFrame.getBuffer();
                channel_.write(buffer);
                fileSize_ += writeBytes;

                // Close the file if size limit is reached
                if (fileSize_ >= 100 * 1024 * 1024) {
                    channel_.close();
                    pcmFile_ = null;
                    fileSize_ = 0;
                }
            } catch (IOException e) {
                System.out.println("file write exception");
            }

            return 1;
        }

	private AgoraRtcConn rtc_conn_;
        private String outputFilePath_;
        private FileOutputStream pcmFile_;
        private WritableByteChannel channel_;
        int fileCount;
        int fileSize_;
	AccountInfo userAccountInfo_;
    }

    static class SampleSignalHandler implements SignalHandler {
        public static void listenTo(String name) {
            Signal signal = new Signal(name);
            Signal.handle(signal, new SampleSignalHandler());
        }

        public void handle(Signal signal) {
            System.out.println("Signal " + signal + " raised, terminating...");
            exitFlag = 1;
        }
    }
}
