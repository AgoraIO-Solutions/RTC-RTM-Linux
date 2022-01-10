package common;

import io.agora.rtc.DefaultRtcConnObserver;
import io.agora.rtc.AgoraRtcConn;
import io.agora.rtc.RtcConnInfo;
import io.agora.rtc.AgoraLocalUser;
import io.agora.rtc.AccountInfo;

public class SampleConnectionObserver extends DefaultRtcConnObserver {
    int ret = 0;
    private AgoraLocalUser localUser_;
    private String remoteUserId_;

    public SampleConnectionObserver(AgoraLocalUser localUser, String remoteUserId) {
        localUser_ = localUser;
        remoteUserId_ = remoteUserId;
    }

    @Override
    public void onUserJoined(AgoraRtcConn agora_rtc_conn, String user_id) {
        System.out
                .println("SampleConnectionObserver onUserJoined->" + remoteUserId_ + "->" + localUser_ + "user_id->"
                        + user_id);
        if (remoteUserId_.isEmpty()) {
            System.out.printf("Subscribe streams from all remote users");
            ret = localUser_.subscribeAllAudio();
            if (ret != 0) {
                System.out.printf("subscribeAudio fail ret=%d\n", ret);
                return;
            }
        } else {
            System.out.printf("Subscribe streams from remote user %s \n", remoteUserId_);
            ret = localUser_.subscribeAudio(remoteUserId_);
            if (ret != 0) {
                System.out.printf("subscribeAudio fail ret=%d\n", ret);
                return;
            }
        }
        AccountInfo accountInfo = agora_rtc_conn.getUserInfoByUserAccount(user_id);
        System.out.println(
                "conn.getUserInfo111 uid->" + accountInfo.getUid() + " UserAccount->" + accountInfo.getUserAccount());
    }

    @Override
    public void onConnected(AgoraRtcConn agora_rtc_conn, RtcConnInfo rtcConnInfo, int reason) {
        System.out.println("SampleConnectionObserver onConnected");
    }
}
