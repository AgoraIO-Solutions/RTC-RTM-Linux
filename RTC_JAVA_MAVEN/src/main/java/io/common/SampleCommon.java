package common;

import io.agora.rtc.AgoraService;
import io.agora.rtc.AgoraServiceConfig;
import io.agora.rtc.SDK;

public class SampleCommon {

    public static String DEFAULT_LOG_PATH = "/tmp/agorasdk.log";
    public static int DEFAULT_LOG_SIZE = 512 * 1024; // default log size is 512 kb

    public static AgoraService createAndInitAgoraService(int enableAudioDevice, int enableAudioProcessor,
            int enableVideo) {
        SDK.load(); // ensure JNI library load
        AgoraService service = new AgoraService();
        AgoraServiceConfig config = new AgoraServiceConfig();
        config.setEnableAudioDevice(enableAudioDevice);
        config.setEnableAudioProcessor(enableAudioProcessor);
        config.setEnableVideo(enableVideo);
        config.setUseStringUid(1);
        int ret = service.initialize(config);
        if (ret != 0) {
            System.out.printf("createAndInitAgoraService AgoraService.initialize fail ret=%d\n", ret);
            return null;
        }

        System.out.printf("createAndInitAgoraService created log file at %s\n", DEFAULT_LOG_PATH);
        ret = service.setLogFile(DEFAULT_LOG_PATH, DEFAULT_LOG_SIZE);
        if (ret != 0) {
            System.out.printf("createAndInitAgoraService AgoraService.setLogFile fail ret=%d\n", ret);
            return null;
        }

        return service;
    }
}