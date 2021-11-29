package common;

import io.agora.rtc.DefaultRtcConnObserver;
import io.agora.rtc.AgoraRtcConn;
import io.agora.rtc.RtcConnInfo;

public class SampleConnectionObserver extends DefaultRtcConnObserver {

    @Override
    public void onConnected(AgoraRtcConn conn, RtcConnInfo rtcConnInfo, int reason) {
        System.out.println("SampleConnectionObserver onConnected");
    }
}