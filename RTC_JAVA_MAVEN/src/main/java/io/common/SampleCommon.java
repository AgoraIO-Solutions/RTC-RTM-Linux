package common;

import io.agora.rtc.AgoraService;
import io.agora.rtc.AgoraServiceConfig;
import io.agora.rtc.SDK;
import io.agora.rtc.AgoraRtcConn;
import io.agora.rtc.AgoraParameter;

public class SampleCommon {

    public static String DEFAULT_LOG_PATH = "/tmp/agorasdk.log";
    public static int DEFAULT_LOG_SIZE = 512 * 1024; // default log size is 512 kb

    public static AgoraService createAndInitAgoraService(int enableAudioDevice, int enableAudioProcessor,
            int enableVideo, String appId) {
        SDK.load(); // ensure JNI library load
        AgoraService service = new AgoraService();
        AgoraServiceConfig config = new AgoraServiceConfig();
        config.setEnableAudioDevice(enableAudioDevice);
        config.setEnableAudioProcessor(enableAudioProcessor);
        config.setEnableVideo(enableVideo);
        config.setUseStringUid(1);
	config.setAppId(appId);
        int ret = service.initialize(config);
        if (ret != 0) {
            System.out.printf("createAndInitAgoraService AgoraService.initialize fail ret=%d\n", ret);
            return null;
        }

        System.out.printf("createAndInitAgoraService created log file at %s\n", DEFAULT_LOG_PATH);
        ret = service.setLogFilter(1);
        ret = service.setLogFile(DEFAULT_LOG_PATH, DEFAULT_LOG_SIZE);
        if (ret != 0) {
            System.out.printf("createAndInitAgoraService AgoraService.setLogFile fail ret=%d\n", ret);
            return null;
        }

        return service;
    }

    public static int setCloudProxy(AgoraRtcConn agora_rtc_conn, String proxyOn){
            System.out.printf("setting proxy with %s\n", proxyOn);
	    AgoraParameter agoraParameter = agora_rtc_conn.getAgoraParameter();
	    if (proxyOn.equals("true")) {
	      //agoraParameter.setParameters("{\"rtc.proxy_server\":[2, \"[\\\"128.1.77.34\\\", \\\"128.1.78.146\\\"]\", 0], \"rtc.enable_proxy\":true}");  // from RTSA doc in Chinese
              //agoraParameter.setParameters("{\"rtc.proxy_server\":[2, \"[\\\"184.72.16.87\\\", \\\"35.168.106.53\\\"]\", 0], \"rtc.enable_proxy\":true}");
	      //agoraParameter.setParameters("{\"rtc.proxy_server\":[2,\"\",0]}"); //setting mode w/o IPs does not work. don't use it
              agoraParameter.setParameters("{\"rtc.enable_proxy\":true}");
              System.out.printf("Enable Cloud Proxy.\n");
            }
            else {
              agoraParameter.setParameters("{\"rtc.enable_proxy\":false}");
              System.out.printf("Disable Cloud Proxy.\n");
            }
            return (0);
    }
}
